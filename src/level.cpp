// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// level.cpp
//
#include "level.hpp"

#include "avatar.hpp"
#include "check-macros.hpp"
#include "context.hpp"
#include "level-file-loader.hpp"
#include "map-textures.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"

#include <iostream>

namespace platformer
{

    Level::Level()
        : tiles()
        , map_position_offset()
        , tile_size_screen()
        , tile_size_texture()
        , collisions()
        , enter_rect()
        , exit_rect()
        , farthest_horiz_map_pixel(0.0f)
        , farthest_horiz_traveled(0.0f)
        , name()
    {
        collisions.reserve(1024);
    }

    void Level::reset()
    {
        tiles.reset();
        collisions.clear();
        farthest_horiz_map_pixel = 0.0f;
        farthest_horiz_traveled  = 0.0f;
    }

    bool Level::load(Context & context)
    {
        if (context.level_loader.load(context))
        {
            appendVertLayers(context);
            // context.avatar.setPosition(enter_rect); // TODO
            findFarthestHorizMapPixel();
            farthest_horiz_traveled = 0.0f;

            dumpInfo();
            return true;
        }
        else
        {
            reset();
            return false;
        }
    }

    bool Level::move(const ScreenLayout & layout, const float move)
    {
        farthest_horiz_traveled += std::abs(move);
        if (farthest_horiz_traveled > (farthest_horiz_map_pixel - layout.wholeSize().x))
        {
            return false;
        }

        enter_rect.left += move;
        exit_rect.left += move;

        for (sf::FloatRect & rect : collisions)
        {
            rect.left += move;
        }

        for (auto & layerUPtr : tiles.layers)
        {
            layerUPtr->moveVerts(move);
        }

        populateVisibleVerts(layout);

        return true;
    }

    void Level::findFarthestHorizMapPixel()
    {
        farthest_horiz_map_pixel = 0.0f;

        for (const auto & layerUPtr : tiles.layers)
        {
            const float farthestHorizPos{ layerUPtr->findFarthestHorizVert() };
            if (farthestHorizPos > farthest_horiz_map_pixel)
            {
                farthest_horiz_map_pixel = farthestHorizPos;
            }
        }
    }

    void Level::appendVertLayers(Context & context)
    {
        for (auto & layerUPtr : tiles.layers)
        {
            layerUPtr->appendVertLayer(
                context, map_position_offset, tiles.count, tiles.size, tile_size_screen);

            layerUPtr->populateVisibleVerts(context.layout.wholeRect());
        }
    }

    void Level::populateVisibleVerts(const ScreenLayout & layout)
    {
        for (auto & layerUPtr : tiles.layers)
        {
            layerUPtr->populateVisibleVerts(layout.wholeRect());
        }
    }

    void Level::dumpInfo() const
    {
        std::cout << "Level Graphics Info\n";

        for (const auto & layerUPtr : tiles.layers)
        {
            layerUPtr->dumpInfo();
        }

        std::cout << std::endl;
    }

} // namespace platformer
