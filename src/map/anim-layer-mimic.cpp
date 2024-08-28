// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-mimic.cpp
//
#include "anim-layer-mimic.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "map/level.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    MimicAnimationLayer::MimicAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_texture{}
        , m_anims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        m_texture.loadFromFile(
            (t_context.settings.media_path / "image/map-anim/mimic.png").string());

        TextureStats::instance().process(m_texture);

        for (const sf::FloatRect & rect : t_rects)
        {
            MimicAnim & anim{ m_anims.emplace_back() };
            anim.time_between_springs_sec = 2.0f;
            anim.sprite.setTexture(m_texture);
            anim.sprite.setTextureRect(textureRect(0));
            anim.sprite.scale(2.0f, 2.0f);

            anim.sprite.setPosition(
                (util::center(rect).x - (anim.sprite.getGlobalBounds().width * 0.5f)),
                (util::bottom(rect) - (anim.sprite.getGlobalBounds().height * 0.85f)));

            anim.coll_rect = util::scaleRectInPlaceCopy(anim.sprite.getGlobalBounds(), 0.65f);
        }
    }

    MimicAnimationLayer::~MimicAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void MimicAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const MimicAnim & anim : m_anims)
        {
            if (t_context.layout.wholeRect().intersects(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void MimicAnimationLayer::move(const Context &, const float t_amount)
    {
        for (MimicAnim & anim : m_anims)
        {
            anim.sprite.move(t_amount, 0.0f);
            anim.coll_rect.left += t_amount;
        }
    }

    void MimicAnimationLayer::update(Context &, const float t_frameTimeSec)
    {
        for (MimicAnim & anim : m_anims)
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
            //else
            //{
            //    if (t_context.avatar.collisionRect().intersects(anim.coll_rect))
            //    {
            //        t_context.sfx.play("mimic");
            //        anim.is_springing = true;
            //    }
            //}
        }
    }

    std::size_t MimicAnimationLayer::frameCount() const
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

    sf::IntRect MimicAnimationLayer::textureRect(const std::size_t frame) const
    {
        sf::IntRect rect;
        rect.width  = static_cast<int>(m_texture.getSize().y);
        rect.height = rect.width;
        rect.top    = 0;
        rect.left   = (static_cast<int>(frame) * rect.width);
        return rect;
    }

    Harm MimicAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        for (MimicAnim & anim : m_anims)
        {
            if (anim.is_resetting)
            {
                continue;
            }

            if (t_avatarRect.intersects(anim.coll_rect))
            {
                anim.is_springing = true;
                harm.rect         = anim.coll_rect;
                harm.damage       = 25;
                harm.sfx          = "mimic";
                break;
            }
        }

        return harm;
    }

} // namespace bramblefore
