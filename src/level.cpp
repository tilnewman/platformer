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
        : map_position_offset()
        , tile_size_screen()
        , tile_size_texture()
        , collisions()
        , enter_rect()
        , exit_rect()
        , farthest_horiz_map_pixel(0.0f)
        , farthest_horiz_traveled(0.0f)
        , name()
        , tile_count()
        , tile_size()
        , tile_layers()
    {
        tile_layers.reserve(32);
        collisions.reserve(1024);
    }

    void Level::reset()
    {
        tile_layers.clear();
        collisions.clear();
        farthest_horiz_map_pixel = 0.0f;
        farthest_horiz_traveled  = 0.0f;
    }

    bool Level::load(Context & context)
    {
        reset();

        if (context.level_loader.load(context))
        {
            appendVertLayers(context);
            context.avatar.setPosition(enter_rect);
            findFarthestHorizMapPixel();
            farthest_horiz_traveled = 0.0f;

            // dumpInfo();
            return true;
        }
        else
        {
            reset();
            return false;
        }
    }

    bool Level::move(const Context & context, const float move)
    {
        // farthest_horiz_traveled += std::abs(move);
        // if (farthest_horiz_traveled > (farthest_horiz_map_pixel - context.layout.wholeSize().x))
        //{
        //    return false;
        //}

        enter_rect.left += move;
        exit_rect.left += move;

        for (sf::FloatRect & rect : collisions)
        {
            rect.left += move;
        }

        for (auto & layerUPtr : tile_layers)
        {
            layerUPtr->move(context, move);
        }

        return true;
    }

    void Level::findFarthestHorizMapPixel()
    {
        farthest_horiz_map_pixel = 0.0f;

        for (const auto & layerUPtr : tile_layers)
        {
            const float temp{ layerUPtr->findFarthestHorizVert() };
            if (temp > farthest_horiz_map_pixel)
            {
                farthest_horiz_map_pixel = temp;
            }
        }
    }

    void Level::appendVertLayers(const Context & context)
    {
        for (auto & layerUPtr : tile_layers)
        {
            layerUPtr->appendVertLayer(
                context, map_position_offset, tile_count, tile_size, tile_size_screen);
        }
    }

    void Level::dumpInfo() const
    {
        std::cout << "Level Graphics Info\n";

        for (const auto & layerUPtr : tile_layers)
        {
            layerUPtr->dumpInfo();
        }

        std::cout << std::endl;
    }

    void Level::draw(
        const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (auto & layerUPtr : tile_layers)
        {
            layerUPtr->draw(context, target, states);
        }
    }

    void Level::update(Context & context, const float frameTimeSec)
    {
        for (auto & layerUPtr : tile_layers)
        {
            layerUPtr->update(context, frameTimeSec);
        }
    }

} // namespace platformer
