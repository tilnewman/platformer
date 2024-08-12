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

    bool Level::move(const Context & context, const float move)
    {
        farthest_horiz_traveled += std::abs(move);
        if (farthest_horiz_traveled > (farthest_horiz_map_pixel - context.layout.wholeSize().x))
        {
            return false;
        }

        enter_rect.left += move;
        exit_rect.left += move;

        for (sf::FloatRect & rect : collisions)
        {
            rect.left += move;
        }

        tiles.move(context, move);
        return true;
    }

    void Level::findFarthestHorizMapPixel()
    {
        farthest_horiz_map_pixel = tiles.findFarthestHorizMapPixel();
    }

    void Level::appendVertLayers(const Context & context)
    {
        tiles.appendVertLayers(context, map_position_offset, tile_size_screen);
    }

    void Level::dumpInfo() const { tiles.dumpInfo(); }

} // namespace platformer
