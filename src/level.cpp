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
#include "monsters.hpp"
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
        monsters.clear();
    }

    bool Level::load(Context & context)
    {
        reset();

        if (context.level_loader.load(context))
        {
            appendVertLayers(context);
            context.avatar.setPosition(enter_rect);
            // dumpInfo();
            return true;
        }
        else
        {
            reset();
            return false;
        }
    }

    void Level::move(const Context & context, const float amount)
    {
        enter_rect.left += amount;
        exit_rect.left += amount;

        for (sf::FloatRect & rect : collisions)
        {
            rect.left += amount;
        }

        for (auto & layerUPtr : tile_layers)
        {
            layerUPtr->move(context, amount);
        }

        monsters.move(amount);
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

        monsters.draw(context, target, states);
    }

    void Level::update(Context & context, const float frameTimeSec)
    {
        for (auto & layerUPtr : tile_layers)
        {
            layerUPtr->update(context, frameTimeSec);
        }

        monsters.update(context, frameTimeSec);
    }

} // namespace platformer
