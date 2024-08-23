// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// spells-anim.cpp
//
#include "spells-anim.hpp"

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

    SpellAnimations::SpellAnimations()
        : m_textureSets{}
        , m_anims{}
        , m_scale{ 0.0f, 0.0f }
    {
        m_textureSets.resize(static_cast<std::size_t>(Spell::Count));
        m_anims.reserve(8); // just a guess, probably no more than 2 or 3 during play
    }

    void SpellAnimations::setup(const Settings & settings)
    {
        m_scale.x = settings.spell_scale;
        m_scale.y = settings.spell_scale;

        for (std::size_t spellIndex(0); spellIndex < static_cast<std::size_t>(Spell::Count);
             ++spellIndex)
        {
            const Spell spell{ static_cast<Spell>(spellIndex) };

            SpellTextures & set{ m_textureSets.at(spellIndex) };

            const std::filesystem::path iconPath{
                settings.media_path / "image/spell-anim" /
                std::string(toFilesystemName(spell)).append("-icon.png")
            };

            set.icon_texture.loadFromFile(iconPath.string());
            TextureStats::instance().process(set.icon_texture);
            set.icon_texture.setSmooth(true);

            const std::filesystem::path path{ settings.media_path / "image/spell-anim" /
                                              toFilesystemName(spell) };

            const std::vector<std::filesystem::path> files{ util::findFilesInDirectory(
                path, ".png") };

            if (files.empty())
            {
                std::cout << "Error:  SpellAnimations::setup() failed to find any spells in "
                          << path << '\n';

                continue;
            }

            set.textures.resize(files.size());

            for (std::size_t frameIndex{ 0 }; frameIndex < files.size(); ++frameIndex)
            {
                sf::Texture & texture{ set.textures.at(frameIndex) };
                texture.loadFromFile(files.at(frameIndex).string());
                texture.setSmooth(true);
                TextureStats::instance().process(texture);
            }
        }
    }

    void SpellAnimations::add(const sf::Vector2f & pos, const Spell spell)
    {
        if (Spell::Count == spell)
        {
            std::cout << "Error:  SpellAnimations::add() given an unknown spell.\n";
            return;
        }

        const std::vector<sf::Texture> & textures{
            m_textureSets.at(static_cast<std::size_t>(spell)).textures
        };

        if (textures.empty())
        {
            return;
        }

        SpellAnim & anim{ m_anims.emplace_back() };
        anim.is_alive           = true;
        anim.spell              = spell;
        anim.time_per_frame_sec = timePerFrameSec(spell);
        anim.sprite.setTexture(textures.at(0));
        anim.sprite.setScale(m_scale);
        util::setOriginToCenter(anim.sprite);
        anim.sprite.setPosition(pos);
    }

    void SpellAnimations::update(Context &, const float frameTimeSec)
    {
        bool didAnyFinish{ false };

        for (SpellAnim & anim : m_anims)
        {
            anim.elapsed_time_sec += frameTimeSec;
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
            m_anims.erase(
                std::remove_if(
                    std::begin(m_anims),
                    std::end(m_anims),
                    [](const SpellAnim & anim) { return !anim.is_alive; }),
                std::end(m_anims));
        }
    }

    void SpellAnimations::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const SpellAnim & anim : m_anims)
        {
            target.draw(anim.sprite, states);
        }
    }

    void SpellAnimations::move(const float amount)
    {
        for (SpellAnim & anim : m_anims)
        {
            anim.sprite.move(amount, 0.0f);
        }
    }

} // namespace platformer
