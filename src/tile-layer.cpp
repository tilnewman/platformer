// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// tile-layer.cpp
//
#include "tile-layer.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "map-textures.hpp"
#include "screen-layout.hpp"
#include "sfml-util.hpp"

#include <iostream>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
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
        t_states.texture = &MapTextureManager::instance().get(m_image).texture;
        t_target.draw(&m_visibleVerts[0], m_visibleVerts.size(), sf::Quads, t_states);
    }

    void TileLayer::move(const Context & t_context, const float t_move)
    {
        for (sf::Vertex & vertex : m_verts)
        {
            vertex.position.x += t_move;
        }

        populateVisibleVerts(t_context.layout.wholeRect());
    }

    float TileLayer::findFarthestHorizVert() const
    {
        float farthestHorizPos{ 0.0f };
        for (const sf::Vertex & vertex : m_verts)
        {
            if (vertex.position.x > farthestHorizPos)
            {
                farthestHorizPos = vertex.position.x;
            }
        }

        return farthestHorizPos;
    }

    void TileLayer::dumpInfo() const
    {
        std::cout << "\tLayer Tiles:  " << m_image << ", possible=" << m_indexes.size()
                  << ", actual=" << (m_verts.size() / util::verts_per_quad)
                  << ", visible=" << (m_visibleVerts.size() / util::verts_per_quad) << "\n";
    }

    void TileLayer::populateVisibleVerts(const sf::FloatRect & t_visibleRect)
    {
        m_visibleVerts.clear();

        std::size_t vertIndex = 0;
        while (vertIndex < m_verts.size())
        {
            const sf::Vertex topLeftVert  = m_verts[vertIndex++];
            const sf::Vertex topRightVert = m_verts[vertIndex++];
            const sf::Vertex botRightVert = m_verts[vertIndex++];
            const sf::Vertex botLeftVert  = m_verts[vertIndex++];

            if (t_visibleRect.contains(topLeftVert.position) ||
                t_visibleRect.contains(topRightVert.position) ||
                t_visibleRect.contains(botRightVert.position) ||
                t_visibleRect.contains(botLeftVert.position))
            {
                m_visibleVerts.push_back(topLeftVert);
                m_visibleVerts.push_back(topRightVert);
                m_visibleVerts.push_back(botRightVert);
                m_visibleVerts.push_back(botLeftVert);
            }
        }
    }

    void TileLayer::appendVertLayer(
        const Context & t_context,
        const sf::Vector2f & t_mapPositionOffset,
        const sf::Vector2i & t_count,
        const sf::Vector2i & t_size,
        const sf::Vector2f & t_sizeOnScreen)
    {
        const TileTexture & tileTexture = MapTextureManager::instance().get(m_image);

        const sf::Vector2i textureTileCount{ tileTexture.size / t_size };

        const std::size_t totalCount =
            (static_cast<std::size_t>(t_count.x) * static_cast<std::size_t>(t_count.y));

        M_CHECK(
            (totalCount == m_indexes.size()),
            "index_count=" << m_indexes.size() << " does not equal tile_count=" << totalCount);

        const sf::Vector2i sizeOnScreenI(t_sizeOnScreen);

        std::size_t textureIndex = 0;
        for (int y(0); y < t_count.y; ++y)
        {
            const float posY = static_cast<float>(y * sizeOnScreenI.y);

            for (int x(0); x < t_count.x; ++x)
            {
                const float posX = static_cast<float>(x * sizeOnScreenI.x);

                const int textureIndexOrig(m_indexes[textureIndex++]);
                if (textureIndexOrig == 0)
                {
                    continue; // zero means no image at this location
                }

                const int index(textureIndexOrig - tileTexture.gid);

                const int texturePosX((index % textureTileCount.x) * t_size.x);
                const int texturePosY((index / textureTileCount.x) * t_size.y);

                const sf::Vector2i texturePos{ texturePosX, texturePosY };
                const sf::IntRect textureRect{ texturePos, t_size };

                const sf::Vector2f screenPos(sf::Vector2f(posX, posY) + t_mapPositionOffset);
                const sf::FloatRect screenRect{ screenPos, t_sizeOnScreen };

                util::appendQuadVerts(screenRect, textureRect, m_verts);
            }
        }

        populateVisibleVerts(t_context.layout.wholeRect());
    }

} // namespace platformer
