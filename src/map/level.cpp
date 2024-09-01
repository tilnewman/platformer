// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// level.cpp
//
#include "map/level.hpp"

#include "avatar/avatar.hpp"
#include "avatar/spells-anim.hpp"
#include "bramblefore/settings.hpp"
#include "map-textures.hpp"
#include "map/accents.hpp"
#include "map/level-file-loader.hpp"
#include "map/pickups.hpp"
#include "subsystem/background-images.hpp"
#include "subsystem/context.hpp"
#include "subsystem/floating-text.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/sfml-util.hpp"

#include <iostream>

namespace bramblefore
{

    Level::Level()
        : map_position_offset{}
        , tile_size_screen{}
        , tile_size_texture{}
        , collisions{}
        , ladders{}
        , enter_rect{}
        , exit_rect{}
        , name{}
        , tile_count{}
        , tile_size{}
        , tile_layers{}
        , monsters{}
        , farthest_horiz_traveled{ 0.0f }
        , farthest_horiz_map_pixel{ 0.0f }
    {
        // harmless guesses based on what I know is in map files
        tile_layers.reserve(32);
        collisions.reserve(1024);
    }

    void Level::reset()
    {
        tile_layers.clear();
        collisions.clear();
        ladders.clear();
        monsters.clear();
        farthest_horiz_traveled  = 0.0f;
        farthest_horiz_map_pixel = 0.0f;
    }

    bool Level::load(Context & t_context)
    {
        reset();

        if (t_context.level_loader.load(t_context))
        {
            appendVertLayers(t_context);
            t_context.avatar.setPosition(enter_rect);
            farthest_horiz_map_pixel = findFarthestHorizMapPixel();
            // dumpInfo();
            return true;
        }
        else
        {
            reset();
            return false;
        }
    }

    bool Level::move(const Context & t_context, const float t_amount)
    {
        farthest_horiz_traveled += util::abs(t_amount);
        if (farthest_horiz_traveled > (farthest_horiz_map_pixel - t_context.layout.wholeSize().x))
        {
            return false;
        }

        enter_rect.left += t_amount;
        exit_rect.left += t_amount;

        for (sf::FloatRect & rect : collisions)
        {
            rect.left += t_amount;
        }

        for (sf::FloatRect & rect : ladders)
        {
            rect.left += t_amount;
        }

        for (auto & layerUPtr : tile_layers)
        {
            layerUPtr->move(t_context, t_amount);
        }

        monsters.move(t_amount);
        t_context.accent.move(t_amount);
        t_context.pickup.move(t_amount);
        t_context.pickup.move(t_amount);
        t_context.bg_image.move(t_amount);
        t_context.spell.move(t_amount);
        t_context.float_text.move(t_amount);

        return true;
    }

    float Level::findFarthestHorizMapPixel() const
    {
        float farthestHorizPos{ 0.0f };

        for (const auto & layerUPtr : tile_layers)
        {
            const float nextFarthest{ layerUPtr->findFarthestHorizVert() };
            if (nextFarthest > farthestHorizPos)
            {
                farthestHorizPos = nextFarthest;
            }
        }

        return farthestHorizPos;
    }

    void Level::appendVertLayers(const Context & t_context)
    {
        for (auto & layerUPtr : tile_layers)
        {
            layerUPtr->appendVertLayer(
                t_context, map_position_offset, tile_count, tile_size, tile_size_screen);
        }
    }

    void Level::dumpInfo() const
    {
        std::clog << "Map Graphics Layer Info:\n";

        for (const auto & layerUPtr : tile_layers)
        {
            layerUPtr->dumpInfo();
        }

        std::clog << '\n';
    }

    void Level::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (auto & layerUPtr : tile_layers)
        {
            layerUPtr->draw(t_context, t_target, t_states);
        }

        monsters.draw(t_context, t_target, t_states);
    }

    void Level::update(Context & t_context, const float t_frameTimeSec)
    {
        for (auto & layerUPtr : tile_layers)
        {
            layerUPtr->update(t_context, t_frameTimeSec);
        }

        monsters.update(t_context, t_frameTimeSec);
    }

    std::optional<sf::FloatRect> Level::ladderCollisionRect(const sf::FloatRect & avatarRect) const
    {
        for (const sf::FloatRect & ladderRect : ladders)
        {
            if (avatarRect.intersects(ladderRect))
            {
                return { ladderRect };
            }
        }

        return {};
    }

} // namespace bramblefore
