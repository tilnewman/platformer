// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// avatar-spell-anims.cpp
//
#include "avatar-spell-anims.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "filesystem-util.hpp"
#include "random.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

namespace platformer
{

    AvatarSpellAnimations::AvatarSpellAnimations()
        : m_druidTextures()
        , m_enchantressTextures()
        , m_witchTextures()
        , m_anims()
        , m_timePerFrameSec(0.1f)
        , m_scale(0.0f, 0.0f)
    {
        // harmless guess, probably no more than 2 or 3 at once during gameplay
        m_anims.reserve(8);
    }

    void AvatarSpellAnimations::setup(const Settings & settings)
    {
        m_scale.x = settings.avatar_scale;
        m_scale.y = settings.avatar_scale;

        loadTextures(m_druidTextures.first, (settings.media_path / "image/avatar/druid/fire"));

        loadTextures(
            m_druidTextures.second, (settings.media_path / "image/avatar/druid/fire_extra"));

        loadTextures(
            m_enchantressTextures.first, (settings.media_path / "image/avatar/enchantress/fire"));

        loadTextures(
            m_enchantressTextures.second,
            (settings.media_path / "image/avatar/enchantress/fire_extra"));

        loadTextures(m_witchTextures.first, (settings.media_path / "image/avatar/witch/fire"));

        loadTextures(
            m_witchTextures.second, (settings.media_path / "image/avatar/witch/fire_extra"));
    }

    void AvatarSpellAnimations::add(
        const sf::Vector2f & pos,
        const AvatarType type,
        const bool isFirstAttack,
        const bool isFacingRight)
    {
        if ((type != AvatarType::Druid) && (type != AvatarType::Enchantress) &&
            (type != AvatarType::Witch))
        {
            return;
        }

        const std::vector<sf::Texture> & textures{ getTextures(type, isFirstAttack) };

        AvatarSpellAnim & anim{ m_anims.emplace_back() };
        anim.is_first_anim   = isFirstAttack;
        anim.type            = type;
        anim.is_moving_right = isFacingRight;
        anim.sprite.setTexture(textures.at(0));
        util::setOriginToCenter(anim.sprite);
        anim.sprite.scale(m_scale);
        anim.sprite.setPosition(pos);

        if (!isFacingRight)
        {
            anim.sprite.scale(-1.0f, 1.0f);
        }
    }

    void AvatarSpellAnimations::update(const float frameTimeSec)
    {
        bool didAnyFinish{ false };

        for (AvatarSpellAnim & anim : m_anims)
        {
            const float moveAmount{ frameTimeSec * 40.0f };
            if (anim.is_moving_right)
            {
                anim.sprite.move(moveAmount, 0.0f);
            }
            else
            {
                anim.sprite.move(-moveAmount, 0.0f);
            }

            anim.elapsed_time_sec += frameTimeSec;
            if (anim.elapsed_time_sec > m_timePerFrameSec)
            {
                anim.elapsed_time_sec -= m_timePerFrameSec;

                const std::vector<sf::Texture> & textures{ getTextures(
                    anim.type, anim.is_first_anim) };

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
                    [](const AvatarSpellAnim & anim) { return !anim.is_alive; }),
                std::end(m_anims));
        }
    }

    void AvatarSpellAnimations::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const AvatarSpellAnim & anim : m_anims)
        {
            target.draw(anim.sprite, states);
        }
    }

    void AvatarSpellAnimations::move(const float amount)
    {
        for (AvatarSpellAnim & anim : m_anims)
        {
            anim.sprite.move(amount, 0.0f);
        }
    }

    void AvatarSpellAnimations::loadTextures(
        std::vector<sf::Texture> & textures, const std::filesystem::path & path) const
    {
        const std::vector<std::filesystem::path> files{ util::findFilesInDirectory(path, ".png") };

        if (files.empty())
        {
            std::cout << "Error:  AvatarSpellAnimations::loadTextures(" << path
                      << ") found no files.\n";

            return;
        }

        textures.resize(files.size());

        for (std::size_t frameCounter(0); frameCounter < files.size(); ++frameCounter)
        {
            sf::Texture & texture{ textures.at(frameCounter) };
            texture.loadFromFile(files.at(frameCounter).string());
            TextureStats::instance().process(texture);
        }
    }

    const std::vector<sf::Texture> &
        AvatarSpellAnimations::getTextures(const AvatarType type, const bool isFirstAttack) const
    {
        if (AvatarType::Druid == type)
        {
            if (isFirstAttack)
            {
                return m_druidTextures.first;
            }
            else
            {
                return m_druidTextures.second;
            }
        }
        else if (AvatarType::Enchantress == type)
        {
            if (isFirstAttack)
            {
                return m_enchantressTextures.first;
            }
            else
            {
                return m_enchantressTextures.second;
            }
        }
        else
        {
            // default to witch if type is invalid -i don't have the heart to throw
            if (isFirstAttack)
            {
                return m_witchTextures.first;
            }
            else
            {
                return m_witchTextures.second;
            }
        }
    }

} // namespace platformer
