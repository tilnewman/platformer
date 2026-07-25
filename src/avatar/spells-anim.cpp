// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// spells-anim.cpp
//
#include "avatar/spells-anim.hpp"

#include "bramblefore/settings.hpp"
#include "map/level.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/filesystem-util.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    SpellAnim::SpellAnim(
        const Context & t_context,
        const Spell t_spell,
        const sf::Texture & t_texture,
        const float t_timePerFrameSec,
        const sf::Vector2f & t_scale,
        const sf::Vector2f & t_position,
        const bool t_isFacingRight)
        : is_alive{ true }
        , spell{ t_spell }
        , frame_index{ 0 }
        , elapsed_time_sec{ 0.0f }
        , time_per_frame_sec{ t_timePerFrameSec }
        , is_facing_right{ t_isFacingRight }
        , sprite{ t_texture }
        , frames{ toFrames(t_spell) }
        , phase{ SpellPhase::Start }
    {
        sprite.setScale(t_scale);
        util::setOriginToCenter(sprite);
        sprite.setPosition(t_position);

        if (!is_facing_right)
        {
            sprite.setScale({ -1.0f, 1.0f });
        }

        // spells are generally too low so raise them up a bit
        const float vertOffset{ t_context.layout.calScaleBasedOnResolution(
                                    t_context, toCastVertOffset(t_spell)) *
                                t_context.settings.map_scale };

        sprite.move({ 0.0f, vertOffset });
    }

    //

    SpellAnimations::SpellAnimations()
        : m_textureSets{}
        , m_anims{}
        , m_scale{}
    {
        // important size to prevent any reallocations
        m_textureSets.resize(static_cast<std::size_t>(Spell::Count));

        // just a guess, probably no more than 2 or 3 during play
        m_anims.reserve(8);
    }

    void SpellAnimations::setup(const Context & t_context)
    {
        const float scale{ t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.spell_scale) };

        m_scale.x = scale;
        m_scale.y = scale;

        for (std::size_t spellIndex(0); spellIndex < static_cast<std::size_t>(Spell::Count);
             ++spellIndex)
        {
            const Spell spell{ static_cast<Spell>(spellIndex) };

            SpellTextures & set{ m_textureSets.at(spellIndex) };

            const std::filesystem::path iconPath{
                t_context.settings.media_path / "image" / "spell" /
                std::string(toFilesystemName(spell)).append("-icon.png")
            };

            util::TextureLoader::load(set.icon_texture, iconPath, true);

            const std::filesystem::path path{ t_context.settings.media_path / "image" / "spell" /
                                              toFilesystemName(spell) };

            const std::vector<std::filesystem::path> files{ util::findFilesInDirectory(
                path, ".png") };

            M_CHECK(!files.empty(), "Failed to find png files in " << path);

            set.textures.resize(files.size());

            for (std::size_t fileIndex{ 0 }; fileIndex < files.size(); ++fileIndex)
            {
                util::TextureLoader::load(set.textures.at(fileIndex), files.at(fileIndex), true);
            }
        }
    }

    void SpellAnimations::add(
        const Context & t_context,
        const sf::Vector2f & t_pos,
        const Spell t_spell,
        const bool t_isFacingRight)
    {
        const std::size_t spellIndex{ static_cast<std::size_t>(t_spell) };

        M_CHECK(
            (spellIndex < m_textureSets.size()),
            toFilesystemName(t_spell)
                << " of index " << spellIndex << " >= " << m_textureSets.size());

        const auto & textures{ m_textureSets.at(spellIndex).textures };

        M_CHECK(
            !textures.empty(),
            toFilesystemName(t_spell) << " of index " << spellIndex
                                      << " has no images loaded. Maybe setup() was not called?");

        m_anims.emplace_back(
            t_context,
            t_spell,
            textures.at(0),
            timePerFrameSec(t_spell),
            m_scale,
            t_pos,
            t_isFacingRight);
    }

    void SpellAnimations::update(const Context & t_context, const float t_frameTimeSec)
    {
        bool didAnyFinish{ false };
        for (SpellAnim & anim : m_anims)
        {
            if ((SpellPhase::Start == anim.phase) && !anim.frames.start.empty())
            {
                updatePhaseStart(t_context, t_frameTimeSec, anim);
            }
            else if ((SpellPhase::Flying == anim.phase) && !anim.frames.flying.empty())
            {
                updatePhaseFlying(t_context, t_frameTimeSec, anim);
            }
            else if ((SpellPhase::Finish == anim.phase) && !anim.frames.finish.empty())
            {
                updatePhaseFinish(t_context, t_frameTimeSec, anim);
            }
            else
            {
                updateNonFlying(t_context, t_frameTimeSec, anim);
            }

            if (!anim.is_alive)
            {
                didAnyFinish = true;
            }
        }

        if (didAnyFinish)
        {
            std::erase_if(m_anims, [](const SpellAnim & anim) { return !anim.is_alive; });
        }
    }

    void SpellAnimations::updatePhaseStart(
        const Context &, const float t_frameTimeSec, SpellAnim & anim)
    {
        anim.elapsed_time_sec += t_frameTimeSec;
        if (anim.elapsed_time_sec > anim.time_per_frame_sec)
        {
            anim.elapsed_time_sec -= anim.time_per_frame_sec;

            ++anim.frame_index;
            if (anim.frame_index < anim.frames.start.size())
            {
                const std::vector<sf::Texture> & textures{
                    m_textureSets.at(static_cast<std::size_t>(anim.spell)).textures
                };

                anim.sprite.setTexture(textures.at(anim.frames.start.at(anim.frame_index)));
            }
            else
            {
                anim.frame_index = 0;
                anim.phase       = SpellPhase::Flying;
            }
        }
    }

    void SpellAnimations::updatePhaseFlying(
        const Context & t_context, const float t_frameTimeSec, SpellAnim & anim)
    {
        const float moveAmount{ (t_context.settings.spell_speed * t_frameTimeSec) *
                                ((anim.is_facing_right) ? 1.0f : -1.0f) };

        anim.sprite.move({ moveAmount, 0.0f });

        anim.elapsed_time_sec += t_frameTimeSec;
        if (anim.elapsed_time_sec > anim.time_per_frame_sec)
        {
            anim.elapsed_time_sec -= anim.time_per_frame_sec;

            ++anim.frame_index;
            if (anim.frame_index < anim.frames.flying.size())
            {
                const std::vector<sf::Texture> & textures{
                    m_textureSets.at(static_cast<std::size_t>(anim.spell)).textures
                };

                anim.sprite.setTexture(textures.at(anim.frames.flying.at(anim.frame_index)));
            }
            else
            {
                anim.frame_index = 0;

                const sf::FloatRect spellRect{ flyingSpellCollisionRect(
                    anim.spell, anim.sprite.getGlobalBounds(), anim.is_facing_right) };

                if (t_context.level.monsters().avatarAttack(
                        t_context, AttackInfo(toDamage(anim.spell), spellRect)))
                {
                    anim.phase = SpellPhase::Finish;
                    return;
                }

                for (const sf::FloatRect & collRect : t_context.level.collisions())
                {
                    if (spellRect.findIntersection(collRect))
                    {
                        anim.phase = SpellPhase::Finish;
                        return;
                    }
                }

                for (const sf::FloatRect & collRect : t_context.level.layerCollisions())
                {
                    if (spellRect.findIntersection(collRect))
                    {
                        anim.phase = SpellPhase::Finish;
                        return;
                    }
                }
            }
        }
    }

    void SpellAnimations::updatePhaseFinish(
        const Context &, const float t_frameTimeSec, SpellAnim & anim)
    {
        anim.elapsed_time_sec += t_frameTimeSec;
        if (anim.elapsed_time_sec > anim.time_per_frame_sec)
        {
            anim.elapsed_time_sec -= anim.time_per_frame_sec;

            ++anim.frame_index;
            if (anim.frame_index < anim.frames.finish.size())
            {
                const std::vector<sf::Texture> & textures{
                    m_textureSets.at(static_cast<std::size_t>(anim.spell)).textures
                };

                anim.sprite.setTexture(textures.at(anim.frames.finish.at(anim.frame_index)));
            }
            else
            {
                anim.frame_index = 0;
                anim.is_alive    = false;
            }
        }
    }

    void SpellAnimations::updateNonFlying(
        const Context &, const float t_frameTimeSec, SpellAnim & anim)
    {
        anim.elapsed_time_sec += t_frameTimeSec;
        if (anim.elapsed_time_sec > anim.time_per_frame_sec)
        {
            anim.elapsed_time_sec -= anim.time_per_frame_sec;

            const std::vector<sf::Texture> & textures{
                m_textureSets.at(static_cast<std::size_t>(anim.spell)).textures
            };

            ++anim.frame_index;
            if (anim.frame_index < textures.size())
            {
                anim.sprite.setTexture(textures.at(anim.frame_index));
            }
            else
            {
                anim.is_alive = false;
            }
        }
    }

    void SpellAnimations::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const SpellAnim & anim : m_anims)
        {
            // can't imagine a situation where these would be completely offscreen so don't check
            t_target.draw(anim.sprite, t_states);

            // util::drawRectangleShape(
            //     t_target,
            //     flyingSpellCollisionRect(
            //         anim.spell, anim.sprite.getGlobalBounds(), anim.is_facing_right),
            //     false,
            //     sf::Color::Red);
        }
    }

    void SpellAnimations::move(const sf::Vector2f & t_move)
    {
        for (SpellAnim & anim : m_anims)
        {
            anim.sprite.move(t_move);
        }
    }

    const sf::FloatRect SpellAnimations::flyingSpellCollisionRect(
        const Spell t_spell, const sf::FloatRect & t_bounds, const bool t_isFacingRight) const
    {
        sf::FloatRect rect{ t_bounds };

        float horizOffset{ 0.0f };
        float vertOffset{ 0.0f };
        sf::Vector2f scale({ 1.0f, 1.0f });

        if (Spell::Comet == t_spell)
        {
            horizOffset = rect.size.x * -0.05f;
            vertOffset  = rect.size.y * 0.05f;
            scale       = { 0.1f, 0.125f };
        }
        else if (Spell::Fire == t_spell)
        {
            horizOffset = rect.size.x * -0.125f;
            vertOffset  = rect.size.y * 0.15f;
            scale       = { 0.2f, 0.25f };
        }
        else if (Spell::Freeze == t_spell)
        {
            horizOffset = rect.size.x * 0.0f;
            vertOffset  = rect.size.y * 0.04f;
            scale       = { 0.35f, 0.125f };
        }
        else if (Spell::Water == t_spell)
        {
            horizOffset = rect.size.x * -0.05f;
            vertOffset  = rect.size.y * 0.05f;
            scale       = { 0.1f, 0.1f };
        }
        else if (Spell::Explosion == t_spell)
        {
            scale = { 0.3f, 0.3f };
        }
        else if (Spell::Spikes2 == t_spell)
        {
            scale = { 0.5f, 0.5f };
        }
        else if (Spell::Lightning2 == t_spell)
        {
            scale = { 0.3f, 0.4f };
        }
        else if (Spell::KillAll == t_spell)
        {
            scale = { 0.5f, 0.6f };
        }
        else if (Spell::Light == t_spell)
        {
            scale = { 0.6f, 0.6f };
        }
        else if (Spell::SunStrike == t_spell)
        {
            scale = { 0.3f, 0.35f };
        }
        else if (Spell::Tornado == t_spell)
        {
            scale = { 0.3f, 0.45f };
        }
        else if (Spell::TeslaBall == t_spell)
        {
            scale = { 0.5f, 0.5f };
        }
        else if (Spell::Spikes1 == t_spell)
        {
            scale = { 0.5f, 0.2f };
        }
        else if (Spell::Hypno == t_spell)
        {
            scale = { 0.75f, 0.75f };
        }
        else if (Spell::Lightning1 == t_spell)
        {
            horizOffset = rect.size.x * -0.075f;
            vertOffset  = rect.size.y * 0.0f;
            scale       = { 0.5f, 0.3f };
        }
        else if (Spell::MidasHand == t_spell)
        {
            horizOffset = rect.size.x * -0.1f;
            vertOffset  = rect.size.y * 0.0f;
            scale       = { 0.6f, 0.5f };
        }

        util::scaleRectInPlace(rect, scale);

        rect.position.y += vertOffset;

        if (t_isFacingRight)
        {
            rect.position.x -= horizOffset;
        }
        else
        {
            rect.position.x += horizOffset;
        }

        return rect;
    }

} // namespace bramblefore
