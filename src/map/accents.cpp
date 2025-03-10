// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// accents.cpp
//
#include "map/accents.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    AccentAnim::AccentAnim(
        const Context & t_context,
        const Accent t_accent,
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const sf::FloatRect & t_screenRect,
        const std::size_t t_frameIndex)
        : which{ t_accent }
        , frame_index{ t_frameIndex }
        , sprite{ t_texture, t_textureRect }
        , elapsed_time_sec{ 0.0f }
        , time_per_frame_sec{ 0.0f }
    {
        const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 2.4f) };
        sprite.setScale({ scale, scale });

        if (isVine(which))
        {
            time_per_frame_sec = t_context.random.fromTo(0.35f, 0.8f);

            sprite.setPosition(
                { (util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
                  t_screenRect.position.y });
        }
        else
        {
            time_per_frame_sec = t_context.random.fromTo(0.08f, 0.18f);

            sprite.setPosition(
                { (util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
                  (util::bottom(t_screenRect) - sprite.getGlobalBounds().size.y) });
        }
    }

    //

    AccentAnimations::AccentAnimations()
        : m_textures{}
        , m_anims{}
    {
        m_anims.reserve(32); // just a harmless guess based on knowledge of maps
    }

    void AccentAnimations::setup(const Context & t_context)
    {
        m_textures.reserve(static_cast<std::size_t>(Accent::Count)); // prevent reallocations
        for (std::size_t accentIndex(0); accentIndex < static_cast<std::size_t>(Accent::Count);
             ++accentIndex)
        {
            const Accent accent{ static_cast<Accent>(accentIndex) };

            sf::Texture & texture{ m_textures.emplace_back() };

            util::TextureLoader::load(
                texture, (t_context.settings.media_path / "image/anim" / toFilename(accent)), true);
        }
    }

    void AccentAnimations::add(
        const Context & t_context, const sf::FloatRect & t_rect, const Accent t_accent)
    {
        m_anims.emplace_back(
            t_context,
            t_accent,
            m_textures.at(static_cast<std::size_t>(t_accent)),
            textureRect(t_accent, 0),
            t_rect,
            t_context.random.zeroToOneLessThan(frameCount(t_accent)));
    }

    std::size_t AccentAnimations::frameCount(const Accent t_which) const
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

    sf::IntRect AccentAnimations::textureRect(const Accent t_which, const std::size_t t_frame) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(t_which)) };

        sf::IntRect rect;
        rect.size.x     = static_cast<int>(texture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(t_frame) * rect.size.x);

        return rect;
    }

    void AccentAnimations::update(Context &, const float t_frameTimeSec)
    {
        for (AccentAnim & anim : m_anims)
        {
            anim.elapsed_time_sec += t_frameTimeSec;
            if (anim.elapsed_time_sec > anim.time_per_frame_sec)
            {
                anim.elapsed_time_sec -= anim.time_per_frame_sec;

                ++anim.frame_index;
                if (anim.frame_index >= frameCount(anim.which))
                {
                    anim.frame_index = 0;
                }

                anim.sprite.setTextureRect(textureRect(anim.which, anim.frame_index));
            }
        }
    }

    void AccentAnimations::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const AccentAnim & anim : m_anims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void AccentAnimations::move(const float t_amount)
    {
        for (AccentAnim & anim : m_anims)
        {
            anim.sprite.move({ t_amount, 0.0f });
        }
    }

} // namespace bramblefore
