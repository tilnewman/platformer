// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// avatar.hpp
//
#include "avatar.hpp"

#include "context.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"

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
            avatarTextures.default_texture.setSmooth(true);

            for (std::size_t animIndex(0);
                 animIndex <= static_cast<std::size_t>(AvatarAnim::WalkAttack);
                 ++animIndex)
            {
                const AvatarAnim anim{ static_cast<AvatarAnim>(animIndex) };
                const std::filesystem::path animPath{ typePath / toString(anim) };

                // not all avatars have all anims
                if (!std::filesystem::exists(animPath))
                {
                    continue;
                }

                AnimTextures & textureSet{ avatarTextures.anim_textures.at(animIndex) };
                textureSet.time_per_frame_sec = timePerFrameSec(anim);
                textureSet.textures.reserve(32); // nobody has more than 18 frames

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
                    texture.setSmooth(true);

                    ++fileIndex;
                }
            }
        }
    }

    //

    Avatar::Avatar()
        : m_sprite()
        , m_type(AvatarType::Assassin) // anything works here
        , m_anim(AvatarAnim::Idle)
        , m_elapsedTimeSec(0.0f)
        , m_animIndex(0)
    {}

    void Avatar::setup(const Context & context, const AvatarType & type)
    {
        m_type = type;
        m_anim = AvatarAnim::Idle;

        m_sprite.setTexture(context.avatar_textures.get(m_type, m_anim).textures.at(0), true);
    }

    void Avatar::update(Context & context, const float frameTimeSec)
    {
        const AnimTextures & textureSet{ context.avatar_textures.get(m_type, m_anim) };

        m_elapsedTimeSec += frameTimeSec;
        if (m_elapsedTimeSec > textureSet.time_per_frame_sec)
        {
            m_elapsedTimeSec -= textureSet.time_per_frame_sec;

            ++m_animIndex;
            if (m_animIndex >= textureSet.textures.size())
            {
                m_animIndex = 0;
            }

            if (textureSet.textures.empty())
            {
                m_sprite.setTexture(context.avatar_textures.getDefault(m_type), true);
            }
            else
            {
                m_sprite.setTexture(textureSet.textures.at(m_animIndex), true);
            }
        }
    }

    void Avatar::draw(sf::RenderTarget & target, sf::RenderStates states)
    {
        target.draw(m_sprite, states);
    }

    void Avatar::advanceAnim()
    {
        std::size_t animIndex{ static_cast<std::size_t>(m_anim) };

        ++animIndex;
        if (animIndex >= static_cast<std::size_t>(AvatarAnim::Count))
        {
            animIndex = 0;
        }

        m_anim = static_cast<AvatarAnim>(animIndex);
    }

} // namespace platformer
