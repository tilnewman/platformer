#ifndef GLOW_RECT_HPP_INCLUDED
#define GLOW_RECT_HPP_INCLUDED
//
// glow-rect.hpp
//
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
} // namespace sf

namespace platformer
{

    class GlowRect
    {
      public:
        GlowRect();

        void setup(
            const sf::Color & color, const sf::FloatRect & outerRect, const float innerResizeRatio);
        
        void setup(
            const sf::Color & color,
            const sf::FloatRect & outerRect,
            const sf::Vector2f & innerResizeRatios);
        
        void setup(
            const sf::Color & color,
            const sf::FloatRect & outerRect,
            const sf::FloatRect & innerRect);

        void draw(sf::RenderTarget & target, sf::RenderStates states) const;

      private:
        std::vector<sf::Vertex> m_verts;
    };

} // namespace platformer

#endif // GLOW_RECT_HPP_INCLUDED
