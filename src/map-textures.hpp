#ifndef MAPTEXTURES_HPP_INCLUDED
#define MAPTEXTURES_HPP_INCLUDED
//
// map-textures.hpp
//
#include "tileset.hpp"

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
        TileTexture forest_ground;
        TileTexture forest_tree;
    };

} // namespace platformer

#endif // MAPTEXTURES_HPP_INCLUDED
