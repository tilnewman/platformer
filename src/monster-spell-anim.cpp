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
        : m_textureSets()
    {
        m_textureSets.resize(static_cast<std::size_t>(MonsterSpell::Count));
    }

    void MonsterSpellTextureManager::setup(const Settings & settings)
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
                settings.media_path /
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
        sf::Sprite & sprite, const MonsterSpell spell, const std::size_t frame) const
    {
        const std::vector<sf::Texture> & textures{
            m_textureSets.at(static_cast<std::size_t>(spell)).textures
        };

        if (frame < textures.size())
        {
            sprite.setTexture(textures.at(frame));
        }
    }

    //

    MonsterSpellTextureManager MonsterSpellAnimations::m_textureManager;

    MonsterSpellAnimations::MonsterSpellAnimations()
        : m_anims()
        , m_timePerFrameSec(0.1f)
    {}

    void MonsterSpellAnimations::setup(const Settings & settings)
    {
        m_textureManager.setup(settings);
    }

    void MonsterSpellAnimations::add(
        const sf::Vector2f & pos, const MonsterSpell spell, const bool isFacingRight)
    {
        MonsterSpellAnim & anim{ m_anims.emplace_back() };
        anim.is_moving_right = isFacingRight;
        anim.spell           = spell;
        m_textureManager.set(anim.sprite, spell, 0);
        util::setOriginToCenter(anim.sprite);
        anim.sprite.setPosition(pos);

        if (!isFacingRight)
        {
            anim.sprite.scale(-1.0f, 1.0f);
        }
    }

    void MonsterSpellAnimations::update(const float frameTimeSec)
    {
        bool didAnyFinish{ false };

        for (MonsterSpellAnim & anim : m_anims)
        {
            anim.elapsed_time_sec += frameTimeSec;
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
            m_anims.erase(
                std::remove_if(
                    std::begin(m_anims),
                    std::end(m_anims),
                    [](const MonsterSpellAnim & anim) { return !anim.is_alive; }),
                std::end(m_anims));
        }
    }

    void MonsterSpellAnimations::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const MonsterSpellAnim & anim : m_anims)
        {
            target.draw(anim.sprite, states);
        }
    }

    void MonsterSpellAnimations::move(const float amount)
    {
        for (MonsterSpellAnim & anim : m_anims)
        {
            anim.sprite.move(amount, 0.0f);
        }
    }

} // namespace platformer
