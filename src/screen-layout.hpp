#ifndef SCREEN_LAYOUT_HPP_INCLUDED
#define SCREEN_LAYOUT_HPP_INCLUDED
//
// screen-layout.hpp
//
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace platformer
{

    class ScreenLayout
    {
      public:
        ScreenLayout();

        void setup(const sf::Vector2u & windowSize);

        inline const sf::Vector2f wholeSize() const { return m_wholeSize; }
        inline const sf::FloatRect wholeRect() const { return m_wholeRect; }

      private:
        sf::Vector2f m_wholeSize;
        sf::FloatRect m_wholeRect;
    };

} // namespace platformer

#endif // SCREEN_LAYOUT_HPP_INCLUDED
