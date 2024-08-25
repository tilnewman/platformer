// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// accents.cpp
//
#include "accents.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "random.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

namespace platformer
{

    AccentAnimations::AccentAnimations()
        : m_textures()
        , m_anims()
        , m_elapsedFireTimeSec(0.0f)
        , m_elapsedVineTimeSec(0.0f)
        , m_timePerFireFrameSec(0.0f)
        , m_timePerVineFrameSec(0.0f)
        , m_scale(0.0f, 0.0f)
    {
        m_anims.reserve(256);
    }

    void AccentAnimations::setup(const Settings & settings)
    {
        m_scale.x = settings.tile_scale;
        m_scale.y = settings.tile_scale;

        m_timePerFireFrameSec = settings.accent_fire_time_per_frame;
        m_timePerVineFrameSec = settings.accent_vine_time_per_frame;

        m_textures.reserve(static_cast<std::size_t>(Accent::Count));

        for (std::size_t i(0); i < static_cast<std::size_t>(Accent::Count); ++i)
        {
            const Accent accent{ static_cast<Accent>(i) };

            sf::Texture & texture{ m_textures.emplace_back() };

            texture.loadFromFile(
                (settings.media_path / "image/accent-anim" / toFilename(accent)).string());

            texture.setSmooth(true);

            TextureStats::instance().process(texture);
        }
    }

    void AccentAnimations::add(
        const Context & context, const sf::FloatRect & rect, const std::string & name)
    {
        const Accent accent{ stringToAccent(name) };
        if (Accent::Count == accent)
        {
            std::cout << "Error: AccentAnimations::add(\"" << name
                      << "\") given an unknown name.\n";

            return;
        }

        AccentAnim & anim{ m_anims.emplace_back() };
        anim.which      = accent;
        anim.anim_index = context.random.zeroToOneLessThan(frameCount(anim.which));
        anim.sprite.setTexture(m_textures.at(static_cast<std::size_t>(accent)), true);
        anim.sprite.setTextureRect(textureRect(accent, 0));
        anim.sprite.setScale(m_scale);

        if (isVine(accent))
        {
            anim.sprite.setPosition(
                (util::center(rect).x - (anim.sprite.getGlobalBounds().width * 0.5f)),
                util::center(rect).y);
        }
        else
        {
            anim.sprite.setPosition(
                (util::center(rect).x - (anim.sprite.getGlobalBounds().width * 0.5f)),
                (util::bottom(rect) - anim.sprite.getGlobalBounds().height));
        }
    }

    std::size_t AccentAnimations::frameCount(const Accent which) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(which)) };

        if (texture.getSize().y > 0)
        {
            return static_cast<std::size_t>(texture.getSize().x / texture.getSize().y);
        }
        else
        {
            return 0;
        }
    }

    const sf::IntRect
        AccentAnimations::textureRect(const Accent which, const std::size_t frame) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(which)) };

        sf::IntRect rect;
        rect.width  = static_cast<int>(texture.getSize().y);
        rect.height = rect.width;
        rect.top    = 0;
        rect.left   = (static_cast<int>(frame) * rect.width);

        return rect;
    }

    void AccentAnimations::update(Context &, const float frameTimeSec)
    {
        m_elapsedFireTimeSec += frameTimeSec;
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

        m_elapsedVineTimeSec += frameTimeSec;
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
        const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const AccentAnim & anim : m_anims)
        {
            if (context.layout.wholeRect().intersects(anim.sprite.getGlobalBounds()))
            {
                target.draw(anim.sprite, states);
            }
        }
    }

    void AccentAnimations::move(const float amount)
    {
        for (AccentAnim & anim : m_anims)
        {
            anim.sprite.move(amount, 0.0f);
        }
    }

} // namespace platformer
