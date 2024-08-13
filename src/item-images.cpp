// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// item-images.cpp
//
#include "item-images.hpp"

#include "context.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

namespace platformer
{

    ItemImages::ItemImages()
        : m_iconsTexture()
        , m_backgroundTexture()
    {}

    void ItemImages::setup(const Settings & settings)
    {
        m_iconsTexture.loadFromFile((settings.media_path / "image/item/items.png").string());
        m_iconsTexture.setSmooth(true);
        TextureStats::instance().process(m_iconsTexture);

        m_backgroundTexture.loadFromFile(
            (settings.media_path / "image/item/item-background.png").string());

        m_backgroundTexture.setSmooth(false);

        TextureStats::instance().process(m_backgroundTexture);
    }

    const sf::Sprite ItemImages::sprite(const Item item) const
    {
        return sf::Sprite(m_iconsTexture, textureRect(item));
    }

    const sf::Sprite ItemImages::background() const { return sf::Sprite(m_backgroundTexture); }

} // namespace platformer
