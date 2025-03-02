// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// tile-layer.cpp
//
#include "map/tile-layer.hpp"

#include "map-textures.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-util.hpp"

#include <iostream>

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    TileLayer::TileLayer(
        const Context & t_context, const TileImage t_image, const std::vector<int> & t_indexes)
        : m_image{ t_image }
        , m_indexes{ t_indexes }
        , m_verts{}
        , m_visibleVerts{}
    {
        MapTextureManager::instance().acquire(t_context, m_image);
        m_verts.reserve(t_indexes.size() * util::verts_per_quad);
        m_visibleVerts.reserve(t_indexes.size() * util::verts_per_quad);
    }

    TileLayer::~TileLayer() { MapTextureManager::instance().release(m_image); }

    void TileLayer::draw(
        const Context &, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        if (!m_visibleVerts.empty())
        {
            t_states.texture = &MapTextureManager::instance().get(m_image).texture;

            t_target.draw(
                &m_visibleVerts[0], m_visibleVerts.size(), sf::PrimitiveType::Triangles, t_states);

            t_states.texture = nullptr;
        }
    }

    void TileLayer::move(const Context & t_context, const float t_move)
    {
        for (sf::Vertex & vertex : m_verts)
        {
            vertex.position.x += t_move;
        }

        populateVisibleVerts(t_context.layout.wholeRect());
    }

    void TileLayer::dumpInfo() const
    {
        std::clog << "\tLayer Tiles:  " << m_image << ", possible=" << m_indexes.size()
                  << ", actual=" << (m_verts.size() / util::verts_per_quad)
                  << ", visible=" << (m_visibleVerts.size() / util::verts_per_quad) << '\n';
    }

    void TileLayer::populateVisibleVerts(const sf::FloatRect & t_visibleRect)
    {
        m_visibleVerts.clear();

        std::size_t vertIndex{ 0 };
        while (vertIndex < m_verts.size())
        {
            // see sfml-util::setupTriangleVerts() for where this mapping comes from
            const sf::Vector2f & topLeftVertPos{ m_verts[vertIndex + 0].position };
            const sf::Vector2f & topRightVertPos{ m_verts[vertIndex + 1].position };
            const sf::Vector2f & botLeftVertPos{ m_verts[vertIndex + 2].position };
            const sf::Vector2f & botRightVertPos{ m_verts[vertIndex + 5].position };

            if (t_visibleRect.contains(topLeftVertPos) || t_visibleRect.contains(topRightVertPos) ||
                t_visibleRect.contains(botRightVertPos) || t_visibleRect.contains(botLeftVertPos))
            {
                for (std::size_t i{ 0 }; i < util::verts_per_quad; ++i)
                {
                    m_visibleVerts.push_back(m_verts[vertIndex + i]);
                }
            }

            vertIndex += util::verts_per_quad;
        }
    }

    void TileLayer::appendVertLayer(
        const Context & t_context,
        const sf::Vector2f & t_mapPositionOffset,
        const sf::Vector2i & t_count,
        const sf::Vector2i & t_size,
        const sf::Vector2f & t_sizeOnScreen)
    {
        const TileTexture & tileTexture{ MapTextureManager::instance().get(m_image) };

        const sf::Vector2i textureTileCount{ tileTexture.size / t_size };

        const std::size_t totalCount{ static_cast<std::size_t>(t_count.x) *
                                      static_cast<std::size_t>(t_count.y) };

        M_CHECK(
            (totalCount == m_indexes.size()),
            "index_count=" << m_indexes.size() << " does not equal tile_count=" << totalCount);

        const sf::Vector2i sizeOnScreenI{ t_sizeOnScreen };

        std::size_t textureIndex{ 0 };
        for (int y{ 0 }; y < t_count.y; ++y)
        {
            const float posY = static_cast<float>(y * sizeOnScreenI.y);

            for (int x{ 0 }; x < t_count.x; ++x)
            {
                const int textureIndexOrig{ m_indexes[textureIndex++] };
                if (textureIndexOrig == 0)
                {
                    continue; // zero means no tile image at this location
                }

                const float posX{ static_cast<float>(x * sizeOnScreenI.x) };

                const int index{ textureIndexOrig - tileTexture.gid };

                const int texturePosX{ (index % textureTileCount.x) * t_size.x };
                const int texturePosY{ (index / textureTileCount.x) * t_size.y };

                const sf::IntRect textureRect{ { texturePosX, texturePosY }, t_size };

                const sf::Vector2f screenPos{ sf::Vector2f(posX, posY) + t_mapPositionOffset };
                const sf::FloatRect screenRect{ screenPos, t_sizeOnScreen };

                util::appendTriangleVerts(screenRect, textureRect, m_verts);
            }
        }

        populateVisibleVerts(t_context.layout.wholeRect());
    }

} // namespace bramblefore
