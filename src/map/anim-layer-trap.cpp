// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-trap.cpp
//
#include "anim-layer-trap.hpp"

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

    TrapAnimationLayer::TrapAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_texture{}
        , m_anims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        M_CHECK(
            m_texture.loadFromFile(
                (t_context.settings.media_path / "image/anim/trap.png").string()),
            "file not found");

        TextureStats::instance().process(m_texture);

        for (const sf::FloatRect & rect : t_rects)
        {
            TrapAnim & anim{ m_anims.emplace_back() };
            anim.time_between_springs_sec = 2.0f;
            anim.sprite.setTexture(m_texture);
            anim.sprite.setTextureRect(textureRect(0));

            const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.5f) };
            anim.sprite.scale({ scale, scale });

            anim.sprite.setPosition(
                { (util::center(rect).x - (anim.sprite.getGlobalBounds().size.x * 0.5f)),
                  (util::bottom(rect) - (anim.sprite.getGlobalBounds().size.y * 0.7f)) });

            anim.coll_rect =
                util::scaleRectInPlaceCopy(anim.sprite.getGlobalBounds(), { 0.7f, 0.2f });

            // make the spring rect bigger so players can walk to and trigger it without harm
            anim.spring_rect =
                util::scaleRectInPlaceCopy(anim.sprite.getGlobalBounds(), { 1.0f, 0.5f });
        }
    }

    TrapAnimationLayer::~TrapAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void TrapAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const TrapAnim & anim : m_anims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void TrapAnimationLayer::move(const Context &, const float t_amount)
    {
        for (TrapAnim & anim : m_anims)
        {
            anim.sprite.move({ t_amount, 0.0f });
            anim.coll_rect.position.x += t_amount;
            anim.spring_rect.position.x += t_amount;
        }
    }

    void TrapAnimationLayer::update(Context & t_context, const float t_frameTimeSec)
    {
        for (TrapAnim & anim : m_anims)
        {
            if (anim.is_resetting)
            {
                anim.elapsed_time_sec += t_frameTimeSec;
                if (anim.elapsed_time_sec > anim.time_between_springs_sec)
                {
                    anim.elapsed_time_sec = 0.0f;
                    anim.is_resetting     = false;
                    anim.is_springing     = false;
                }
            }
            else if (anim.is_springing)
            {
                anim.elapsed_time_sec += t_frameTimeSec;
                if (anim.elapsed_time_sec > anim.time_between_frames_sec)
                {
                    anim.elapsed_time_sec -= anim.time_between_frames_sec;

                    ++anim.frame_index;
                    if (anim.frame_index >= frameCount())
                    {
                        anim.frame_index      = 0;
                        anim.elapsed_time_sec = 0.0f;
                        anim.is_resetting     = true;
                        anim.is_springing     = false;
                    }

                    anim.sprite.setTextureRect(textureRect(anim.frame_index));
                }
            }
            else
            {
                if (t_context.avatar.collisionRect().findIntersection(anim.spring_rect))
                {
                    t_context.sfx.play("trap-metal");
                    anim.is_springing = true;
                }
            }
        }
    }

    std::size_t TrapAnimationLayer::frameCount() const noexcept
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

    sf::IntRect TrapAnimationLayer::textureRect(const std::size_t frame) const noexcept
    {
        sf::IntRect rect;
        rect.size.x     = static_cast<int>(m_texture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(frame) * rect.size.x);

        return rect;
    }

    Harm TrapAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        for (TrapAnim & anim : m_anims)
        {
            if (anim.is_resetting)
            {
                continue;
            }

            if (t_avatarRect.findIntersection(anim.coll_rect))
            {
                anim.is_springing = true;

                harm.rect   = anim.coll_rect;
                harm.damage = 10;
                harm.sfx    = "trap-metal";

                break;
            }
        }

        return harm;
    }

} // namespace bramblefore
