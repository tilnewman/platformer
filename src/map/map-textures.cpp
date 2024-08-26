// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// map-textures.cpp
//
#include "map-textures.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/sfml-util.hpp"

namespace platformer
{

    MapTextureManager::MapTextureManager()
        : m_tileTextures{}
    {
        // size only this once to prevent any re-allocations
        m_tileTextures.resize(static_cast<std::size_t>(TileImage::Count));
    }

    MapTextureManager & MapTextureManager::instance()
    {
        static MapTextureManager mapTextureManager;
        return mapTextureManager;
    }

    void MapTextureManager::acquire(const Context & t_context, const TileImage t_image)
    {
        TileTexture & tileTexture{ m_tileTextures.at(static_cast<std::size_t>(t_image)) };

        if (0 == tileTexture.ref_count)
        {
            tileTexture.texture.loadFromFile(
                (t_context.settings.media_path /
                 std::string("image/map/").append(toImageFilename(t_image)))
                    .string());

            TextureStats::instance().process(tileTexture.texture);
            tileTexture.size = sf::Vector2i{ tileTexture.texture.getSize() };
        }

        ++tileTexture.ref_count;
    }

    void MapTextureManager::release(const TileImage t_image)
    {
        TileTexture & tileTexture{ m_tileTextures.at(static_cast<std::size_t>(t_image)) };

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

} // namespace platformer
