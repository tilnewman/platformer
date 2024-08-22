#ifndef MAPTEXTURES_HPP_INCLUDED
#define MAPTEXTURES_HPP_INCLUDED
//
// map-textures.hpp
//
#include "tile-image.hpp"

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

        void acquire(const Context & context, const TileImage image);
        void release(const TileImage image);

        void setup(const Settings & settings);

        inline const TileTexture & get(const TileImage image) const
        {
            return m_tileTextures.at(static_cast<std::size_t>(image));
        }

        inline void setGid(const TileImage image, const int gid)
        {
            m_tileTextures.at(static_cast<std::size_t>(image)).gid = gid;
        }

      private:
        std::vector<TileTexture> m_tileTextures;
    };

} // namespace platformer

#endif // MAPTEXTURES_HPP_INCLUDED
