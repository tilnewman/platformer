// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// acid-anim-layer.cpp
//
#include "acid-anim-layer.hpp"

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

    AcidAnimationLayer::AcidAnimationLayer(
        Context & context, const std::vector<sf::FloatRect> & rects)
        : m_texture()
        , m_sprites()
        , m_elapsedTimeSec(0.0f)
        , m_timePerFrameSec(0.2f)
        , m_frameIndex(0)
    {
        KillCollisionManager::instance().addOwner(*this);

        m_texture.loadFromFile(
            (context.settings.media_path / "image/map-anim/acid-surface.png").string());

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

    AcidAnimationLayer::~AcidAnimationLayer()
    {
        KillCollisionManager::instance().removeOwner(*this);
    }

    void AcidAnimationLayer::draw(
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

    void AcidAnimationLayer::move(const Context &, const float amount)
    {
        for (sf::Sprite & sprite : m_sprites)
        {
            sprite.move(amount, 0.0f);
        }
    }

    std::size_t AcidAnimationLayer::frameCount() const
    {
        return static_cast<std::size_t>(m_texture.getSize().x / m_texture.getSize().y);
    }

    const sf::IntRect AcidAnimationLayer::textureRect(const std::size_t frame) const
    {
        sf::IntRect rect;
        rect.width  = static_cast<int>(m_texture.getSize().y);
        rect.height = rect.width;
        rect.top    = 0;
        rect.left   = (static_cast<int>(frame) * static_cast<int>(rect.width));

        return rect;
    }

    void AcidAnimationLayer::update(Context &, const float frameTimeSec)
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

    const Harm AcidAnimationLayer::avatarCollide(Context &, const sf::FloatRect & avatarRect)
    {
        Harm harm;

        for (const sf::Sprite & sprite : m_sprites)
        {
            const sf::FloatRect acidRect{ sprite.getGlobalBounds() };
            if (avatarRect.intersects(acidRect))
            {
                harm.rect   = acidRect;
                harm.damage = 99999; // TOOD lookup real max health somewhere
                harm.sfx    = "acid";
                break;
            }
        }

        return harm;
    }

} // namespace platformer
