// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// lightning-anim-layer.cpp
//
#include "lightning-anim-layer.hpp"

#include "context.hpp"
#include "random.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    LightningAnimationLayer::LightningAnimationLayer(
        Context & context, const std::vector<sf::FloatRect> & rects)
        : m_texture()
        , m_anims()
        , m_timebetweenFrames(0.1f)
        , m_frameCount(8)
    {
        m_texture.loadFromFile(
            (context.settings.media_path / "image/map-anim/lightning.png").string());

        m_texture.setSmooth(true);

        TextureStats::instance().process(m_texture);

        m_anims.reserve(rects.size());
        for (const sf::FloatRect & rect : rects)
        {
            LightningAnim & anim{ m_anims.emplace_back() };
            anim.sprite.setTexture(m_texture);
            anim.sprite.setTextureRect(textureRect(0));

            anim.sprite.setPosition(
                (util::center(rect).x - anim.sprite.getGlobalBounds().width),
                (util::bottom(rect) - anim.sprite.getGlobalBounds().height));

            anim.time_between_anim_sec = context.random.fromTo(2.0f, 6.0f);
            anim.is_animating          = false;
            anim.elapsed_time_sec      = 0.0f;
            anim.frame_index           = 0;
        }
    }

    void LightningAnimationLayer::draw(
        const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const LightningAnim & anim : m_anims)
        {
            if (context.layout.wholeRect().intersects(anim.sprite.getGlobalBounds()))
            {
                target.draw(anim.sprite, states);
            }
        }
    }

    void LightningAnimationLayer::move(const Context &, const float amount)
    {
        for (LightningAnim & anim : m_anims)
        {
            anim.sprite.move(amount, 0.0f);
        }
    }

    const sf::IntRect LightningAnimationLayer::textureRect(const std::size_t frame) const
    {
        sf::IntRect rect;
        rect.width  = 64;
        rect.height = static_cast<int>(m_texture.getSize().y);
        rect.top    = 0;
        rect.left   = (static_cast<int>(frame) * rect.width);

        return rect;
    }

    void LightningAnimationLayer::update(Context &, const float frameTimeSec)
    {
        for (LightningAnim & anim : m_anims)
        {
            anim.elapsed_time_sec += frameTimeSec;

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
                }
            }
        }
    }

} // namespace platformer
