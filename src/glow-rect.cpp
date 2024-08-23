// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// glow-rect.hpp
//
#include "glow-rect.hpp"

#include "sfml-util.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    GlowRect::GlowRect()
        : m_verts{}
    {}

    void GlowRect::setup(
        const sf::Color & color, const sf::FloatRect & outerRect, const float innerResizeRatio)
    {
        setup(color, outerRect, util::scaleRectInPlaceCopy(outerRect, innerResizeRatio));
    }

    void GlowRect::setup(
        const sf::Color & color,
        const sf::FloatRect & outerRect,
        const sf::Vector2f & innerResizeRatios)
    {
        setup(color, outerRect, util::scaleRectInPlaceCopy(outerRect, innerResizeRatios));
    }

    void GlowRect::setup(
        const sf::Color & color, const sf::FloatRect & outerRect, const sf::FloatRect & innerRect)
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

        util::appendQuadVerts(innerRect, m_verts, color);

        const sf::FloatRect topLeftRect{ outerRect.left,
                                         outerRect.top,
                                         (innerRect.left - outerRect.left),
                                         (innerRect.top - outerRect.top) };

        util::appendQuadVerts(topLeftRect, m_verts, color);
        setTransparentBotLeft();
        setTransparentTopLeft();
        setTransparentTopRight();

        const sf::FloatRect topRect{
            innerRect.left, outerRect.top, innerRect.width, topLeftRect.height
        };

        util::appendQuadVerts(topRect, m_verts, color);
        setTransparentTopLeft();
        setTransparentTopRight();

        const sf::FloatRect topRightRect{
            util::right(topRect), topRect.top, topLeftRect.width, topLeftRect.height
        };

        util::appendQuadVerts(topRightRect, m_verts, color);
        setTransparentTopLeft();
        setTransparentTopRight();
        setTransparentBotRight();

        const sf::FloatRect leftRect{
            outerRect.left, innerRect.top, topLeftRect.width, innerRect.height
        };

        util::appendQuadVerts(leftRect, m_verts, color);
        setTransparentTopLeft();
        setTransparentBotLeft();

        const sf::FloatRect rightRect{
            util::right(innerRect), innerRect.top, leftRect.width, innerRect.height

        };

        util::appendQuadVerts(rightRect, m_verts, color);
        setTransparentTopRight();
        setTransparentBotRight();

        const sf::FloatRect botLeftRect{
            outerRect.left, util::bottom(innerRect), leftRect.width, topLeftRect.height
        };

        util::appendQuadVerts(botLeftRect, m_verts, color);
        setTransparentTopLeft();
        setTransparentBotLeft();
        setTransparentBotRight();

        const sf::FloatRect botRect{
            innerRect.left, util::bottom(innerRect), innerRect.width, topLeftRect.height

        };

        util::appendQuadVerts(botRect, m_verts, color);
        setTransparentBotLeft();
        setTransparentBotRight();

        const sf::FloatRect botRightRect{
            util::right(innerRect), util::bottom(innerRect), topRightRect.width, topRightRect.height
        };

        util::appendQuadVerts(botRightRect, m_verts, color);
        setTransparentBotLeft();
        setTransparentBotRight();
        setTransparentTopRight();
    }

    void GlowRect::draw(sf::RenderTarget & target, sf::RenderStates) const
    {
        target.draw(&m_verts[0], m_verts.size(), sf::Quads);
    }

} // namespace platformer
