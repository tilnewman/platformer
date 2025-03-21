// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-spell-anim.cpp
//
#include "monster/monster-spell-anim.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/filesystem-util.hpp"
#include "util/random.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    MonsterSpellTextureManager::MonsterSpellTextureManager()
        : m_textureSets{}
    {}

    MonsterSpellTextureManager & MonsterSpellTextureManager::instance()
    {
        static MonsterSpellTextureManager monsterSpellTextureManager;
        return monsterSpellTextureManager;
    }

    void MonsterSpellTextureManager::setup(const Settings &)
    {
        // one time size to avoid any reallocations
        m_textureSets.resize(static_cast<std::size_t>(MonsterSpell::Count));
    }

    void MonsterSpellTextureManager::teardown() { m_textureSets.clear(); }

    void MonsterSpellTextureManager::acquire(const Context & t_context, const MonsterSpell t_spell)
    {
        const std::size_t spellIndex{ static_cast<std::size_t>(t_spell) };

        M_CHECK(
            (spellIndex < m_textureSets.size()),
            "MonsterSpell " << toString(t_spell) << " of index " << spellIndex
                            << " >= " << m_textureSets.size() << " -maybe setup() was not called?");

        MonsterSpellTextures & set{ m_textureSets.at(spellIndex) };

        if (0 == set.ref_count)
        {
            const std::filesystem::path imageDirPath{
                t_context.settings.media_path /
                std::string("image/monster-spell/").append(toString(t_spell))
            };

            const std::vector<std::filesystem::path> files{ util::findFilesInDirectory(
                imageDirPath, ".png") };

            M_CHECK(!files.empty(), "Found no png files in " << imageDirPath);

            set.textures.resize(files.size());
            for (std::size_t frameIndex{ 0 }; frameIndex < files.size(); ++frameIndex)
            {
                util::TextureLoader::load(set.textures.at(frameIndex), files.at(frameIndex));
            }
        }

        ++set.ref_count;
    }

    void MonsterSpellTextureManager::release(const MonsterSpell t_spell)
    {
        const std::size_t spellIndex{ static_cast<std::size_t>(t_spell) };

        M_CHECK(
            (spellIndex < m_textureSets.size()),
            "MonsterSpell " << toString(t_spell) << " of index " << spellIndex
                            << " >= " << m_textureSets.size() << " -maybe setup() was not called?");

        MonsterSpellTextures & set{ m_textureSets.at(spellIndex) };

        M_CHECK((0 != set.ref_count), toString(t_spell) << "'s ref_count is already zero!");

        if (1 == set.ref_count)
        {
            for (sf::Texture & texture : set.textures)
            {
                texture = sf::Texture();
            }
        }

        --set.ref_count;
    }

    void MonsterSpellTextureManager::set(
        sf::Sprite & t_sprite, const MonsterSpell t_spell, const std::size_t t_frame) const
    {
        const std::size_t spellIndex{ static_cast<std::size_t>(t_spell) };

        M_CHECK(
            (spellIndex < m_textureSets.size()),
            "MonsterSpell " << toString(t_spell) << " of index " << spellIndex
                            << " >= " << m_textureSets.size() << " -maybe setup() was not called?");

        const std::vector<sf::Texture> & textures{ m_textureSets.at(spellIndex).textures };

        M_CHECK((t_frame < textures.size()), "t_frame=" << t_frame << " >= " << textures.size());

        t_sprite.setTexture(textures.at(t_frame));
    }

    std::size_t MonsterSpellTextureManager::frameCount(const MonsterSpell t_spell) const
    {
        const std::size_t spellIndex{ static_cast<std::size_t>(t_spell) };

        M_CHECK(
            (spellIndex < m_textureSets.size()),
            "MonsterSpell " << toString(t_spell) << " of index " << spellIndex
                            << " >= " << m_textureSets.size() << " -maybe setup() was not called?");

        return m_textureSets.at(spellIndex).textures.size();
    }

    //

    MonsterSpellAnim::MonsterSpellAnim(
        const MonsterSpell t_spell,
        const bool t_isFacingRight,
        const sf::Vector2f & t_scale,
        const sf::Vector2f & t_position)
        : is_alive{ true }
        , spell{ t_spell }
        , frame_index{ 0 }
        , elapsed_time_sec{ 0.0f }
        , is_moving_right{ t_isFacingRight }
        , sprite{ util::SfmlDefaults::instance().texture() }
    {
        MonsterSpellTextureManager::instance().set(sprite, t_spell, 0);
        sprite.setScale(t_scale);
        util::setOriginToCenter(sprite);
        sprite.setPosition(t_position);

        if (!t_isFacingRight)
        {
            sprite.scale({ -1.0f, 1.0f });
        }
    }

    //

    MonsterSpellAnimations::MonsterSpellAnimations()
        : m_anims{}
        , m_timePerFrameSec{ 0.1f }
    {}

    void MonsterSpellAnimations::add(
        const sf::Vector2f & t_pos, const MonsterSpell t_spell, const bool t_isFacingRight)
    {
        m_anims.emplace_back(t_spell, t_isFacingRight, sf::Vector2f{ 1.0f, 1.0f }, t_pos);
    }

    void MonsterSpellAnimations::update(const float t_frameTimeSec)
    {
        bool didAnyFinish{ false };

        for (MonsterSpellAnim & anim : m_anims)
        {
            anim.elapsed_time_sec += t_frameTimeSec;
            if (anim.elapsed_time_sec > m_timePerFrameSec)
            {
                anim.elapsed_time_sec -= m_timePerFrameSec;

                ++anim.frame_index;
                if (anim.frame_index <
                    MonsterSpellTextureManager::instance().frameCount(anim.spell))
                {
                    MonsterSpellTextureManager::instance().set(
                        anim.sprite, anim.spell, anim.frame_index);
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
            std::erase_if(m_anims, [](const MonsterSpellAnim & anim) { return !anim.is_alive; });
        }
    }

    void MonsterSpellAnimations::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const MonsterSpellAnim & anim : m_anims)
        {
            t_target.draw(anim.sprite, t_states);
        }
    }

    void MonsterSpellAnimations::move(const float t_amount)
    {
        for (MonsterSpellAnim & anim : m_anims)
        {
            anim.sprite.move({ t_amount, 0.0f });
        }
    }

} // namespace bramblefore
