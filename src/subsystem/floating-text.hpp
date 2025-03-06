#ifndef FLOATING_TEXT_HPP_INCLUDED
#define FLOATING_TEXT_HPP_INCLUDED
//
// floating-text.hpp
//
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sf
{
    class RenderTarget;
    struct RenderStates;
} // namespace sf

namespace bramblefore
{
    struct Context;

    //

    struct FloatingTextAnim
    {
        explicit FloatingTextAnim(const sf::Text & t_text, const sf::Vector2f & t_position);

        bool is_alive;
        sf::Text text;
    };

    //

    class FloatingText
    {
      public:
        FloatingText();

        void
            add(Context & t_context,
                const std::string & t_message,
                const sf::Color & t_color,
                const sf::Vector2f & t_pos);

        void clear() noexcept { m_animations.clear(); }
        void update(Context & t_context, const float t_frameTimeSec);

        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        void move(const float t_amount);

      private:
        std::vector<FloatingTextAnim> m_animations;
    };

} // namespace bramblefore

#endif // FLOATING_TEXT_HPP_INCLUDED