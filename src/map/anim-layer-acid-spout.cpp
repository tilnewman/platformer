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
#include "util/check-macros.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    AcidSplashAnim::AcidSplashAnim(
        const Context & t_context,
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const sf::FloatRect & t_screenRect)
        : is_alive{ true }
        , elapsed_time_sec{ 0.0f }
        , time_between_frames_sec{ 0.1f }
        , frame_index{ 0 }
        , sprite{ t_texture, t_textureRect }
    {
        const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.0f) };
        sprite.scale({ scale, scale });

        sprite.setPosition(
            { (util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
              (util::bottom(t_screenRect) - (sprite.getGlobalBounds().size.y * 0.7f)) });
    }

    //

    AcidDropAnim::AcidDropAnim(
        const sf::Texture & t_texture,
        const sf::FloatRect & t_screenRect,
        const sf::Vector2f & t_scale)
        : is_alive{ true }
        , velocity{ 0.0f }
        , sprite{ t_texture }
        , region{ t_screenRect }
    {
        sprite.setScale(t_scale);

        sprite.setPosition({ (util::center(region).x - (sprite.getGlobalBounds().size.x * 0.5f)),
                             region.position.y });
    }

    //

    AcidSpoutAnim::AcidSpoutAnim(
        const Context & t_context,
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const sf::FloatRect & t_screenRect,
        const sf::Vector2f & t_scale)
        : is_dripping{ false }
        , elapsed_time_sec{ 0.0f }
        , time_between_drip_sec{ t_context.random.fromTo(1.0f, 4.0f) }
        , time_between_frames_sec{ 0.15f }
        , frame_index{ 0 }
        , sprite{ t_texture, t_textureRect }
        , region{ t_screenRect }
    {
        sprite.setScale(t_scale);

        sprite.setPosition(
            { (util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
              t_screenRect.position.y });
    }

    //

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

        //

        util::TextureLoader::load(
            m_spoutTexture, (t_context.settings.media_path / "image/anim/acid-spout.png"));

        util::TextureLoader::load(
            m_dropTexture, (t_context.settings.media_path / "image/anim/acid-spout-drop.png"));

        util::TextureLoader::load(
            m_splashTexture, (t_context.settings.media_path / "image/anim/acid-spout-splash.png"));

        //

        m_spoutAnims.reserve(t_rects.size());
        for (const sf::FloatRect & rect : t_rects)
        {
            m_spoutAnims.emplace_back(
                t_context,
                m_spoutTexture,
                textureRect(m_spoutTexture, 0),
                rect,
                sf::Vector2f{ m_scale, m_scale });
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
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }

        for (const AcidSpoutAnim & anim : m_spoutAnims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }

        for (const AcidSplashAnim & anim : m_splashAnims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void AcidSpoutAnimationLayer::move(const Context &, const float t_amount)
    {
        for (AcidDropAnim & anim : m_dropAnims)
        {
            anim.sprite.move({ t_amount, 0.0f });
            anim.region.position.x += t_amount;
        }

        for (AcidSpoutAnim & anim : m_spoutAnims)
        {
            anim.sprite.move({ t_amount, 0.0f });
            anim.region.position.x += t_amount;
        }

        for (AcidSplashAnim & anim : m_splashAnims)
        {
            anim.sprite.move({ t_amount, 0.0f });
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
                if (anim.elapsed_time_sec > anim.time_between_drip_sec)
                {
                    anim.elapsed_time_sec = 0.0f;
                    anim.is_dripping      = true;

                    m_dropAnims.emplace_back(
                        m_dropTexture, anim.region, sf::Vector2f{ m_scale, m_scale });
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
            anim.sprite.move({ 0.0f, anim.velocity });

            if (util::bottom(anim.sprite) > util::bottom(anim.region))
            {
                didAnyDropsLand = true;
                anim.is_alive   = false;

                AcidSplashAnim & splash{ m_splashAnims.emplace_back(
                    t_context, m_splashTexture, textureRect(m_splashTexture, 0), anim.region) };

                if (t_context.layout.wholeRect().findIntersection(splash.sprite.getGlobalBounds()))
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
        rect.size.x     = static_cast<int>(t_texture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(frame) * rect.size.x);

        return rect;
    }

    Harm AcidSpoutAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        for (const AcidDropAnim & anim : m_dropAnims)
        {
            const sf::FloatRect acidRect{ anim.sprite.getGlobalBounds() };
            if (t_avatarRect.findIntersection(acidRect))
            {
                return makeHarm(acidRect);
            }
        }

        for (const AcidSplashAnim & anim : m_splashAnims)
        {
            const sf::FloatRect acidRect{ util::scaleRectInPlaceCopy(
                anim.sprite.getGlobalBounds(), 0.7f) };

            if (t_avatarRect.findIntersection(acidRect))
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
