// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// water-anim-layer.cpp
//
#include "water-anim-layer.hpp"

#include "avatar.hpp"
#include "context.hpp"
#include "level.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "sound-player.hpp"
#include "texture-stats.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    WaterAnimationLayer::WaterAnimationLayer(
        Context & context, const std::vector<sf::FloatRect> & rects)
        : m_texture()
        , m_sprites()
        , m_elapsedTimeSec(0.0f)
        , m_timePerFrameSec(0.3f)
        , m_frameIndex(0)
    {
        HarmCollisionManager::instance().addOwner(*this);

        m_texture.loadFromFile(
            (context.settings.media_path / "image/map-anim/water-surface1.png").string());

        TextureStats::instance().process(m_texture);

        m_sprites.reserve(rects.size());

        for (const sf::FloatRect & rect : rects)
        {
            sf::Sprite & sprite{ m_sprites.emplace_back() };
            sprite.setTexture(m_texture);
            sprite.setTextureRect(textureRect(0));
            util::fitAndCenterInside(sprite, rect);
        }
    }

    WaterAnimationLayer::~WaterAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void WaterAnimationLayer::draw(
        const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const sf::Sprite & sprite : m_sprites)
        {
            if (context.layout.wholeRect().intersects(sprite.getGlobalBounds()))
            {
                target.draw(sprite, states);
            }
        }
    }

    void WaterAnimationLayer::move(const Context &, const float amount)
    {
        for (sf::Sprite & sprite : m_sprites)
        {
            sprite.move(amount, 0.0f);
        }
    }

    std::size_t WaterAnimationLayer::frameCount() const
    {
        return static_cast<std::size_t>(m_texture.getSize().x / m_texture.getSize().y);
    }

    const sf::IntRect WaterAnimationLayer::textureRect(const std::size_t frame) const
    {
        sf::IntRect rect;
        rect.width  = static_cast<int>(m_texture.getSize().y);
        rect.height = rect.width;
        rect.top    = 0;
        rect.left   = (static_cast<int>(frame) * rect.width);

        return rect;
    }

    void WaterAnimationLayer::update(Context &, const float frameTimeSec)
    {
        m_elapsedTimeSec += frameTimeSec;
        if (m_elapsedTimeSec > m_timePerFrameSec)
        {
            m_elapsedTimeSec -= m_timePerFrameSec;

            ++m_frameIndex;
            if (m_frameIndex >= frameCount())
            {
                m_frameIndex = 0;
            }

            for (sf::Sprite & sprite : m_sprites)
            {
                sprite.setTextureRect(textureRect(m_frameIndex));
            }
        }
    }

    const Harm WaterAnimationLayer::avatarCollide(Context &, const sf::FloatRect & avatarRect)
    {
        Harm harm;

        for (const sf::Sprite & sprite : m_sprites)
        {
            const sf::FloatRect waterRect{ sprite.getGlobalBounds() };
            if (avatarRect.intersects(waterRect))
            {
                harm.rect   = waterRect;
                harm.damage = 0;
                harm.sfx    = "dunk";
                break;
            }
        }

        return harm;
    }

} // namespace platformer
