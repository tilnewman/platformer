// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// accents.cpp
//
#include "map/accents.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    AccentAnimations::AccentAnimations()
        : m_textures{}
        , m_anims{}
        , m_scale{}
    {
        m_anims.reserve(32); // just a harmless guess based on knowledge of maps
    }

    void AccentAnimations::setup(const Context & t_context)
    {
        const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 2.4f) };
        m_scale.x = scale;
        m_scale.y = scale;

        // prevent reallocations
        m_textures.reserve(static_cast<std::size_t>(Accent::Count));

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
        const Context & t_context, const sf::FloatRect & t_rect, const std::string & t_name)
    {
        const Accent accent{ stringToAccent(t_name) };
        M_CHECK((Accent::Count != accent), "\"" << t_name << "\" is an unknown Accent name");

        AccentAnim & anim{ m_anims.emplace_back() };
        anim.which      = accent;
        anim.anim_index = t_context.random.zeroToOneLessThan(frameCount(anim.which));
        anim.sprite.setTexture(m_textures.at(static_cast<std::size_t>(accent)), true);
        anim.sprite.setTextureRect(textureRect(accent, 0));
        anim.sprite.setScale(m_scale);

        if (isVine(accent))
        {
            anim.time_per_frame_sec = t_context.random.fromTo(0.35f, 0.8f);

            anim.sprite.setPosition(
                { (util::center(t_rect).x - (anim.sprite.getGlobalBounds().size.x * 0.5f)),
                  util::center(t_rect).y });
        }
        else
        {
            anim.time_per_frame_sec = t_context.random.fromTo(0.08f, 0.18f);

            anim.sprite.setPosition(
                { (util::center(t_rect).x - (anim.sprite.getGlobalBounds().size.x * 0.5f)),
                  (util::bottom(t_rect) - anim.sprite.getGlobalBounds().size.y) });
        }
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
            if (t_context.layout.wholeRect().findIntersection(anim.sprite.getGlobalBounds()))
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
