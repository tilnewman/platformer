#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED
//
// level.hpp
//
#include "map/tile-layer.hpp"
#include "monster/monster-manager.hpp"

#include <optional>
#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{
    struct Context;
    struct TileTexture;
    class ScreenLayout;

    //

    struct Level
    {
        Level();

        void load(Context & t_context, const std::string & t_filename);
        void reset();

        // returns true if the map was moved
        bool move(const Context & t_context, const float t_move);

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const;

        void update(Context & t_context, const float t_frameTimeSec);

        std::optional<sf::FloatRect> ladderCollisionRect(const sf::FloatRect & avatarRect) const;

        sf::Vector2f map_position_offset;
        sf::Vector2f tile_size_screen;
        sf::Vector2f tile_size_texture;
        std::vector<sf::FloatRect> collisions;
        std::vector<sf::FloatRect> kill_collisions;
        std::vector<sf::FloatRect> layer_collisions;
        std::vector<sf::FloatRect> ladders;
        sf::FloatRect enter_rect;
        sf::FloatRect exit_rect;
        std::string name;
        sf::Vector2i tile_count;
        sf::Vector2i tile_size;
        std::vector<std::unique_ptr<ITileLayer>> tile_layers;
        MonsterManager monsters;
        float farthest_horiz_traveled;
        float farthest_horiz_map_pixel;

      private:
        void appendVertLayers(const Context & t_context);
        void dumpInfo(const std::string & t_filename) const;
    };

} // namespace bramblefore

#endif // LEVEL_HPP_INCLUDED
