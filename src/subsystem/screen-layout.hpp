#ifndef SCREEN_LAYOUT_HPP_INCLUDED
#define SCREEN_LAYOUT_HPP_INCLUDED
//
// screen-layout.hpp
//
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace bramblefore
{

    class ScreenLayout
    {
      public:
        ScreenLayout();

        void setup(const sf::Vector2u & t_windowSize);

        [[nodiscard]] inline sf::Vector2f wholeSize() const noexcept { return m_wholeSize; }
        [[nodiscard]] inline sf::FloatRect wholeRect() const noexcept { return m_wholeRect; }

      private:
        sf::Vector2f m_wholeSize;
        sf::FloatRect m_wholeRect;
    };

} // namespace bramblefore

#endif // SCREEN_LAYOUT_HPP_INCLUDED
