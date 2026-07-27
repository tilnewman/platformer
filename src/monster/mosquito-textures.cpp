//
// mosquito-textures.cpp
//
#include "mosquito-textures.hpp"

#include "bramblefore/settings.hpp"
#include "monster/monster-mosquito.hpp"
#include "subsystem/context.hpp"
#include "util/check-macros.hpp"
#include "util/filesystem-util.hpp"
#include "util/texture-loader.hpp"

#include <filesystem>

namespace bramblefore
{

    MosquitoTextureManager::MosquitoTextureManager()
        : m_refCount{ 0 }
        , m_textures{}
    {}

    MosquitoTextureManager & MosquitoTextureManager::instance()
    {
        static MosquitoTextureManager instance;
        return instance;
    }

    void MosquitoTextureManager::acquire(const Context & t_context)
    {
        if (m_refCount == 0)
        {
            // exact size to prevent re-allocations
            m_textures.reserve(static_cast<std::size_t>(MosquitoAnim::Count));

            for (std::size_t i{ 0 }; i < static_cast<std::size_t>(MosquitoAnim::Count); ++i)
            {
                const MosquitoAnim anim{ static_cast<MosquitoAnim>(i) };

                std::vector<sf::Texture> & textures = m_textures.emplace_back();

                const std::filesystem::path imageDirPath{ t_context.settings.media_path / "image" /
                                                          "monster" / "mosquito" / toString(anim) };

                const std::vector<std::filesystem::path> files{ util::findFilesInDirectory(
                    imageDirPath, ".png") };

                M_CHECK(
                    !files.empty(),
                    "Failed to find any MosquitoAnim::" << toString(anim) << " images!");

                textures.reserve(files.size()); // exact size to prevent re-allocations

                for (const std::filesystem::path & path : files)
                {
                    sf::Texture & texture{ textures.emplace_back() };
                    util::TextureLoader::load(texture, path, true);
                }
            }
        }

        ++m_refCount;
    }

    void MosquitoTextureManager::release()
    {
        M_CHECK((m_refCount > 0), "Tried to release() when the m_refCount is zero!");

        if (m_refCount == 1)
        {
            for (std::vector<sf::Texture> & vec : m_textures)
            {
                for (sf::Texture & texture : vec)
                {
                    texture = sf::Texture();
                }
            }
        }

        --m_refCount;
    }

} // namespace bramblefore
