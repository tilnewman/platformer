// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// spells-anim.cpp
//
#include "avatar/spells-anim.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/check-macros.hpp"
#include "util/filesystem-util.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

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
                t_context.settings.media_path / "image/spell-anim" /
                std::string(toFilesystemName(spell)).append("-icon.png")
            };

            set.icon_texture.loadFromFile(iconPath.string());
            TextureStats::instance().process(set.icon_texture);
            set.icon_texture.setSmooth(true);

            const std::filesystem::path path{ t_context.settings.media_path / "image/spell-anim" /
                                              toFilesystemName(spell) };

            const std::vector<std::filesystem::path> files{ util::findFilesInDirectory(
                path, ".png") };

            M_CHECK(!files.empty(), "Failed to find png files in " << path);

            set.textures.resize(files.size());

            for (std::size_t fileIndex{ 0 }; fileIndex < files.size(); ++fileIndex)
            {
                sf::Texture & texture{ set.textures.at(fileIndex) };
                texture.loadFromFile(files.at(fileIndex).string());
                texture.setSmooth(true);
                TextureStats::instance().process(texture);
            }
        }
    }

    void SpellAnimations::add(const sf::Vector2f & t_pos, const Spell t_spell)
    {
        const std::vector<sf::Texture> & textures{
            m_textureSets.at(static_cast<std::size_t>(t_spell)).textures
        };

        if (textures.empty())
        {
            return;
        }

        SpellAnim & anim{ m_anims.emplace_back() };
        anim.is_alive           = true;
        anim.spell              = t_spell;
        anim.time_per_frame_sec = timePerFrameSec(t_spell);
        anim.sprite.setTexture(textures.at(0));
        anim.sprite.setScale(m_scale);
        util::setOriginToCenter(anim.sprite);
        anim.sprite.setPosition(t_pos);
    }

    void SpellAnimations::update(Context &, const float t_frameTimeSec)
    {
        bool didAnyFinish{ false };

        for (SpellAnim & anim : m_anims)
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
                    didAnyFinish  = true;
                    anim.is_alive = false;
                }
            }
        }

        if (didAnyFinish)
        {
            std::erase_if(m_anims, [](const SpellAnim & anim) { return !anim.is_alive; });
        }
    }

    void SpellAnimations::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const SpellAnim & anim : m_anims)
        {
            t_target.draw(anim.sprite, t_states);
        }
    }

    void SpellAnimations::move(const float t_amount)
    {
        for (SpellAnim & anim : m_anims)
        {
            anim.sprite.move(t_amount, 0.0f);
        }
    }

} // namespace bramblefore
