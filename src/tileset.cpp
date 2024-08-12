// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// tileset.cpp
//
#include "tileset.hpp"

#include "context.hpp"

#include <iostream>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    TileSet::TileSet()
        : m_tileCount(0, 0)
        , m_tileSize(0, 0)
        , m_layers()
    {
        m_layers.reserve(16);
    }

    void TileSet::reset()
    {
        m_layers.clear();
        m_tileCount = { 0, 0 };
        m_tileSize  = { 0, 0 };
    }

    void TileSet::setTileCountAndSize(const sf::Vector2i tileCount, const sf::Vector2i tileSize)
    {
        m_tileCount = tileCount;
        m_tileSize  = tileSize;
    }

    void TileSet::appendTileLayer(const TileImage image, const std::vector<int> & indexes)
    {
        m_layers.push_back(std::make_unique<TileLayer>(image, indexes));
    }

    void TileSet::move(const Context & context, const float move)
    {
        for (auto & layerUPtr : m_layers)
        {
            layerUPtr->moveVerts(context, move);
        }
    }

    float TileSet::findFarthestHorizMapPixel()
    {
        float result{ 0.0f };

        for (const auto & layerUPtr : m_layers)
        {
            const float farthestHorizPos{ layerUPtr->findFarthestHorizVert() };
            if (farthestHorizPos > result)
            {
                result = farthestHorizPos;
            }
        }

        return result;
    }

    void TileSet::dumpInfo() const
    {
        std::cout << "Level Graphics Info\n";

        for (const auto & layerUPtr : m_layers)
        {
            layerUPtr->dumpInfo();
        }

        std::cout << std::endl;
    }

    void TileSet::appendVertLayers(
        const Context & context,
        const sf::Vector2f & mapPosOffset,
        const sf::Vector2f & tileScreenSize)
    {
        for (auto & layerUPtr : m_layers)
        {
            layerUPtr->appendVertLayer(
                context, mapPosOffset, m_tileCount, m_tileSize, tileScreenSize);
        }
    }

    void TileSet::draw(
        const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (auto & layerUPtr : m_layers)
        {
            layerUPtr->draw(context, target, states);
        }
    }

} // namespace platformer
