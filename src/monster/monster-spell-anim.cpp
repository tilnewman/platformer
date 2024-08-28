// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-spell-anim.cpp
//
#include "monster/monster-spell-anim.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/filesystem-util.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"

#include <iostream>

namespace platformer
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

    void MonsterSpellTextureManager::teardown()
    {
        m_textureSets = std::vector<MonsterSpellTextures>();
    }

    void MonsterSpellTextureManager::acquire(Context & t_context, const MonsterSpell t_spell)
    {
        const std::size_t spellIndex{ static_cast<std::size_t>(t_spell) };
        if (spellIndex >= m_textureSets.size())
        {
            return;
        }

        MonsterSpellTextures & set{ m_textureSets.at(spellIndex) };

        if (0 == set.ref_count)
        {
            const std::filesystem::path imageDirPath{
                t_context.settings.media_path /
                std::string("image/monster-spell-anim/").append(toString(t_spell))
            };

            const std::vector<std::filesystem::path> files{ util::findFilesInDirectory(
                imageDirPath, ".png") };

            if (files.empty())
            {
                std::cout << "Error:  MonsterSpellTextureManager::acquire() found no png files in "
                          << imageDirPath << '\n';

                return;
            }

            set.textures.resize(files.size());
            for (std::size_t frameIndex{ 0 }; frameIndex < files.size(); ++frameIndex)
            {
                sf::Texture & texture{ set.textures.at(frameIndex) };
                texture.loadFromFile(files.at(frameIndex).string());
                TextureStats::instance().process(texture);
            }
        }

        ++set.ref_count;
    }

    void MonsterSpellTextureManager::release(const MonsterSpell t_spell)
    {
        const std::size_t spellIndex{ static_cast<std::size_t>(t_spell) };
        if (spellIndex >= m_textureSets.size())
        {
            return;
        }

        MonsterSpellTextures & set{ m_textureSets.at(spellIndex) };

        if (0 == set.ref_count)
        {
            std::cout << "MonsterSpellTextureManager::release(" << toString(t_spell)
                      << ") but the ref_count is already zero.\n";

            return;
        }

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
        if (spellIndex >= m_textureSets.size())
        {
            return;
        }

        const std::vector<sf::Texture> & textures{ m_textureSets.at(spellIndex).textures };

        if (t_frame >= textures.size())
        {
            return;
        }

        t_sprite.setTexture(textures.at(t_frame));
    }

    std::size_t MonsterSpellTextureManager::frameCount(const MonsterSpell t_spell) const
    {
        const std::size_t spellIndex{ static_cast<std::size_t>(t_spell) };
        if (spellIndex >= m_textureSets.size())
        {
            return 0;
        }
        else
        {
            return m_textureSets.at(spellIndex).textures.size();
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
        MonsterSpellAnim & anim{ m_anims.emplace_back() };
        anim.is_moving_right = t_isFacingRight;
        anim.spell           = t_spell;
        MonsterSpellTextureManager::instance().set(anim.sprite, t_spell, 0);
        util::setOriginToCenter(anim.sprite);
        anim.sprite.setPosition(t_pos);

        if (!t_isFacingRight)
        {
            anim.sprite.scale(-1.0f, 1.0f);
        }
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
            anim.sprite.move(t_amount, 0.0f);
        }
    }

} // namespace platformer
