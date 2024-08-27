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

#include <exception>

namespace platformer
{

    MapTextureManager::MapTextureManager()
        : m_tileTextures{}
    {}

    MapTextureManager & MapTextureManager::instance()
    {
        static MapTextureManager mapTextureManager;
        return mapTextureManager;
    }

    void MapTextureManager::setup()
    {
        // size only this once to prevent any re-allocations
        m_tileTextures.resize(static_cast<std::size_t>(TileImage::Count));
    }

    void MapTextureManager::teardown() { m_tileTextures.clear(); }

    void MapTextureManager::acquire(const Context & t_context, const TileImage t_image)
    {
        const std::size_t imageIndex{ static_cast<std::size_t>(t_image) };
        if (imageIndex >= m_tileTextures.size())
        {
            return;
        }

        TileTexture & tileTexture{ m_tileTextures.at(imageIndex) };

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
        const std::size_t imageIndex{ static_cast<std::size_t>(t_image) };
        if (imageIndex >= m_tileTextures.size())
        {
            return;
        }

        TileTexture & tileTexture{ m_tileTextures.at(imageIndex) };

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

    const TileTexture & MapTextureManager::get(const TileImage t_image) const
    {
        const std::size_t imageIndex{ static_cast<std::size_t>(t_image) };
        if (imageIndex >= m_tileTextures.size())
        {
            throw std::runtime_error("MapTextureManager::get() given out of range TileImage.");
        }
        else
        {
            return m_tileTextures.at(imageIndex);
        }
    }

    void MapTextureManager::setGid(const TileImage t_image, const int t_gid)
    {
        const std::size_t imageIndex{ static_cast<std::size_t>(t_image) };
        if (imageIndex >= m_tileTextures.size())
        {
            return;
        }

        m_tileTextures.at(imageIndex).gid = t_gid;
    }

} // namespace platformer
