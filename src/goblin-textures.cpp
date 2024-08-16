// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// goblin-textures.hpp
//
#include "goblin-textures.hpp"

#include "context.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

#include <filesystem>

namespace platformer
{

    GoblinTextureManager::GoblinTextureManager()
        : m_textures()
    {}

    void GoblinTextureManager::setup(const Settings & settings)
    {
        m_textures.reserve(static_cast<std::size_t>(GoblinAnim::Count));

        for (std::size_t animIndex(0); animIndex < static_cast<std::size_t>(GoblinAnim::Count);
             ++animIndex)
        {
            const GoblinAnim anim{ static_cast<GoblinAnim>(animIndex) };

            const std::filesystem::path path{ settings.media_path / "image/monster/goblin" /
                                              std::string(toString(anim)).append(".png") };

            sf::Texture & texture{ m_textures.emplace_back() };
            texture.loadFromFile(path.string());
            texture.setSmooth(true);
            TextureStats::instance().process(texture);
        }
    }

    std::size_t GoblinTextureManager::frameCount(const GoblinAnim anim) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(anim)) };
        return static_cast<std::size_t>(texture.getSize().x / texture.getSize().y);
    }

    const sf::IntRect
        GoblinTextureManager::rect(const GoblinAnim anim, const std::size_t frame) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(anim)) };

        sf::IntRect rect;

        if (frame >= frameCount(anim))
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

    void GoblinTextureManager::set(
        sf::Sprite & sprite, const GoblinAnim anim, const std::size_t frame) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(anim)) };
        sprite.setTexture(texture);
        sprite.setTextureRect(rect(anim, frame));
    }

} // namespace platformer