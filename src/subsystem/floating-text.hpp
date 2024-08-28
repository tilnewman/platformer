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
    class RenderStates;
} // namespace sf

namespace platformer
{
    struct Context;

    //

    struct FloatingTextAnim
    {
        bool is_alive{ true };
        sf::Text text{};
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

} // namespace platformer

#endif // FLOATING_TEXT_HPP_INCLUDED