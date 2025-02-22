// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-textures.hpp
//
#include "monster/monster-textures.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-util.hpp"

#include <filesystem>

namespace bramblefore
{

    MonsterTextureManager::MonsterTextureManager()
        : m_textureSets{}
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

    void MonsterTextureManager::teardown()
    {
        for (std::size_t typeIndex(0); typeIndex < static_cast<std::size_t>(MonsterType::Count);
             ++typeIndex)
        {
            const MonsterTextures & set{ m_textureSets.at(typeIndex) };

            M_CHECK(
                (0 == set.ref_count),
                toString(static_cast<MonsterType>(typeIndex)) << "'s ref_count=" << set.ref_count);
        }

        m_textureSets.clear();
    }

    void MonsterTextureManager::acquire(Context & t_context, const MonsterType t_type)
    {
        const std::size_t typeIndex{ static_cast<std::size_t>(t_type) };

        M_CHECK(
            (typeIndex < m_textureSets.size()),
            toString(t_type) << " of " << typeIndex << " >= " << m_textureSets.size());

        MonsterTextures & set{ m_textureSets.at(typeIndex) };

        if (0 == set.ref_count)
        {
            for (std::size_t animIndex{ 0 };
                 animIndex < static_cast<std::size_t>(MonsterAnim::Count);
                 ++animIndex)
            {
                const MonsterAnim anim{ static_cast<MonsterAnim>(animIndex) };
                sf::Texture & texture{ set.textures.at(animIndex) };

                const std::filesystem::path path{ t_context.settings.media_path / "image/monster/" /
                                                  std::string(toString(t_type)) /
                                                  std::string(toString(anim)).append(".png") };

                texture.loadFromFile(path.string());
                texture.setSmooth(true);
                TextureStats::instance().process(texture);
            }
        }

        ++set.ref_count;
    }

    void MonsterTextureManager::release(const MonsterType t_type)
    {
        const std::size_t typeIndex{ static_cast<std::size_t>(t_type) };

        M_CHECK(
            (typeIndex < m_textureSets.size()),
            toString(t_type) << " of " << typeIndex << " >= " << m_textureSets.size());

        MonsterTextures & set{ m_textureSets.at(typeIndex) };

        M_CHECK((0 != set.ref_count), toString(t_type) << "'s ref_count was already zero.");

        if (1 == set.ref_count)
        {
            for (sf::Texture & texture : set.textures)
            {
                texture = sf::Texture();
            }
        }

        --set.ref_count;
    }

    void MonsterTextureManager::setTexture(
        sf::Sprite & t_sprite,
        const MonsterType t_type,
        const MonsterAnim t_anim,
        const std::size_t t_frame) const
    {
        t_sprite.setTexture(getTexture(t_type, t_anim));
        t_sprite.setTextureRect(getTextureRect(t_type, t_anim, t_frame));
    }

    std::size_t
        MonsterTextureManager::frameCount(const MonsterType t_type, const MonsterAnim t_anim) const
    {
        const sf::Texture & texture{ getTexture(t_type, t_anim) };

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
        MonsterTextureManager::getTexture(const MonsterType t_type, const MonsterAnim t_anim) const
    {
        const std::size_t typeIndex{ static_cast<std::size_t>(t_type) };

        M_CHECK(
            (typeIndex < m_textureSets.size()),
            toString(t_type) << " of " << typeIndex << " >= " << m_textureSets.size());

        const MonsterTextures & set{ m_textureSets.at(typeIndex) };

        const std::size_t animIndex{ static_cast<std::size_t>(t_anim) };

        M_CHECK(
            (animIndex < set.textures.size()),
            toString(t_anim) << " of " << animIndex << " >= " << set.textures.size());

        return m_textureSets.at(typeIndex).textures.at(animIndex);
    }

    sf::IntRect MonsterTextureManager::getTextureRect(
        const MonsterType t_type, const MonsterAnim t_anim, const std::size_t t_frame) const
    {
        const sf::Texture & texture{ getTexture(t_type, t_anim) };

        sf::IntRect rect;

        if (t_frame >= frameCount(t_type, t_anim))
        {
            rect.left = 0;
        }
        else
        {
            rect.left = static_cast<int>(static_cast<std::size_t>(texture.getSize().y) * t_frame);
        }

        rect.top    = 0;
        rect.width  = static_cast<int>(texture.getSize().y);
        rect.height = rect.width;

        return rect;
    }

} // namespace bramblefore
