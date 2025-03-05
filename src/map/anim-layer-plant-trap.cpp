// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-plant-trap.cpp
//
#include "anim-layer-plant-trap.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "map/level.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    PlantTrapAnim::PlantTrapAnim(
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const float t_scale,
        const sf::FloatRect & t_screenRect)
        : is_resetting{ false }
        , is_springing{ false }
        , elapsed_time_sec{ 0.0f }
        , time_between_springs_sec{ 2.0f }
        , time_between_frames_sec{ 0.1f }
        , frame_index{ 0 }
        , sprite{ t_texture, t_textureRect }
        , spring_rect{}
        , coll_rect{}
    {
        sprite.scale({ t_scale, t_scale });

        sprite.setPosition(
            { (util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
              (util::bottom(t_screenRect) - (sprite.getGlobalBounds().size.y * 0.7f)) });

        coll_rect = util::scaleRectInPlaceCopy(sprite.getGlobalBounds(), { 0.7f, 0.2f });

        // make the spring rect bigger so players can walk to and trigger it without harm
        spring_rect = util::scaleRectInPlaceCopy(sprite.getGlobalBounds(), { 1.0f, 0.5f });
    }

    //

    PlantTrapAnimationLayer::PlantTrapAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_texture{}
        , m_anims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        util::TextureLoader::load(
            m_texture, (t_context.settings.media_path / "image/anim/plant-trap.png"));

        m_anims.reserve(t_rects.size());
        for (const sf::FloatRect & rect : t_rects)
        {
            m_anims.emplace_back(
                m_texture,
                textureRect(0),
                t_context.layout.calScaleBasedOnResolution(t_context, 1.5f),
                rect);
        }
    }

    PlantTrapAnimationLayer::~PlantTrapAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void PlantTrapAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const PlantTrapAnim & anim : m_anims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void PlantTrapAnimationLayer::move(const Context &, const float t_amount)
    {
        for (PlantTrapAnim & anim : m_anims)
        {
            anim.sprite.move({ t_amount, 0.0f });
            anim.coll_rect.position.x += t_amount;
            anim.spring_rect.position.x += t_amount;
        }
    }

    void PlantTrapAnimationLayer::update(Context & t_context, const float t_frameTimeSec)
    {
        const sf::FloatRect avatarCollRect{ t_context.avatar.collisionRect() };

        for (PlantTrapAnim & anim : m_anims)
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
                if (avatarCollRect.findIntersection(anim.spring_rect))
                {
                    t_context.sfx.play("mimic");
                    anim.is_springing = true;
                }
            }
        }
    }

    std::size_t PlantTrapAnimationLayer::frameCount() const noexcept
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

    sf::IntRect PlantTrapAnimationLayer::textureRect(const std::size_t frame) const noexcept
    {
        sf::IntRect rect;
        rect.size.x     = static_cast<int>(m_texture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(frame) * rect.size.x);

        return rect;
    }

    Harm PlantTrapAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        for (PlantTrapAnim & anim : m_anims)
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
