// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// tileset.cpp
//
#include "tileset.hpp"

#include "context.hpp"
#include "map-textures.hpp"
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

    void TileLayer::moveVerts(const float move)
    {
        for (sf::Vertex & vertex : m_verts)
        {
            vertex.position.x += move;
        }
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

    //

    TileSet::TileSet()
        : count(0, 0)
        , size(0, 0)
        , layers()
    {
        layers.reserve(16);
    }

    void TileSet::reset()
    {
        layers.clear();
        count = { 0, 0 };
        size  = { 0, 0 };
    }

} // namespace platformer
