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

namespace bramblefore
{

    class GlowRect
    {
      public:
        GlowRect();

        void setup(
            const sf::Color & t_color,
            const sf::FloatRect & t_outerRect,
            const float t_innerResizeRatio);

        void setup(
            const sf::Color & t_color,
            const sf::FloatRect & t_outerRect,
            const sf::Vector2f & t_innerResizeRatios);

        void setup(
            const sf::Color & t_color,
            const sf::FloatRect & t_outerRect,
            const sf::FloatRect & t_innerRect);

        void draw(sf::RenderTarget & t_target) const;

      private:
        std::vector<sf::Vertex> m_verts;
    };

} // namespace bramblefore

#endif // GLOW_RECT_HPP_INCLUDED
