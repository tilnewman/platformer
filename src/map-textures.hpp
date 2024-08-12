#ifndef MAPTEXTURES_HPP_INCLUDED
#define MAPTEXTURES_HPP_INCLUDED
//
// map-textures.hpp
//
#include "tile-image.hpp"

#include <string>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace platformer
{

    struct Settings;

    //

    struct TileTexture
    {
        TileTexture()
            : gid(0)
            , which(TileImage::ForestGround) // any default works here
            , size()
            , texture()
        {}

        int gid;
        TileImage which;
        sf::Vector2i size;
        sf::Texture texture;
    };

    //

    class MapTextures
    {
      public:
        MapTextures();

        void setup(const Settings & settings);
        TileTexture & get(const TileImage image);

      private:
        TileTexture m_castleGround;
        TileTexture m_castleObject;
        TileTexture m_dungeon1Ground;
        TileTexture m_dungeon2Ground;
        TileTexture m_dungeon2Object;
        TileTexture m_forestGround;
        TileTexture m_forestTrees;
        TileTexture m_mountainsGround;
        TileTexture m_mountainsObject;
    };

} // namespace platformer

#endif // MAPTEXTURES_HPP_INCLUDED
