// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// ghost-bottle-anim-layer.cpp
//
#include "ghost-bottle-anim-layer.hpp"

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

    GhostBottleAnimationLayer::GhostBottleAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_texture{}
        , m_anims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        m_texture.loadFromFile(
            (t_context.settings.media_path / "image/map-anim/ghost-bottle.png").string());

        TextureStats::instance().process(m_texture);

        for (const sf::FloatRect & rect : t_rects)
        {
            GhostBottleAnim & anim{ m_anims.emplace_back() };
            anim.coll_rect = rect;
            anim.sprite.setTexture(m_texture);
            anim.sprite.setTextureRect(textureRect(0));
            anim.sprite.scale(2.0f, 2.0f);

            anim.sprite.setPosition(
                (util::center(rect).x - (anim.sprite.getGlobalBounds().width * 0.5f)),
                (util::bottom(rect) - (anim.sprite.getGlobalBounds().height * 0.9f)));
        }
    }

    GhostBottleAnimationLayer::~GhostBottleAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void GhostBottleAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const GhostBottleAnim & anim : m_anims)
        {
            if (t_context.layout.wholeRect().intersects(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void GhostBottleAnimationLayer::move(const Context &, const float t_amount)
    {
        for (GhostBottleAnim & anim : m_anims)
        {
            anim.sprite.move(t_amount, 0.0f);
            anim.coll_rect.left += t_amount;
        }
    }

    void GhostBottleAnimationLayer::update(Context &, const float t_frameTimeSec)
    {
        for (GhostBottleAnim & anim : m_anims)
        {
            if (!anim.has_emerged)
            {
                continue;
            }

            anim.elapsed_time_sec += t_frameTimeSec;
            if (anim.elapsed_time_sec > anim.time_between_frames_sec)
            {
                anim.elapsed_time_sec -= anim.time_between_frames_sec;

                ++anim.frame_index;
                if (anim.frame_index < frameCount())
                {
                    anim.sprite.setTextureRect(textureRect(anim.frame_index));
                }
            }
        }
    }

    std::size_t GhostBottleAnimationLayer::frameCount() const
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

    sf::IntRect GhostBottleAnimationLayer::textureRect(const std::size_t frame) const
    {
        sf::IntRect rect;
        rect.width  = static_cast<int>(m_texture.getSize().y);
        rect.height = rect.width;
        rect.top    = 0;
        rect.left   = (static_cast<int>(frame) * rect.width);
        return rect;
    }

    Harm GhostBottleAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        for (GhostBottleAnim & anim : m_anims)
        {
            if (anim.has_emerged)
            {
                continue;
            }

            if (t_avatarRect.intersects(anim.coll_rect))
            {
                anim.has_emerged = true;

                harm.rect        = anim.coll_rect;
                harm.damage      = 10;
                harm.sfx         = "ghost-laugh";

                break;
            }
        }

        return harm;
    }

} // namespace platformer
