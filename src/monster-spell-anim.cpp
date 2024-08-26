// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-spell-anim.cpp
//
#include "monster-spell-anim.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "filesystem-util.hpp"
#include "random.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

#include <iostream>

namespace platformer
{

    MonsterSpellTextureManager::MonsterSpellTextureManager()
        : m_textureSets{}
    {
        // one time size to avoid any reallocations
        m_textureSets.resize(static_cast<std::size_t>(MonsterSpell::Count));
    }

    void MonsterSpellTextureManager::setup(const Settings & t_settings)
    {
        for (std::size_t spellIndex{ 0 };
             spellIndex < static_cast<std::size_t>(MonsterSpell::Count);
             ++spellIndex)
        {
            std::vector<sf::Texture> & textures{ m_textureSets.at(spellIndex).textures };

            // bail if the textures have already been loaded
            if (!textures.empty())
            {
                return;
            }

            const MonsterSpell spell{ static_cast<MonsterSpell>(spellIndex) };

            const std::filesystem::path imageDirPath{
                t_settings.media_path /
                std::string("image/monster-spell-anim/").append(toString(spell))
            };

            const std::vector<std::filesystem::path> files{ util::findFilesInDirectory(
                imageDirPath, ".png") };

            if (files.empty())
            {
                std::cout << "Error:  MonsterSpellTextureManager::setup() found no png files in "
                          << imageDirPath << '\n';

                continue;
            }

            textures.resize(files.size());
            for (std::size_t frameIndex{ 0 }; frameIndex < files.size(); ++frameIndex)
            {
                sf::Texture & texture{ textures.at(frameIndex) };
                texture.loadFromFile(files.at(frameIndex).string());
                TextureStats::instance().process(texture);
            }
        }
    }

    void MonsterSpellTextureManager::set(
        sf::Sprite & t_sprite, const MonsterSpell t_spell, const std::size_t t_frame) const
    {
        const std::vector<sf::Texture> & textures{
            m_textureSets.at(static_cast<std::size_t>(t_spell)).textures
        };

        if (t_frame < textures.size())
        {
            t_sprite.setTexture(textures.at(t_frame));
        }
    }

    //

    MonsterSpellTextureManager MonsterSpellAnimations::m_textureManager;

    MonsterSpellAnimations::MonsterSpellAnimations()
        : m_anims{}
        , m_timePerFrameSec{ 0.1f }
    {}

    void MonsterSpellAnimations::setup(const Settings & settings)
    {
        m_textureManager.setup(settings);
    }

    void MonsterSpellAnimations::add(
        const sf::Vector2f & t_pos, const MonsterSpell t_spell, const bool t_isFacingRight)
    {
        MonsterSpellAnim & anim{ m_anims.emplace_back() };
        anim.is_moving_right = t_isFacingRight;
        anim.spell           = t_spell;
        m_textureManager.set(anim.sprite, t_spell, 0);
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
                if (anim.frame_index < m_textureManager.getFrameCount(anim.spell))
                {
                    m_textureManager.set(anim.sprite, anim.spell, anim.frame_index);
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
