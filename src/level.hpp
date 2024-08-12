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
        bool move(const ScreenLayout & layout, const float move);

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

        void appendVertLayers(Context & context);

        void appendVertLayer(
            const sf::Vector2i & count,
            const sf::Vector2i & size,
            const sf::Vector2f & sizeOnScreen,
            const TileTexture & texture,
            ITileLayer & layer) const;

        void populateVisibleVerts(const ScreenLayout & layout);

        void dumpInfo() const;
    };

} // namespace platformer

#endif // LEVEL_HPP_INCLUDED
