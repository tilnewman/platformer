// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-bomb.cpp
//
#include "anim-layer-bomb.hpp"

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

    BombAnimationLayer::BombAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_texture{}
        , m_anims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        m_texture.loadFromFile(
            (t_context.settings.media_path / "image/map-anim/bomb.png").string());

        TextureStats::instance().process(m_texture);

        for (const sf::FloatRect & rect : t_rects)
        {
            BombAnim & anim{ m_anims.emplace_back() };
            anim.coll_rect = rect;
            anim.sprite.setTexture(m_texture);
            anim.sprite.setTextureRect(textureRect(0));

            anim.sprite.setPosition(
                (util::center(rect).x - (anim.sprite.getGlobalBounds().width * 0.5f)),
                (util::bottom(rect) - (anim.sprite.getGlobalBounds().height * 0.6f)));
        }
    }

    BombAnimationLayer::~BombAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void BombAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const BombAnim & anim : m_anims)
        {
            if (t_context.layout.wholeRect().intersects(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void BombAnimationLayer::move(const Context &, const float t_amount)
    {
        for (BombAnim & anim : m_anims)
        {
            anim.sprite.move(t_amount, 0.0f);
            anim.coll_rect.left += t_amount;
        }
    }

    void BombAnimationLayer::update(Context &, const float t_frameTimeSec)
    {
        for (BombAnim & anim : m_anims)
        {
            if (!anim.has_exploded)
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

    std::size_t BombAnimationLayer::frameCount() const
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

    sf::IntRect BombAnimationLayer::textureRect(const std::size_t frame) const
    {
        sf::IntRect rect;
        rect.width  = static_cast<int>(m_texture.getSize().y);
        rect.height = rect.width;
        rect.top    = 0;
        rect.left   = (static_cast<int>(frame) * rect.width);
        return rect;
    }

    Harm BombAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        for (BombAnim & anim : m_anims)
        {
            if (anim.has_exploded)
            {
                continue;
            }

            if (t_avatarRect.intersects(anim.coll_rect))
            {
                anim.has_exploded = true;

                harm.rect        = anim.coll_rect;
                harm.damage      = 40;
                harm.sfx         = "bomb";

                break;
            }
        }

        return harm;
    }

} // namespace platformer
