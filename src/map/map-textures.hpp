#ifndef MAPTEXTURES_HPP_INCLUDED
#define MAPTEXTURES_HPP_INCLUDED
//
// map-textures.hpp
//
#include "map/tile-image.hpp"

#include <vector>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace platformer
{
    struct Context;
    struct Settings;

    //

    struct TileTexture
    {
        std::size_t ref_count{ 0 };
        int gid{ 0 };
        sf::Vector2i size{};
        sf::Texture texture{};
    };

    //

    class MapTextureManager
    {
      public:
        MapTextureManager();

        static MapTextureManager & instance();

        void acquire(const Context & t_context, const TileImage t_image);
        void release(const TileImage t_image);

        [[nodiscard]] const TileTexture & get(const TileImage t_image) const;
        void setGid(const TileImage t_image, const int t_gid);

      private:
        std::vector<TileTexture> m_tileTextures;
    };

} // namespace platformer

#endif // MAPTEXTURES_HPP_INCLUDED
