// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-lightning.cpp
//
#include "anim-layer-lightning.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    LightningAnimationLayer::LightningAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_texture{}
        , m_anims{}
        , m_timebetweenFrames{ 0.1f }
        , m_frameCount{ 8 }
    {
        HarmCollisionManager::instance().addOwner(*this);

        util::TextureLoader::load(
            m_texture, (t_context.settings.media_path / "image/anim/lightning.png"), true);

        m_anims.reserve(t_rects.size());
        for (const sf::FloatRect & rect : t_rects)
        {
            LightningAnim & anim{ m_anims.emplace_back() };
            anim.sprite.setTexture(m_texture);
            anim.sprite.setTextureRect(textureRect(0));

            const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.0f) };
            anim.sprite.scale({ scale, scale });

            anim.sprite.setPosition(
                { (util::center(rect).x - anim.sprite.getGlobalBounds().size.x),
                  (util::bottom(rect) - anim.sprite.getGlobalBounds().size.y) });

            anim.time_between_anim_sec = t_context.random.fromTo(2.0f, 6.0f);
            anim.is_animating          = false;
            anim.elapsed_time_sec      = 0.0f;
            anim.frame_index           = 0;
        }
    }

    LightningAnimationLayer::~LightningAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void LightningAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const LightningAnim & anim : m_anims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void LightningAnimationLayer::move(const Context &, const float t_amount)
    {
        for (LightningAnim & anim : m_anims)
        {
            anim.sprite.move({ t_amount, 0.0f });
        }
    }

    sf::IntRect LightningAnimationLayer::textureRect(const std::size_t t_frame) const noexcept
    {
        sf::IntRect rect;
        rect.size.x     = 64;
        rect.size.y     = static_cast<int>(m_texture.getSize().y);
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(t_frame) * rect.size.x);

        return rect;
    }

    void LightningAnimationLayer::update(Context & t_context, const float t_frameTimeSec)
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (LightningAnim & anim : m_anims)
        {
            anim.elapsed_time_sec += t_frameTimeSec;

            if (anim.is_animating)
            {
                if (anim.elapsed_time_sec > m_timebetweenFrames)
                {
                    anim.elapsed_time_sec -= m_timebetweenFrames;

                    ++anim.frame_index;
                    if (anim.frame_index >= m_frameCount)
                    {
                        anim.frame_index      = 0;
                        anim.elapsed_time_sec = 0.0f;
                        anim.is_animating     = false;
                    }

                    anim.sprite.setTextureRect(textureRect(anim.frame_index));
                }
            }
            else
            {
                if (anim.elapsed_time_sec > anim.time_between_anim_sec)
                {
                    anim.elapsed_time_sec = 0.0f;
                    anim.is_animating     = true;

                    if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
                    {
                        t_context.sfx.play("electricity");
                    }
                }
            }
        }
    }

    Harm LightningAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        for (const LightningAnim & anim : m_anims)
        {
            if (!anim.is_animating)
            {
                continue;
            }

            const sf::FloatRect reducedBounds{ util::scaleRectInPlaceCopy(
                anim.sprite.getGlobalBounds(), 0.75f) };

            if (!reducedBounds.findIntersection(t_avatarRect))
            {
                continue;
            }

            harm.rect   = reducedBounds;
            harm.damage = 25;
            harm.sfx    = "";
            break;
        }

        return harm;
    }

} // namespace bramblefore
