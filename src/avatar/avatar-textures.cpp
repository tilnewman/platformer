// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// avatar-textures.hpp
//
#include "avatar/avatar-textures.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/default-texture.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/check-macros.hpp"
#include "util/filesystem-util.hpp"
#include "util/sfml-util.hpp"

namespace bramblefore
{

    AvatarTextureManager::AvatarTextureManager()
        : m_textureSets{}
    {}

    AvatarTextureManager & AvatarTextureManager::instance()
    {
        static AvatarTextureManager avatarTextureManager;
        return avatarTextureManager;
    }

    void AvatarTextureManager::setup(const Settings & t_settings)
    {
        // size all vectors so that no re-allocations ever occur
        m_textureSets.resize(static_cast<std::size_t>(AvatarType::Count));

        for (std::size_t typeIndex(0); typeIndex < static_cast<std::size_t>(AvatarType::Count);
             ++typeIndex)
        {
            const AvatarType type{ static_cast<AvatarType>(typeIndex) };

            const std::filesystem::path typePath{ t_settings.media_path / "image/avatar" /
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

    void AvatarTextureManager::teardown()
    {
        for (std::size_t typeIndex(0); typeIndex < static_cast<std::size_t>(AvatarType::Count);
             ++typeIndex)
        {
            const AvatarTextureSet & set{ m_textureSets.at(typeIndex) };

            M_CHECK(
                (0 == set.ref_count),
                toString(static_cast<AvatarType>(typeIndex))
                    << "'s ref_count was " << set.ref_count);
        }

        m_textureSets.clear();
    }

    void AvatarTextureManager::acquire(const Context & t_context, const AvatarType t_type)
    {
        const std::size_t typeIndex{ static_cast<std::size_t>(t_type) };
        if (typeIndex >= m_textureSets.size())
        {
            return;
        }

        AvatarTextureSet & set{ m_textureSets.at(typeIndex) };

        if (0 == set.ref_count)
        {
            const std::filesystem::path typePath{ t_context.settings.media_path / "image/avatar" /
                                                  toString(t_type) };

            for (std::size_t animIndex(0); animIndex < static_cast<std::size_t>(AvatarAnim::Count);
                 ++animIndex)
            {
                const AvatarAnim anim{ static_cast<AvatarAnim>(animIndex) };
                const std::filesystem::path animPath{ typePath / toString(anim) };

                AnimTextures & anims{ set.anims.at(animIndex) };

                const std::vector<std::filesystem::path> files{ util::findFilesInDirectory(
                    animPath, ".png") };

                M_CHECK(
                    (anims.textures.size() == files.size()),
                    "Initial parse of " << animPath << " found " << anims.textures.size()
                                        << " PNG files but now there are " << files.size());

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

    void AvatarTextureManager::release(const AvatarType t_type)
    {
        const std::size_t typeIndex{ static_cast<std::size_t>(t_type) };
        if (typeIndex >= m_textureSets.size())
        {
            return;
        }

        AvatarTextureSet & set{ m_textureSets.at(typeIndex) };

        M_CHECK((0 != set.ref_count), toString(t_type) << "'s ref_count was already zero.");

        if (1 == set.ref_count)
        {
            for (AnimTextures & anim : set.anims)
            {
                for (sf::Texture & texture : anim.textures)
                {
                    texture = sf::Texture();
                }
            }
        }

        --set.ref_count;
    }

    void AvatarTextureManager::set(
        sf::Sprite & t_sprite,
        const AvatarType t_type,
        const AvatarAnim t_anim,
        const std::size_t t_frame) const
    {
        const AvatarTextureSet & set{ m_textureSets.at(static_cast<std::size_t>(t_type)) };
        const AnimTextures & anims{ set.anims.at(static_cast<std::size_t>(t_anim)) };

        if (t_frame < anims.textures.size())
        {
            t_sprite.setTexture(anims.textures.at(t_frame), true);
        }
    }

    std::size_t
        AvatarTextureManager::frameCount(const AvatarType t_type, const AvatarAnim t_anim) const
    {
        const AvatarTextureSet & set{ m_textureSets.at(static_cast<std::size_t>(t_type)) };
        const AnimTextures & anims{ set.anims.at(static_cast<std::size_t>(t_anim)) };
        return anims.textures.size();
    }

    const sf::Texture & AvatarTextureManager::getDefault(const AvatarType t_type) const
    {
        const std::size_t index{ static_cast<std::size_t>(t_type) };
        if (index >= m_textureSets.size())
        {
            return DefaultTexture::instance().get();
        }
        else
        {
            return m_textureSets.at(index).defalt;
        }
    }

    const sf::Texture & AvatarTextureManager::getIcon(const AvatarType t_type) const
    {
        const std::size_t index{ static_cast<std::size_t>(t_type) };
        if (index >= m_textureSets.size())
        {
            return DefaultTexture::instance().get();
        }
        else
        {
            return m_textureSets.at(static_cast<std::size_t>(t_type)).icon;
        }
    }

} // namespace bramblefore
