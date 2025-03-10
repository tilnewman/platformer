// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// map-textures.cpp
//
#include "map-textures.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

namespace bramblefore
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

    void MapTextureManager::teardown()
    {
        for (std::size_t imageIndex(0); imageIndex < static_cast<std::size_t>(TileImage::Count);
             ++imageIndex)
        {
            const TileTexture & tileTexture{ m_tileTextures.at(imageIndex) };

            M_CHECK(
                (0 == tileTexture.ref_count),
                toString(static_cast<TileImage>(imageIndex))
                    << "'s ref_count=" << tileTexture.ref_count);
        }

        m_tileTextures = std::vector<TileTexture>();
    }

    void MapTextureManager::acquire(const Context & t_context, const TileImage t_image)
    {
        const std::size_t imageIndex{ static_cast<std::size_t>(t_image) };

        M_CHECK(
            (imageIndex < m_tileTextures.size()),
            "TileImage " << toString(t_image) << " of index " << imageIndex
                         << " >= " << m_tileTextures.size() << " -maybe setup() was not called?");

        TileTexture & tileTexture{ m_tileTextures.at(imageIndex) };

        if (0 == tileTexture.ref_count)
        {
            util::TextureLoader::load(
                tileTexture.texture,
                (t_context.settings.media_path /
                 std::string("image/map/").append(toImageFilename(t_image))));

            tileTexture.size = sf::Vector2i{ tileTexture.texture.getSize() };
        }

        ++tileTexture.ref_count;
    }

    void MapTextureManager::release(const TileImage t_image)
    {
        const std::size_t imageIndex{ static_cast<std::size_t>(t_image) };

        M_CHECK(
            (imageIndex < m_tileTextures.size()),
            "TileImage " << toString(t_image) << " of index " << imageIndex
                         << " >= " << m_tileTextures.size() << " -maybe setup() was not called?");

        TileTexture & tileTexture{ m_tileTextures.at(imageIndex) };

        M_CHECK(
            (0 != tileTexture.ref_count), toString(t_image) << "'s ref_count was already zero.");

        if (1 == tileTexture.ref_count)
        {
            tileTexture.texture = sf::Texture();
        }

        --tileTexture.ref_count;
    }

    const TileTexture & MapTextureManager::get(const TileImage t_image) const
    {
        const std::size_t imageIndex{ static_cast<std::size_t>(t_image) };

        M_CHECK(
            (imageIndex < m_tileTextures.size()),
            "TileImage " << toString(t_image) << " of index " << imageIndex
                         << " >= " << m_tileTextures.size() << " -maybe setup() was not called?");

        return m_tileTextures.at(imageIndex);
    }

    void MapTextureManager::setGid(const TileImage t_image, const int t_gid)
    {
        const std::size_t imageIndex{ static_cast<std::size_t>(t_image) };

        M_CHECK(
            (imageIndex < m_tileTextures.size()),
            "TileImage " << toString(t_image) << " of index " << imageIndex
                         << " >= " << m_tileTextures.size() << " -maybe setup() was not called?");

        m_tileTextures.at(imageIndex).gid = t_gid;
    }

} // namespace bramblefore
