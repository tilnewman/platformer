// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-ghost-bottle.cpp
//
#include "anim-layer-ghost-bottle.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "map/level.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    GhostBottleAnimationLayer::GhostBottleAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_texture{}
        , m_anims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        M_CHECK(
            m_texture.loadFromFile(
                (t_context.settings.media_path / "image/anim/ghost-bottle.png").string()),
            "file not found");

        TextureStats::instance().process(m_texture);

        for (const sf::FloatRect & rect : t_rects)
        {
            GhostBottleAnim & anim{ m_anims.emplace_back() };
            anim.coll_rect = rect;
            anim.sprite.setTexture(m_texture);
            anim.sprite.setTextureRect(textureRect(0));

            const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 2.0f) };
            anim.sprite.scale({ scale, scale });

            anim.sprite.setPosition(
                { (util::center(rect).x - (anim.sprite.getGlobalBounds().size.x * 0.5f)),
                  (util::bottom(rect) - (anim.sprite.getGlobalBounds().size.y * 0.9f)) });
        }
    }

    GhostBottleAnimationLayer::~GhostBottleAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void GhostBottleAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const GhostBottleAnim & anim : m_anims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void GhostBottleAnimationLayer::move(const Context &, const float t_amount)
    {
        for (GhostBottleAnim & anim : m_anims)
        {
            anim.sprite.move({ t_amount, 0.0f });
            anim.coll_rect.position.x += t_amount;
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

    std::size_t GhostBottleAnimationLayer::frameCount() const noexcept
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

    sf::IntRect GhostBottleAnimationLayer::textureRect(const std::size_t frame) const noexcept
    {
        sf::IntRect rect;
        rect.size.x     = static_cast<int>(m_texture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(frame) * rect.size.x);

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

            if (t_avatarRect.findIntersection(anim.coll_rect))
            {
                anim.has_emerged = true;

                harm.rect   = anim.coll_rect;
                harm.damage = 20;
                harm.sfx    = "ghost-laugh";

                break;
            }
        }

        return harm;
    }

} // namespace bramblefore
