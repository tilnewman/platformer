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

    TileLayer::TileLayer(const TileImage image, const std::vector<int> & indexes)
        : m_image(image)
        , m_indexes(indexes)
        , m_verts()
        , m_visibleVerts()
    {
        m_verts.reserve(indexes.size() * util::verts_per_quad);
        m_visibleVerts.reserve(indexes.size() * util::verts_per_quad);
    }

    void TileLayer::draw(
        const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        states.texture = &context.map_textures.get(m_image).texture;
        target.draw(&m_visibleVerts[0], m_visibleVerts.size(), sf::Quads, states);
    }

    void TileLayer::moveVerts(const Context & context, const float move)
    {
        for (sf::Vertex & vertex : m_verts)
        {
            vertex.position.x += move;
        }

        populateVisibleVerts(context.layout.wholeRect());
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

    void TileLayer::populateVisibleVerts(const sf::FloatRect & visibleRect)
    {
        m_visibleVerts.clear();

        std::size_t vertIndex = 0;
        while (vertIndex < m_verts.size())
        {
            const sf::Vertex topLeftVert  = m_verts[vertIndex++];
            const sf::Vertex topRightVert = m_verts[vertIndex++];
            const sf::Vertex botRightVert = m_verts[vertIndex++];
            const sf::Vertex botLeftVert  = m_verts[vertIndex++];

            if (visibleRect.contains(topLeftVert.position) ||
                visibleRect.contains(topRightVert.position) ||
                visibleRect.contains(botRightVert.position) ||
                visibleRect.contains(botLeftVert.position))
            {
                m_visibleVerts.push_back(topLeftVert);
                m_visibleVerts.push_back(topRightVert);
                m_visibleVerts.push_back(botRightVert);
                m_visibleVerts.push_back(botLeftVert);
            }
        }
    }

    void TileLayer::appendVertLayer(
        const Context & context,
        const sf::Vector2f & mapPositionOffset,
        const sf::Vector2i & count,
        const sf::Vector2i & size,
        const sf::Vector2f & sizeOnScreen)
    {
        const TileTexture & texture = context.map_textures.get(m_image);

        const sf::Vector2i textureTileCount{ texture.size / size };

        const std::size_t totalCount =
            (static_cast<std::size_t>(count.x) * static_cast<std::size_t>(count.y));

        M_CHECK(
            (totalCount == m_indexes.size()),
            "index_count=" << m_indexes.size() << " does not equal tile_count=" << totalCount);

        const sf::Vector2i sizeOnScreenI(sizeOnScreen);

        std::size_t textureIndex = 0;
        for (int y(0); y < count.y; ++y)
        {
            const float posY = static_cast<float>(y * sizeOnScreenI.y);

            for (int x(0); x < count.x; ++x)
            {
                const float posX = static_cast<float>(x * sizeOnScreenI.x);

                const int textureIndexOrig(m_indexes[textureIndex++]);
                if (textureIndexOrig == 0)
                {
                    continue; // zero means no image at this location
                }

                const int index(textureIndexOrig - texture.gid);

                const int texturePosX((index % textureTileCount.x) * size.x);
                const int texturePosY((index / textureTileCount.x) * size.y);

                const sf::Vector2i texturePos{ texturePosX, texturePosY };
                const sf::IntRect textureRect{ texturePos, size };

                const sf::Vector2f screenPos(sf::Vector2f(posX, posY) + mapPositionOffset);
                const sf::FloatRect screenRect{ screenPos, sizeOnScreen };

                util::appendQuadVerts(screenRect, textureRect, m_verts);
            }
        }

        populateVisibleVerts(context.layout.wholeRect());
    }

} // namespace platformer
