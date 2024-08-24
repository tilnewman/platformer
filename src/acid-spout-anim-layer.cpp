// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// acid-spout-anim-layer.cpp
//
#include "acid-spout-anim-layer.hpp"

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

    AcidSpoutAnimationLayer::AcidSpoutAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_spoutTexture{}
        , m_dropTexture{}
        , m_splashTexture{}
        , m_spoutAnims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        m_spoutTexture.loadFromFile(
            (t_context.settings.media_path / "image/map-anim/acid-spout.png").string());

        TextureStats::instance().process(m_spoutTexture);

        m_dropTexture.loadFromFile(
            (t_context.settings.media_path / "image/map-anim/acid-spout-drop.png").string());

        TextureStats::instance().process(m_dropTexture);

        m_splashTexture.loadFromFile(
            (t_context.settings.media_path / "image/map-anim/acid-spout-splash.png").string());

        TextureStats::instance().process(m_splashTexture);

        for (const sf::FloatRect & rect : t_rects)
        {
            AcidSpoutAnim & anim{ m_spoutAnims.emplace_back() };
            anim.time_between_drips = t_context.random.fromTo(1.0f, 4.0f);
            anim.sprite.setTexture(m_spoutTexture);
            anim.sprite.setTextureRect(textureRect(m_spoutTexture, 0));
            anim.sprite.scale(1.5f, 1.5f);

            anim.sprite.setPosition(
                (util::center(rect).x - (anim.sprite.getGlobalBounds().width * 0.5f)), rect.top);
        }
    }

    AcidSpoutAnimationLayer::~AcidSpoutAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void AcidSpoutAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const AcidSpoutAnim & anim : m_spoutAnims)
        {
            if (t_context.layout.wholeRect().intersects(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void AcidSpoutAnimationLayer::move(const Context &, const float t_amount)
    {
        for (AcidSpoutAnim & anim : m_spoutAnims)
        {
            anim.sprite.move(t_amount, 0.0f);
        }
    }

    void AcidSpoutAnimationLayer::update(Context &, const float t_frameTimeSec) 
    {
        for (AcidSpoutAnim & anim : m_spoutAnims)
        {
            if (anim.is_dripping)
            {
                anim.elapsed_time_sec += t_frameTimeSec;
                if (anim.elapsed_time_sec > anim.time_between_frames_sec)
                {
                    anim.elapsed_time_sec -= anim.time_between_frames_sec;

                    ++anim.frame_index;
                    if (anim.frame_index >= frameCount(m_spoutTexture))
                    {
                        anim.frame_index      = 0;
                        anim.elapsed_time_sec = 0.0f;
                        anim.is_dripping      = false;
                    }

                    anim.sprite.setTextureRect(textureRect(m_spoutTexture, anim.frame_index));
                }
            }
            else
            {
                anim.elapsed_time_sec += t_frameTimeSec;
                if (anim.elapsed_time_sec > anim.time_between_drips)
                {
                    anim.elapsed_time_sec = 0.0f;
                    anim.is_dripping      = true;
                }
            }
            
        }
    }

    std::size_t AcidSpoutAnimationLayer::frameCount(const sf::Texture & t_texture) const
    {
        return static_cast<std::size_t>(t_texture.getSize().x / t_texture.getSize().y);
    }

    sf::IntRect AcidSpoutAnimationLayer::textureRect(
        const sf::Texture & t_texture, const std::size_t frame) const
    {
        sf::IntRect rect;
        rect.width  = static_cast<int>(t_texture.getSize().y);
        rect.height = rect.width;
        rect.top    = 0;
        rect.left   = (static_cast<int>(frame) * rect.width);

        return rect;
    }

    Harm AcidSpoutAnimationLayer::avatarCollide(Context &, const sf::FloatRect &)
    {
        Harm harm;

        // for (const sf::Sprite & sprite : m_sprites)
        // {
        //     const sf::FloatRect acidRect{ sprite.getGlobalBounds() };
        //     if (t_avatarRect.intersects(acidRect))
        //     {
        //         harm.rect   = acidRect;
        //         harm.damage = 99999; // TOOD lookup real max health somewhere
        //         harm.sfx    = "acid";
        //         break;
        //     }
        // }

        return harm;
    }

} // namespace platformer
