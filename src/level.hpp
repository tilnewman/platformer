#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED
//
// level.hpp
//
#include "tileset.hpp"

#include <vector>

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{
    struct Context;
    struct TileTexture;
    class ScreenLayout;

    //
    struct Level
    {
        Level();

        bool load(Context & context);
        void reset();
        bool move(const Context & context, const float move);

        TileSet tiles;
        sf::Vector2f map_position_offset;
        sf::Vector2f tile_size_screen;
        sf::Vector2f tile_size_texture;
        std::vector<sf::FloatRect> collisions;
        sf::FloatRect enter_rect;
        sf::FloatRect exit_rect;
        float farthest_horiz_map_pixel;
        float farthest_horiz_traveled;
        std::string name;

      private:
        void findFarthestHorizMapPixel();
        void appendVertLayers(const Context & context);
        void dumpInfo() const;
    };

} // namespace platformer

#endif // LEVEL_HPP_INCLUDED
