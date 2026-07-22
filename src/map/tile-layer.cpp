// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// tile-layer.cpp
//
#include "map/tile-layer.hpp"

#include "level.hpp"
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
        , m_imageTileCounts{ 0, 0 }
        , m_indexes{ t_indexes }
        , m_indexTiles{}
        , m_verts{}
        , m_visibleVerts{}
    {
        MapTextureManager::instance().acquire(t_context, m_image);

        // calculate how many tiles horiz and vert there are in this layer's image
        const sf::Vector2i mapTextureSizePixels{ MapTextureManager::instance().get(m_image).size };
        m_imageTileCounts = (mapTextureSizePixels / t_context.level.tileSize());

        // sanity check that the map/level total tile count matches this layers index count
        const std::size_t totalTileCount{ static_cast<std::size_t>(t_context.level.tileCount().x) *
                                          static_cast<std::size_t>(t_context.level.tileCount().y) };

        M_CHECK(
            (totalTileCount == m_indexes.size()),
            "index_count=" << m_indexes.size()
                           << " does not equal total_tile_count=" << totalTileCount);

        // reserve the total possible for now
        m_verts.reserve(t_indexes.size() * util::verts_per_quad);
        m_visibleVerts.reserve(t_indexes.size() * util::verts_per_quad);

        // pre-skip empty tiles in the map and pre-compute remaining tile texture rects
        setupOptimizedTileIndexes(t_context.level.tileCount(), t_context.level.tileSize());
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

    void TileLayer::move(const Context & t_context, const sf::Vector2f & t_move)
    {
        for (sf::Vertex & vertex : m_verts)
        {
            vertex.position += t_move;
        }

        populateVisibleVerts(t_context.layout.wholeRect());
    }

    void TileLayer::dumpInfo() const
    {
        std::clog << "\tTilLayer Quads for " << toString(m_image)
                  << ": possible=" << m_indexes.size() << ", optimized=" << m_indexTiles.size()
                  << ", actual=" << (m_verts.size() / util::verts_per_quad)
                  << ", visible=" << (m_visibleVerts.size() / util::verts_per_quad) << '\n';
    }

    void TileLayer::populateVisibleVerts(const sf::FloatRect & t_visibleRect)
    {
        m_visibleVerts.clear();

        if (m_verts.size() < util::verts_per_quad)
        {
            return;
        }

        std::size_t vertIndex{ 0 };
        while (vertIndex < m_verts.size())
        {
            // see sfml-util::setupTriangleVerts() for where this mapping comes from
            const sf::Vertex & topLeftVert{ m_verts[vertIndex + 0] };
            const sf::Vertex & topRightVert{ m_verts[vertIndex + 1] };
            const sf::Vertex & botLeftVert{ m_verts[vertIndex + 2] };
            const sf::Vertex & botLeftVert2{ m_verts[vertIndex + 3] };
            const sf::Vertex & topRightVert2{ m_verts[vertIndex + 4] };
            const sf::Vertex & botRightVert{ m_verts[vertIndex + 5] };

            if (t_visibleRect.contains(topLeftVert.position) ||
                t_visibleRect.contains(topRightVert.position) ||
                t_visibleRect.contains(botRightVert.position) ||
                t_visibleRect.contains(botLeftVert.position))
            {
                m_visibleVerts.push_back(topLeftVert);
                m_visibleVerts.push_back(topRightVert);
                m_visibleVerts.push_back(botLeftVert);
                m_visibleVerts.push_back(botLeftVert2);
                m_visibleVerts.push_back(topRightVert2);
                m_visibleVerts.push_back(botRightVert);
            }

            vertIndex += util::verts_per_quad;
        }
    }

    void TileLayer::appendVerts(
        const Context & t_context,
        const sf::Vector2f & t_mapOnScreenPosOffset,
        const sf::Vector2f & t_tileSizeOnScreen)
    {
        for (const IndexedTile & indexTile : m_indexTiles)
        {
            const float posX{ static_cast<float>(indexTile.position.x) * t_tileSizeOnScreen.x };
            const float posY{ static_cast<float>(indexTile.position.y) * t_tileSizeOnScreen.y };
            const sf::Vector2f screenPos{ sf::Vector2f(posX, posY) + t_mapOnScreenPosOffset };
            const sf::FloatRect screenRect{ screenPos, t_tileSizeOnScreen };

            util::appendTriangleVerts(screenRect, indexTile.texture_rect, m_verts);
        }

        populateVisibleVerts(t_context.layout.wholeRect());
    }

    void TileLayer::setupOptimizedTileIndexes(
        const sf::Vector2i & t_mapTileCount, const sf::Vector2i & t_tileSize)
    {
        const int tileTextureGid{ MapTextureManager::instance().get(m_image).gid };

        std::size_t tileIndex{ 0 };
        for (int y{ 0 }; y < t_mapTileCount.y; ++y)
        {
            for (int x{ 0 }; x < t_mapTileCount.x; ++x)
            {
                const int tileNumberRaw{ m_indexes[tileIndex++] };
                if (tileNumberRaw == 0)
                {
                    continue; // zero means no tile image at this location
                }

                const int tileNumber{ tileNumberRaw - tileTextureGid };
                const int texturePosX{ (tileNumber % m_imageTileCounts.x) * t_tileSize.x };
                const int texturePosY{ (tileNumber / m_imageTileCounts.x) * t_tileSize.y };
                const sf::IntRect textureRect{ { texturePosX, texturePosY }, t_tileSize };

                m_indexTiles.emplace_back(sf::Vector2i(x, y), textureRect);
            }
        }
    }

} // namespace bramblefore
