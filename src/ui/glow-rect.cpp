// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// glow-rect.hpp
//
#include "ui/glow-rect.hpp"

#include "util/sfml-util.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    GlowRect::GlowRect()
        : m_verts{}
    {}

    void GlowRect::setup(
        const sf::Color & t_color,
        const sf::FloatRect & t_outerRect,
        const float t_innerResizeRatio)
    {
        setup(t_color, t_outerRect, util::scaleRectInPlaceCopy(t_outerRect, t_innerResizeRatio));
    }

    void GlowRect::setup(
        const sf::Color & t_color,
        const sf::FloatRect & t_outerRect,
        const sf::Vector2f & t_innerResizeRatios)
    {
        setup(t_color, t_outerRect, util::scaleRectInPlaceCopy(t_outerRect, t_innerResizeRatios));
    }

    void GlowRect::setup(
        const sf::Color & t_color,
        const sf::FloatRect & t_outerRect,
        const sf::FloatRect & t_innerRect)
    {
        const auto setTransparentTopLeft = [&]() {
            m_verts.at(m_verts.size() - 4).color = sf::Color::Transparent;
        };

        const auto setTransparentTopRight = [&]() {
            m_verts.at(m_verts.size() - 3).color = sf::Color::Transparent;
        };

        const auto setTransparentBotRight = [&]() {
            m_verts.at(m_verts.size() - 2).color = sf::Color::Transparent;
        };

        const auto setTransparentBotLeft = [&]() {
            m_verts.at(m_verts.size() - 1).color = sf::Color::Transparent;
        };

        m_verts.clear();

        util::appendTriangleVerts(t_innerRect, m_verts, t_color);

        const sf::FloatRect topLeftRect{ t_outerRect.left,
                                         t_outerRect.top,
                                         (t_innerRect.left - t_outerRect.left),
                                         (t_innerRect.top - t_outerRect.top) };

        util::appendTriangleVerts(topLeftRect, m_verts, t_color);
        setTransparentBotLeft();
        setTransparentTopLeft();
        setTransparentTopRight();

        const sf::FloatRect topRect{
            t_innerRect.left, t_outerRect.top, t_innerRect.width, topLeftRect.height
        };

        util::appendTriangleVerts(topRect, m_verts, t_color);
        setTransparentTopLeft();
        setTransparentTopRight();

        const sf::FloatRect topRightRect{
            util::right(topRect), topRect.top, topLeftRect.width, topLeftRect.height
        };

        util::appendTriangleVerts(topRightRect, m_verts, t_color);
        setTransparentTopLeft();
        setTransparentTopRight();
        setTransparentBotRight();

        const sf::FloatRect leftRect{
            t_outerRect.left, t_innerRect.top, topLeftRect.width, t_innerRect.height
        };

        util::appendTriangleVerts(leftRect, m_verts, t_color);
        setTransparentTopLeft();
        setTransparentBotLeft();

        const sf::FloatRect rightRect{
            util::right(t_innerRect), t_innerRect.top, leftRect.width, t_innerRect.height

        };

        util::appendTriangleVerts(rightRect, m_verts, t_color);
        setTransparentTopRight();
        setTransparentBotRight();

        const sf::FloatRect botLeftRect{
            t_outerRect.left, util::bottom(t_innerRect), leftRect.width, topLeftRect.height
        };

        util::appendTriangleVerts(botLeftRect, m_verts, t_color);
        setTransparentTopLeft();
        setTransparentBotLeft();
        setTransparentBotRight();

        const sf::FloatRect botRect{
            t_innerRect.left, util::bottom(t_innerRect), t_innerRect.width, topLeftRect.height

        };

        util::appendTriangleVerts(botRect, m_verts, t_color);
        setTransparentBotLeft();
        setTransparentBotRight();

        const sf::FloatRect botRightRect{ util::right(t_innerRect),
                                          util::bottom(t_innerRect),
                                          topRightRect.width,
                                          topRightRect.height };

        util::appendTriangleVerts(botRightRect, m_verts, t_color);
        setTransparentBotLeft();
        setTransparentBotRight();
        setTransparentTopRight();
    }

    void GlowRect::draw(sf::RenderTarget &) const
    {
        // TODO fix and uncomment
        //  if (!m_verts.empty())
        //  {
        //      t_target.draw(&m_verts[0], m_verts.size(), sf::Quads);
        //  }
    }

} // namespace bramblefore
