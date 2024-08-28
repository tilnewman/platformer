// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// avatar-spell-anims.cpp
//
#include "avatar/avatar-spell-anims.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/check-macros.hpp"
#include "util/filesystem-util.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"

#include <iostream>

namespace bramblefore
{

    AvatarSpellAnimations::AvatarSpellAnimations()
        : m_druidTextures{}
        , m_enchantressTextures{}
        , m_witchTextures{}
        , m_anims{}
        , m_timePerFrameSec{ 0.1f }
        , m_scale{}
    {
        // harmless guess, probably no more than 2 or 3 at once during gameplay
        m_anims.reserve(8);
    }

    void AvatarSpellAnimations::setup(const Settings & t_settings)
    {
        m_scale.x = t_settings.avatar_scale;
        m_scale.y = t_settings.avatar_scale;

        loadTextures(m_druidTextures.first, (t_settings.media_path / "image/avatar/druid/fire"));

        loadTextures(
            m_druidTextures.second, (t_settings.media_path / "image/avatar/druid/fire_extra"));

        loadTextures(
            m_enchantressTextures.first, (t_settings.media_path / "image/avatar/enchantress/fire"));

        loadTextures(
            m_enchantressTextures.second,
            (t_settings.media_path / "image/avatar/enchantress/fire_extra"));

        loadTextures(m_witchTextures.first, (t_settings.media_path / "image/avatar/witch/fire"));

        loadTextures(
            m_witchTextures.second, (t_settings.media_path / "image/avatar/witch/fire_extra"));
    }

    void AvatarSpellAnimations::add(
        const sf::Vector2f & t_pos,
        const AvatarType t_type,
        const bool t_isFirstAttack,
        const bool t_isFacingRight)
    {
        if (!isSpellCaster(t_type))
        {
            return;
        }

        AvatarSpellAnim & anim{ m_anims.emplace_back() };
        anim.is_first_anim   = t_isFirstAttack;
        anim.type            = t_type;
        anim.is_moving_right = t_isFacingRight;
        anim.sprite.setTexture(getTextures(t_type, t_isFirstAttack).at(0));
        util::setOriginToCenter(anim.sprite);
        anim.sprite.scale(m_scale);
        anim.sprite.setPosition(t_pos);

        if (!t_isFacingRight)
        {
            anim.sprite.scale(-1.0f, 1.0f);
        }
    }

    void AvatarSpellAnimations::update(const float t_frameTimeSec)
    {
        bool didAnyFinish{ false };

        for (AvatarSpellAnim & anim : m_anims)
        {
            const float moveAmount{ t_frameTimeSec * 40.0f };
            if (anim.is_moving_right)
            {
                anim.sprite.move(moveAmount, 0.0f);
            }
            else
            {
                anim.sprite.move(-moveAmount, 0.0f);
            }

            anim.elapsed_time_sec += t_frameTimeSec;
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
            std::erase_if(m_anims, [](const AvatarSpellAnim & anim) { return !anim.is_alive; });
        }
    }

    void AvatarSpellAnimations::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const AvatarSpellAnim & anim : m_anims)
        {
            t_target.draw(anim.sprite, t_states);
        }
    }

    void AvatarSpellAnimations::move(const float t_amount)
    {
        for (AvatarSpellAnim & anim : m_anims)
        {
            anim.sprite.move(t_amount, 0.0f);
        }
    }

    void AvatarSpellAnimations::loadTextures(
        std::vector<sf::Texture> & t_textures, const std::filesystem::path & t_path) const
    {
        const std::vector<std::filesystem::path> files{ util::findFilesInDirectory(
            t_path, ".png") };

        M_CHECK(!files.empty(), "Found no files in: " << t_path);

        t_textures.resize(files.size());

        for (std::size_t frameCounter(0); frameCounter < files.size(); ++frameCounter)
        {
            sf::Texture & texture{ t_textures.at(frameCounter) };
            texture.loadFromFile(files.at(frameCounter).string());
            TextureStats::instance().process(texture);
        }
    }

    const std::vector<sf::Texture> & AvatarSpellAnimations::getTextures(
        const AvatarType t_type, const bool t_isFirstAttack) const
    {
        if (AvatarType::Druid == t_type)
        {
            if (t_isFirstAttack)
            {
                return m_druidTextures.first;
            }
            else
            {
                return m_druidTextures.second;
            }
        }
        else if (AvatarType::Enchantress == t_type)
        {
            if (t_isFirstAttack)
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
            // default to witch if t_type is invalid -i don't have the heart to throw
            if (t_isFirstAttack)
            {
                return m_witchTextures.first;
            }
            else
            {
                return m_witchTextures.second;
            }
        }
    }

} // namespace bramblefore
