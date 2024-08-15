// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// avatar-textures.hpp
//
#include "avatar-textures.hpp"

#include "context.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

#include <filesystem>

namespace platformer
{

    AvatarTextureManager::AvatarTextureManager()
        : m_textures()
    {
        m_textures.reserve(static_cast<std::size_t>(AvatarType::Witch) + 1);
    }

    void AvatarTextureManager::setup(const Settings & settings)
    {
        const std::filesystem::path avatarPath{ settings.media_path / "image/avatar" };

        m_textures.resize(static_cast<std::size_t>(AvatarType::Count));

        for (std::size_t typeIndex(0); typeIndex < static_cast<std::size_t>(AvatarType::Count);
             ++typeIndex)
        {
            const AvatarType type{ static_cast<AvatarType>(typeIndex) };
            const std::filesystem::path typePath{ avatarPath / toString(type) };

            AvatarTextures & avatarTextures{ m_textures.at(typeIndex) };
            avatarTextures.anim_textures.resize(static_cast<std::size_t>(AvatarAnim::Count));

            const std::filesystem::path defaultFilePath{ typePath / "default.png" };
            avatarTextures.default_texture.loadFromFile(defaultFilePath.string());
            avatarTextures.default_texture.setSmooth(false);
            TextureStats::instance().process(avatarTextures.default_texture);

            const std::filesystem::path iconFilePath{ typePath / "icon.png" };
            avatarTextures.icon_texture.loadFromFile(iconFilePath.string());
            avatarTextures.icon_texture.setSmooth(false);
            TextureStats::instance().process(avatarTextures.icon_texture);

            for (std::size_t animIndex(0); animIndex < static_cast<std::size_t>(AvatarAnim::Count);
                 ++animIndex)
            {
                const AvatarAnim anim{ static_cast<AvatarAnim>(animIndex) };
                const std::filesystem::path animPath{ typePath / toString(anim) };

                AnimTextures & textureSet{ avatarTextures.anim_textures.at(animIndex) };
                textureSet.time_per_frame_sec = timePerFrameSec(anim);
                textureSet.textures.reserve(32); // no avatar has more than 18 frames

                std::size_t fileIndex(0);
                while (true)
                {
                    const std::filesystem::path filePath{ animPath /
                                                          std::string(toString(anim))
                                                              .append(std::to_string(fileIndex + 1))
                                                              .append(".png") };

                    if (!std::filesystem::exists(filePath))
                    {
                        break;
                    }

                    sf::Texture & texture{ textureSet.textures.emplace_back() };
                    texture.loadFromFile(filePath.string());
                    texture.setSmooth(false);
                    TextureStats::instance().process(texture);

                    ++fileIndex;
                }
            }
        }
    }

} // namespace platformer
