// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-water.cpp
//
#include "anim-layer-water.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "map/level.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    WaterAnimationLayer::WaterAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_texture{}
        , m_anims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        m_texture.loadFromFile(
            (t_context.settings.media_path / "image/map-anim/water-surface1.png").string());

        TextureStats::instance().process(m_texture);

        m_anims.reserve(t_rects.size());

        for (const sf::FloatRect & rect : t_rects)
        {
            WaterAnim & anim{ m_anims.emplace_back() };
            anim.time_per_frame_sec = t_context.random.fromTo(0.3f, 2.0f);
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
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const WaterAnim & anim : m_anims)
        {
            if (t_context.layout.wholeRect().intersects(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
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

    std::size_t WaterAnimationLayer::frameCount() const noexcept
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

    sf::IntRect WaterAnimationLayer::textureRect(const std::size_t t_frame) const noexcept
    {
        sf::IntRect rect;
        rect.width  = static_cast<int>(m_texture.getSize().y);
        rect.height = rect.width;
        rect.top    = 0;
        rect.left   = (static_cast<int>(t_frame) * rect.width);
        return rect;
    }

    void WaterAnimationLayer::update(Context &, const float t_frameTimeSec)
    {
        for (WaterAnim & anim : m_anims)
        {
            anim.elapsed_time_sec += t_frameTimeSec;
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

    Harm WaterAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        for (const WaterAnim & anim : m_anims)
        {
            const sf::FloatRect waterRect{ anim.sprite.getGlobalBounds() };
            if (t_avatarRect.intersects(waterRect))
            {
                harm.rect   = waterRect;
                harm.damage = 0;
                harm.sfx    = "dunk";
                break;
            }
        }

        return harm;
    }

} // namespace bramblefore
