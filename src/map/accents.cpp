// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// accents.cpp
//
#include "map/accents.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/check-macros.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"

namespace bramblefore
{

    AccentAnimations::AccentAnimations()
        : m_textures{}
        , m_anims{}
        , m_elapsedFireTimeSec{ 0.0f }
        , m_elapsedVineTimeSec{ 0.0f }
        , m_timePerFireFrameSec{ 0.0f }
        , m_timePerVineFrameSec{ 0.0f }
        , m_scale{}
    {
        m_anims.reserve(32); // just a harmless guess
    }

    void AccentAnimations::setup(const Settings & t_settings)
    {
        m_scale.x = t_settings.tile_scale;
        m_scale.y = t_settings.tile_scale;

        m_timePerFireFrameSec = t_settings.accent_fire_time_per_frame;
        m_timePerVineFrameSec = t_settings.accent_vine_time_per_frame;

        m_textures.reserve(static_cast<std::size_t>(Accent::Count));

        for (std::size_t accentIndex(0); accentIndex < static_cast<std::size_t>(Accent::Count);
             ++accentIndex)
        {
            const Accent accent{ static_cast<Accent>(accentIndex) };

            sf::Texture & texture{ m_textures.emplace_back() };

            texture.loadFromFile(
                (t_settings.media_path / "image/accent-anim" / toFilename(accent)).string());

            texture.setSmooth(true);

            TextureStats::instance().process(texture);
        }
    }

    void AccentAnimations::add(
        const Context & t_context, const sf::FloatRect & t_rect, const std::string & t_name)
    {
        const Accent accent{ stringToAccent(t_name) };
        M_CHECK((Accent::Count != accent), "\"" << t_name << "\" is an unknown name");

        AccentAnim & anim{ m_anims.emplace_back() };
        anim.which      = accent;
        anim.anim_index = t_context.random.zeroToOneLessThan(frameCount(anim.which));
        anim.sprite.setTexture(m_textures.at(static_cast<std::size_t>(accent)), true);
        anim.sprite.setTextureRect(textureRect(accent, 0));
        anim.sprite.setScale(m_scale);

        if (isVine(accent))
        {
            anim.sprite.setPosition(
                (util::center(t_rect).x - (anim.sprite.getGlobalBounds().width * 0.5f)),
                util::center(t_rect).y);
        }
        else
        {
            anim.sprite.setPosition(
                (util::center(t_rect).x - (anim.sprite.getGlobalBounds().width * 0.5f)),
                (util::bottom(t_rect) - anim.sprite.getGlobalBounds().height));
        }
    }

    std::size_t AccentAnimations::frameCount(const Accent t_which) const noexcept
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(t_which)) };

        if (texture.getSize().y > 0)
        {
            return static_cast<std::size_t>(texture.getSize().x / texture.getSize().y);
        }
        else
        {
            return 0;
        }
    }

    sf::IntRect AccentAnimations::textureRect(
        const Accent t_which, const std::size_t t_frame) const noexcept
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(t_which)) };

        sf::IntRect rect;
        rect.width  = static_cast<int>(texture.getSize().y);
        rect.height = rect.width;
        rect.top    = 0;
        rect.left   = (static_cast<int>(t_frame) * rect.width);

        return rect;
    }

    void AccentAnimations::update(Context &, const float t_frameTimeSec)
    {
        m_elapsedFireTimeSec += t_frameTimeSec;
        if (m_elapsedFireTimeSec > m_timePerFireFrameSec)
        {
            m_elapsedFireTimeSec -= m_timePerFireFrameSec;

            for (AccentAnim & anim : m_anims)
            {
                if (isVine(anim.which))
                {
                    continue;
                }

                ++anim.anim_index;
                if (anim.anim_index >= frameCount(anim.which))
                {
                    anim.anim_index = 0;
                }

                anim.sprite.setTextureRect(textureRect(anim.which, anim.anim_index));
            }
        }

        m_elapsedVineTimeSec += t_frameTimeSec;
        if (m_elapsedVineTimeSec > m_timePerVineFrameSec)
        {
            m_elapsedVineTimeSec -= m_timePerVineFrameSec;

            for (AccentAnim & anim : m_anims)
            {
                if (!isVine(anim.which))
                {
                    continue;
                }

                ++anim.anim_index;
                if (anim.anim_index >= frameCount(anim.which))
                {
                    anim.anim_index = 0;
                }

                anim.sprite.setTextureRect(textureRect(anim.which, anim.anim_index));
            }
        }
    }

    void AccentAnimations::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const AccentAnim & anim : m_anims)
        {
            if (t_context.layout.wholeRect().intersects(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void AccentAnimations::move(const float t_amount)
    {
        for (AccentAnim & anim : m_anims)
        {
            anim.sprite.move(t_amount, 0.0f);
        }
    }

} // namespace bramblefore
