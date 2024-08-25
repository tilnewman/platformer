// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// water-anim-layer.cpp
//
#include "water-anim-layer.hpp"

#include "avatar.hpp"
#include "context.hpp"
#include "level.hpp"
#include "random.hpp"
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
        , m_anims()
    {
        HarmCollisionManager::instance().addOwner(*this);

        m_texture.loadFromFile(
            (context.settings.media_path / "image/map-anim/water-surface1.png").string());

        TextureStats::instance().process(m_texture);

        m_anims.reserve(rects.size());

        for (const sf::FloatRect & rect : rects)
        {
            WaterAnim & anim{ m_anims.emplace_back() };
            anim.time_per_frame_sec = context.random.fromTo(0.3f, 2.0f);
            anim.sprite.setTexture(m_texture);
            anim.sprite.setTextureRect(textureRect(0));
            util::fitAndCenterInside(anim.sprite, rect);
        }
    }

    WaterAnimationLayer::~WaterAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void WaterAnimationLayer::draw(
        const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const WaterAnim & anim : m_anims)
        {
            if (context.layout.wholeRect().intersects(anim.sprite.getGlobalBounds()))
            {
                target.draw(anim.sprite, states);
            }
        }
    }

    void WaterAnimationLayer::move(const Context &, const float amount)
    {
        for (WaterAnim & anim : m_anims)
        {
            anim.sprite.move(amount, 0.0f);
        }
    }

    std::size_t WaterAnimationLayer::frameCount() const
    {
        if (m_texture.getSize().y > 0)
        {
            return static_cast<std::size_t>(m_texture.getSize().x / m_texture.getSize().y);
        }
        else
        {
            return 0;
        }
    }

    sf::IntRect WaterAnimationLayer::textureRect(const std::size_t frame) const
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
        for (WaterAnim & anim : m_anims)
        {
            anim.elapsed_time_sec += frameTimeSec;
            if (anim.elapsed_time_sec > anim.time_per_frame_sec)
            {
                anim.elapsed_time_sec -= anim.time_per_frame_sec;

                ++anim.frame_index;
                if (anim.frame_index >= frameCount())
                {
                    anim.frame_index = 0;
                }

                anim.sprite.setTextureRect(textureRect(anim.frame_index));
            }
        }
    }

    Harm WaterAnimationLayer::avatarCollide(Context &, const sf::FloatRect & avatarRect)
    {
        Harm harm;

        for (const WaterAnim & anim : m_anims)
        {
            const sf::FloatRect waterRect{ anim.sprite.getGlobalBounds() };
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
