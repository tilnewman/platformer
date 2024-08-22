// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// map-textures.cpp
//
#include "map-textures.hpp"

#include "context.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

namespace platformer
{

    MapTextureManager::MapTextureManager()
        : m_tileTextures()
    {
        m_tileTextures.resize(static_cast<std::size_t>(TileImage::Count));
    }

    MapTextureManager & MapTextureManager::instance()
    {
        static MapTextureManager mapTextureManager;
        return mapTextureManager;
    }

    void MapTextureManager::acquire(const Context & context, const TileImage image)
    {
        TileTexture & tileTexture{ m_tileTextures.at(static_cast<std::size_t>(image)) };

        if (0 == tileTexture.ref_count)
        {
            tileTexture.texture.loadFromFile(
                (context.settings.media_path /
                 std::string("image/map/").append(toImageFilename(image)))
                    .string());

            TextureStats::instance().process(tileTexture.texture);
            tileTexture.size = sf::Vector2i{ tileTexture.texture.getSize() };
        }

        ++tileTexture.ref_count;
    }

    void MapTextureManager::release(const TileImage image)
    {
        TileTexture & tileTexture{ m_tileTextures.at(static_cast<std::size_t>(image)) };

        if (tileTexture.ref_count <= 1)
        {
            tileTexture.ref_count = 0;
            tileTexture.texture   = sf::Texture();
        }
        else
        {
            --tileTexture.ref_count;
        }
    }

    void MapTextureManager::setup(const Settings &) {}

} // namespace platformer
