// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// floating-text.hpp
//
#include "subsystem/floating-text.hpp"

#include "subsystem/context.hpp"
#include "subsystem/font.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    FloatingTextAnim::FloatingTextAnim(const sf::Text & t_text, const sf::Vector2f & t_position)
        : is_alive{ true }
        , text{ t_text }
    {
        text.setPosition({ (t_position.x - (text.getGlobalBounds().size.x * 0.5f)),
                           (t_position.y - (text.getGlobalBounds().size.y * 0.5f)) });
    }

    //

    FloatingText::FloatingText()
        : m_animations()
    {
        // just a harmless guess based on knowledge of maps and play
        m_animations.reserve(16);
    }

    void FloatingText::add(
        Context & t_context,
        const std::string & t_message,
        const sf::Color & t_color,
        const sf::Vector2f & t_pos)
    {
        m_animations.emplace_back(
            t_context.font.makeText(Font::Title, FontSize::Medium, t_message, t_color), t_pos);
    }

    void FloatingText::update(Context &, const float t_frameTimeSec)
    {
        bool areAnyAnimationsFinished{ false };
        for (FloatingTextAnim & anim : m_animations)
        {
            anim.text.move({ 0.0f, -(t_frameTimeSec * 80.0f) });

            std::uint8_t alpha{ anim.text.getFillColor().a };
            if (alpha < 4)
            {
                anim.is_alive            = false;
                areAnyAnimationsFinished = true;
            }
            else
            {
                alpha -= 2;
            }

            sf::Color color{ anim.text.getFillColor() };
            color.a = alpha;
            anim.text.setFillColor(color);
        }

        if (areAnyAnimationsFinished)
        {
            std::erase_if(
                m_animations, [](const FloatingTextAnim & anim) { return !anim.is_alive; });
        }
    }

    void FloatingText::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const FloatingTextAnim & anim : m_animations)
        {
            // these finish animating very quickly so assume always on screen
            t_target.draw(anim.text, t_states);
        }
    }

    void FloatingText::move(const float t_amount)
    {
        for (FloatingTextAnim & anim : m_animations)
        {
            anim.text.move({ t_amount, 0.0f });
        }
    }

} // namespace bramblefore
