// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-acid-spout.cpp
//
#include "anim-layer-acid-spout.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "map/level.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    AcidSpoutAnimationLayer::AcidSpoutAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_scale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.5f) }
        , m_spoutTexture{}
        , m_dropTexture{}
        , m_splashTexture{}
        , m_spoutAnims{}
        , m_dropAnims{}
        , m_splashAnims{}
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
            anim.region             = rect;
            anim.time_between_drips = t_context.random.fromTo(1.0f, 4.0f);
            anim.sprite.setTexture(m_spoutTexture);
            anim.sprite.setTextureRect(textureRect(m_spoutTexture, 0));
            anim.sprite.scale(m_scale, m_scale);

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
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const AcidDropAnim & anim : m_dropAnims)
        {
            if (wholeScreenRect.intersects(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }

        for (const AcidSpoutAnim & anim : m_spoutAnims)
        {
            if (wholeScreenRect.intersects(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }

        for (const AcidSplashAnim & anim : m_splashAnims)
        {
            if (wholeScreenRect.intersects(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void AcidSpoutAnimationLayer::move(const Context &, const float t_amount)
    {
        for (AcidDropAnim & anim : m_dropAnims)
        {
            anim.sprite.move(t_amount, 0.0f);
            anim.region.left += t_amount;
        }

        for (AcidSpoutAnim & anim : m_spoutAnims)
        {
            anim.sprite.move(t_amount, 0.0f);
            anim.region.left += t_amount;
        }

        for (AcidSplashAnim & anim : m_splashAnims)
        {
            anim.sprite.move(t_amount, 0.0f);
        }
    }

    void AcidSpoutAnimationLayer::update(Context & t_context, const float t_frameTimeSec)
    {
        updateSpouts(t_frameTimeSec);
        updateDrops(t_context, t_frameTimeSec);
        updateSplashes(t_frameTimeSec);
    }

    void AcidSpoutAnimationLayer::updateSpouts(const float t_frameTimeSec)
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

                    AcidDropAnim & drop{ m_dropAnims.emplace_back() };
                    drop.region = anim.region;
                    drop.sprite.setTexture(m_dropTexture);
                    drop.sprite.scale(m_scale, m_scale);

                    drop.sprite.setPosition(
                        (util::center(anim.region).x -
                         (drop.sprite.getGlobalBounds().width * 0.5f)),
                        anim.region.top);
                }
            }
        }
    }

    void AcidSpoutAnimationLayer::updateDrops(const Context & t_context, const float t_frameTimeSec)
    {
        bool didAnyDropsLand{ false };
        for (AcidDropAnim & anim : m_dropAnims)
        {
            anim.velocity += (t_frameTimeSec * 50.0f);
            anim.sprite.move(0.0f, anim.velocity);

            if (util::bottom(anim.sprite) > util::bottom(anim.region))
            {
                didAnyDropsLand = true;
                anim.is_alive   = false;

                AcidSplashAnim & splash{ m_splashAnims.emplace_back() };
                splash.sprite.setTexture(m_splashTexture);
                splash.sprite.setTextureRect(textureRect(m_splashTexture, 0));

                const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.0f) };
                splash.sprite.scale(scale, scale);

                splash.sprite.setPosition(
                    (util::center(anim.region).x - (splash.sprite.getGlobalBounds().width * 0.5f)),
                    (util::bottom(anim.region) - (splash.sprite.getGlobalBounds().height * 0.7f)));

                if (t_context.layout.wholeRect().intersects(splash.sprite.getGlobalBounds()))
                {
                    t_context.sfx.play("splat");
                }
            }
        }

        if (didAnyDropsLand)
        {
            std::erase_if(m_dropAnims, [](const AcidDropAnim & anim) { return !anim.is_alive; });
        }
    }

    void AcidSpoutAnimationLayer::updateSplashes(const float t_frameTimeSec)
    {
        bool didAnySplashAnimsFinish{ false };
        for (AcidSplashAnim & anim : m_splashAnims)
        {
            anim.elapsed_time_sec += t_frameTimeSec;
            if (anim.elapsed_time_sec > anim.time_between_frames_sec)
            {
                anim.elapsed_time_sec -= anim.time_between_frames_sec;

                ++anim.frame_index;
                if (anim.frame_index >= frameCount(m_splashTexture))
                {
                    anim.frame_index        = 0;
                    anim.is_alive           = false;
                    didAnySplashAnimsFinish = true;
                }

                anim.sprite.setTextureRect(textureRect(m_splashTexture, anim.frame_index));
            }
        }

        if (didAnySplashAnimsFinish)
        {
            std::erase_if(
                m_splashAnims, [](const AcidSplashAnim & anim) { return !anim.is_alive; });
        }
    }

    std::size_t AcidSpoutAnimationLayer::frameCount(const sf::Texture & t_texture) const noexcept
    {
        if (t_texture.getSize().y > 0)
        {
            return static_cast<std::size_t>(t_texture.getSize().x / t_texture.getSize().y);
        }
        else
        {
            return 0;
        }
    }

    sf::IntRect AcidSpoutAnimationLayer::textureRect(
        const sf::Texture & t_texture, const std::size_t frame) const noexcept
    {
        sf::IntRect rect;
        rect.width  = static_cast<int>(t_texture.getSize().y);
        rect.height = rect.width;
        rect.top    = 0;
        rect.left   = (static_cast<int>(frame) * rect.width);
        return rect;
    }

    Harm AcidSpoutAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        for (const AcidDropAnim & anim : m_dropAnims)
        {
            const sf::FloatRect acidRect{ anim.sprite.getGlobalBounds() };
            if (t_avatarRect.intersects(acidRect))
            {
                return makeHarm(acidRect);
            }
        }

        for (const AcidSplashAnim & anim : m_splashAnims)
        {
            const sf::FloatRect acidRect{ util::scaleRectInPlaceCopy(
                anim.sprite.getGlobalBounds(), 0.7f) };

            if (t_avatarRect.intersects(acidRect))
            {
                return makeHarm(acidRect);
            }
        }

        return Harm{};
    }

    Harm AcidSpoutAnimationLayer::makeHarm(const sf::FloatRect & t_rect) const noexcept
    {
        Harm harm;
        harm.rect   = t_rect;
        harm.damage = 10;
        harm.sfx    = "acid";
        return harm;
    }

} // namespace bramblefore
