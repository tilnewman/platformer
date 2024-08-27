// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-textures.hpp
//
#include "monster/monster-textures.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/default-texture.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/sfml-util.hpp"

#include <filesystem>

namespace platformer
{

    MonsterTextureManager::MonsterTextureManager()
        : m_textureSets()
    {}

    MonsterTextureManager & MonsterTextureManager::instance()
    {
        static MonsterTextureManager monsterTextureManager;
        return monsterTextureManager;
    }

    void MonsterTextureManager::setup(const Settings &)
    {
        // create all texture vectors of correct size so they are never re-allocated
        m_textureSets.resize(static_cast<std::size_t>(MonsterType::Count));
        for (MonsterTextures & set : m_textureSets)
        {
            set.textures.resize(static_cast<std::size_t>(MonsterAnim::Count));
        }
    }

    void MonsterTextureManager::teardown() { m_textureSets.clear(); }

    void MonsterTextureManager::acquire(Context & context, const MonsterType type)
    {
        const std::size_t typeIndex{ static_cast<std::size_t>(type) };
        if (typeIndex >= m_textureSets.size())
        {
            return;
        }

        MonsterTextures & set{ m_textureSets.at(typeIndex) };

        if (0 == set.ref_count)
        {
            for (std::size_t animIndex{ 0 };
                 animIndex < static_cast<std::size_t>(MonsterAnim::Count);
                 ++animIndex)
            {
                const MonsterAnim anim{ static_cast<MonsterAnim>(animIndex) };
                sf::Texture & texture{ set.textures.at(animIndex) };

                const std::filesystem::path path{ context.settings.media_path / "image/monster/" /
                                                  std::string(toString(type)) /
                                                  std::string(toString(anim)).append(".png") };

                texture.loadFromFile(path.string());
                texture.setSmooth(true);
                TextureStats::instance().process(texture);
            }
        }

        ++set.ref_count;
    }

    void MonsterTextureManager::release(const MonsterType type)
    {
        const std::size_t typeIndex{ static_cast<std::size_t>(type) };
        if (typeIndex >= m_textureSets.size())
        {
            return;
        }

        MonsterTextures & set{ m_textureSets.at(typeIndex) };

        if (set.ref_count >= 2)
        {
            --set.ref_count;
        }
        else
        {
            set.ref_count = 0;

            for (sf::Texture & texture : set.textures)
            {
                texture = sf::Texture();
            }
        }
    }

    void MonsterTextureManager::setTexture(
        sf::Sprite & sprite,
        const MonsterType type,
        const MonsterAnim anim,
        const std::size_t frame) const
    {
        sprite.setTexture(getTexture(type, anim));
        sprite.setTextureRect(getTextureRect(type, anim, frame));
    }

    std::size_t
        MonsterTextureManager::frameCount(const MonsterType type, const MonsterAnim anim) const
    {
        const sf::Texture & texture{ getTexture(type, anim) };

        if (texture.getSize().y > 0)
        {
            return static_cast<std::size_t>(texture.getSize().x / texture.getSize().y);
        }
        else
        {
            return 0;
        }
    }

    const sf::Texture &
        MonsterTextureManager::getTexture(const MonsterType type, const MonsterAnim anim) const
    {
        const std::size_t typeIndex{ static_cast<std::size_t>(type) };
        if (typeIndex >= m_textureSets.size())
        {
            return DefaultTexture::instance().get();
        }

        const MonsterTextures & set{ m_textureSets.at(typeIndex) };

        const std::size_t animIndex{ static_cast<std::size_t>(anim) };
        if (animIndex >= set.textures.size())
        {
            return DefaultTexture::instance().get();
        }

        return m_textureSets.at(typeIndex).textures.at(animIndex);
    }

    sf::IntRect MonsterTextureManager::getTextureRect(
        const MonsterType type, const MonsterAnim anim, const std::size_t frame) const
    {
        const sf::Texture & texture{ getTexture(type, anim) };

        sf::IntRect rect;

        if (frame >= frameCount(type, anim))
        {
            rect.left = 0;
        }
        else
        {
            rect.left = static_cast<int>(static_cast<std::size_t>(texture.getSize().y) * frame);
        }

        rect.top    = 0;
        rect.width  = static_cast<int>(texture.getSize().y);
        rect.height = rect.width;

        return rect;
    }

} // namespace platformer
