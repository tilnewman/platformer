// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// avatar-textures.hpp
//
#include "avatar-textures.hpp"

#include "context.hpp"
#include "filesystem-util.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

#include <iostream>

namespace platformer
{

    AvatarTextureManager::AvatarTextureManager()
        : m_textureSets()
    {}

    AvatarTextureManager & AvatarTextureManager::instance()
    {
        static AvatarTextureManager avatarTextureManager;
        return avatarTextureManager;
    }

    void AvatarTextureManager::setup(const Settings & settings)
    {
        // size all vectors so that no re-allocations ever occur
        m_textureSets.resize(static_cast<std::size_t>(AvatarType::Count));

        for (std::size_t typeIndex(0); typeIndex < static_cast<std::size_t>(AvatarType::Count);
             ++typeIndex)
        {
            const AvatarType type{ static_cast<AvatarType>(typeIndex) };

            const std::filesystem::path typePath{ settings.media_path / "image/avatar" /
                                                  toString(type) };

            AvatarTextureSet & set{ m_textureSets.at(typeIndex) };
            set.anims.resize(static_cast<std::size_t>(AvatarAnim::Count));

            set.defalt.loadFromFile((typePath / "default.png").string());
            TextureStats::instance().process(set.defalt);

            set.icon.loadFromFile((typePath / "icon.png").string());
            TextureStats::instance().process(set.icon);

            for (std::size_t animIndex(0); animIndex < static_cast<std::size_t>(AvatarAnim::Count);
                 ++animIndex)
            {
                const AvatarAnim anim{ static_cast<AvatarAnim>(animIndex) };
                const std::filesystem::path animPath{ typePath / toString(anim) };

                AnimTextures & anims{ set.anims.at(animIndex) };

                const std::vector<std::filesystem::path> files{ util::findFilesInDirectory(
                    animPath, ".png") };

                anims.textures.resize(files.size());
            }
        }
    }

    void AvatarTextureManager::acquire(const Context & context, const AvatarType type)
    {
        AvatarTextureSet & set{ m_textureSets.at(static_cast<std::size_t>(type)) };

        if (0 == set.ref_count)
        {
            const std::filesystem::path typePath{ context.settings.media_path / "image/avatar" /
                                                  toString(type) };

            for (std::size_t animIndex(0); animIndex < static_cast<std::size_t>(AvatarAnim::Count);
                 ++animIndex)
            {
                const AvatarAnim anim{ static_cast<AvatarAnim>(animIndex) };
                const std::filesystem::path animPath{ typePath / toString(anim) };

                AnimTextures & anims{ set.anims.at(animIndex) };

                const std::vector<std::filesystem::path> files{ util::findFilesInDirectory(
                    animPath, ".png") };

                if (anims.textures.size() != files.size())
                {
                    std::cout << "Error:  Initial parse of " << animPath << " found "
                              << anims.textures.size() << " PNG files but now there are "
                              << files.size() << '\n';

                    continue;
                }

                for (std::size_t frameIndex{ 0 }; frameIndex < files.size(); ++frameIndex)
                {
                    const std::filesystem::path path{ files.at(frameIndex) };
                    sf::Texture & texture{ anims.textures.at(frameIndex) };
                    texture.loadFromFile(path.string());
                    TextureStats::instance().process(texture);
                }
            }
        }

        ++set.ref_count;
    }

    void AvatarTextureManager::release(const AvatarType type)
    {
        AvatarTextureSet & set{ m_textureSets.at(static_cast<std::size_t>(type)) };

        if (set.ref_count <= 1)
        {
            set.ref_count = 0;

            for (AnimTextures & anim : set.anims)
            {
                for (sf::Texture & texture : anim.textures)
                {
                    texture = sf::Texture();
                }
            }
        }
        else
        {
            --set.ref_count;
        }
    }

    void AvatarTextureManager::set(
        sf::Sprite & sprite,
        const AvatarType type,
        const AvatarAnim anim,
        const std::size_t frame) const
    {
        const AvatarTextureSet & set{ m_textureSets.at(static_cast<std::size_t>(type)) };
        const AnimTextures & anims{ set.anims.at(static_cast<std::size_t>(anim)) };

        if (frame >= anims.textures.size())
        {
            std::cout << "Error: AvatarTextureManager::set(" << toString(type) << ", "
                      << toString(anim) << ", frameIndex=" << frame
                      << ") but that frameIndex is >= to the max of " << anims.textures.size()
                      << '\n';

            return;
        }

        sprite.setTexture(anims.textures.at(frame), true);
    }

} // namespace platformer
