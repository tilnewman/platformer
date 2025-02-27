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
    {
        m_verts.reserve(util::verts_per_quad);
    }

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
        // see sfml-util::setupTriangleVerts() for where these offsets come from

        const auto setTransparentTopLeft = [&]() {
            m_verts.at(m_verts.size() - 6).color = sf::Color::Transparent;
        };

        const auto setTransparentTopRight = [&]() {
            m_verts.at(m_verts.size() - 2).color = sf::Color::Transparent;
            m_verts.at(m_verts.size() - 5).color = sf::Color::Transparent;
        };

        const auto setTransparentBotRight = [&]() {
            m_verts.at(m_verts.size() - 1).color = sf::Color::Transparent;
        };

        const auto setTransparentBotLeft = [&]() {
            m_verts.at(m_verts.size() - 3).color = sf::Color::Transparent;
            m_verts.at(m_verts.size() - 4).color = sf::Color::Transparent;
        };

        m_verts.clear();

        util::appendTriangleVerts(t_innerRect, m_verts, t_color);

        const sf::FloatRect topLeftRect{ { t_outerRect.position.x, t_outerRect.position.y },
                                         { (t_innerRect.position.x - t_outerRect.position.x),
                                           (t_innerRect.position.y - t_outerRect.position.y) } };

        util::appendTriangleVerts(topLeftRect, m_verts, t_color);
        setTransparentBotLeft();
        setTransparentTopLeft();
        setTransparentTopRight();

        const sf::FloatRect topRect{ { t_innerRect.position.x, t_outerRect.position.y },
                                     { t_innerRect.size.x, topLeftRect.size.y } };

        util::appendTriangleVerts(topRect, m_verts, t_color);
        setTransparentTopLeft();
        setTransparentTopRight();

        const sf::FloatRect topRightRect{ { util::right(topRect), topRect.position.y },
                                          { topLeftRect.size.x, topLeftRect.size.y } };

        util::appendTriangleVerts(topRightRect, m_verts, t_color);
        setTransparentTopLeft();
        setTransparentTopRight();
        setTransparentBotRight();

        const sf::FloatRect leftRect{ { t_outerRect.position.x, t_innerRect.position.y },
                                      { topLeftRect.size.x, t_innerRect.size.y } };

        util::appendTriangleVerts(leftRect, m_verts, t_color);
        setTransparentTopLeft();
        setTransparentBotLeft();

        const sf::FloatRect rightRect{ { util::right(t_innerRect), t_innerRect.position.y },
                                       { leftRect.size.x, t_innerRect.size.y }

        };

        util::appendTriangleVerts(rightRect, m_verts, t_color);
        setTransparentTopRight();
        setTransparentBotRight();

        const sf::FloatRect botLeftRect{ { t_outerRect.position.x, util::bottom(t_innerRect) },
                                         { leftRect.size.x, topLeftRect.size.y } };

        util::appendTriangleVerts(botLeftRect, m_verts, t_color);
        setTransparentTopLeft();
        setTransparentBotLeft();
        setTransparentBotRight();

        const sf::FloatRect botRect{ { t_innerRect.position.x, util::bottom(t_innerRect) },
                                     { t_innerRect.size.x, topLeftRect.size.y } };

        util::appendTriangleVerts(botRect, m_verts, t_color);
        setTransparentBotLeft();
        setTransparentBotRight();

        const sf::FloatRect botRightRect{ { util::right(t_innerRect), util::bottom(t_innerRect) },
                                          { topRightRect.size.x, topRightRect.size.y } };

        util::appendTriangleVerts(botRightRect, m_verts, t_color);
        setTransparentBotLeft();
        setTransparentBotRight();
        setTransparentTopRight();
    }

    void GlowRect::draw(sf::RenderTarget & t_target) const
    {
        if (!m_verts.empty())
        {
            t_target.draw(&m_verts[0], m_verts.size(), sf::PrimitiveType::Triangles);
        }
    }

} // namespace bramblefore
