// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-bomb.cpp
//
#include "anim-layer-bomb.hpp"

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
    BombAnim::BombAnim(
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const sf::FloatRect & t_rect,
        const float t_scale)
        : has_exploded{ false }
        , elapsed_time_sec{ 0.0f }
        , time_between_frames_sec{ 0.1f }
        , frame_index{ 0 }
        , sprite{ t_texture, t_textureRect }
        , coll_rect{ t_rect }
    {
        sprite.setScale({ t_scale, t_scale });

        sprite.setPosition(
            { (util::center(coll_rect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
              (util::bottom(coll_rect) - (sprite.getGlobalBounds().size.y * 0.6f)) });
    }

    //

    BombAnimationLayer::BombAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_texture{}
        , m_anims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        util::TextureLoader::load(
            m_texture, (t_context.settings.media_path / "image/anim/bomb.png"));

        for (const sf::FloatRect & rect : t_rects)
        {
            m_anims.emplace_back(
                m_texture,
                textureRect(0),
                rect,
                t_context.layout.calScaleBasedOnResolution(t_context, 1.0f));
        }
    }

    BombAnimationLayer::~BombAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void BombAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const BombAnim & anim : m_anims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void BombAnimationLayer::move(const Context &, const float t_amount)
    {
        for (BombAnim & anim : m_anims)
        {
            anim.sprite.move({ t_amount, 0.0f });
            anim.coll_rect.position.x += t_amount;
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

    std::size_t BombAnimationLayer::frameCount() const noexcept
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

    sf::IntRect BombAnimationLayer::textureRect(const std::size_t frame) const noexcept
    {
        sf::IntRect rect;
        rect.size.x     = static_cast<int>(m_texture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(frame) * rect.size.x);

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

            if (t_avatarRect.findIntersection(anim.coll_rect))
            {
                anim.has_exploded = true;

                harm.rect   = anim.coll_rect;
                harm.damage = 40;
                harm.sfx    = "bomb";

                break;
            }
        }

        return harm;
    }

    void BombAnimationLayer::dumpInfo() const
    {
        std::clog << "\tBomb Trap Animation Layer: x" << m_anims.size() << '\n';
    }

} // namespace bramblefore
