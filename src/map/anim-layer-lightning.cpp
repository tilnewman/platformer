// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-lightning.cpp
//
#include "anim-layer-lightning.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    SparkParticle::SparkParticle(
        const Context & t_context, const sf::Texture & t_texture, const sf::Vector2f & t_position)
        : sprite{ t_texture }
        , velocity{ t_context.random.fromTo(-40.0f, 40.0f),
                    t_context.random.fromTo(-400.0f, -300.0f) }
        , rotation_speed{ t_context.random.fromTo(50.0f, 100.0f) }
        , scale_ratio{ t_context.random.fromTo(0.95f, 0.99f) }
        , age_sec{ 0.0f }
        , age_limit_sec{ t_context.random.fromTo(0.8f, 1.2f) }
    {
        const float scale{ t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.random.fromTo(0.1f, 0.25f)) };

        sprite.setScale({ scale, scale });
        util::setOriginToCenter(sprite);
        sprite.rotate(sf::degrees(t_context.random.fromTo(0.0f, 360.0f)));
        sprite.setPosition(t_position);
        sprite.setColor(sf::Color(200,255,255));
    }

    //

    LightningAnim::LightningAnim(
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const float t_scale,
        const float t_timeBetweenAnimSec,
        const sf::FloatRect & t_screenRect)
        : sprite{ t_texture, t_textureRect }
        , is_animating{ false }
        , frame_index{ 0 }
        , elapsed_time_sec{ 0.0f }
        , time_between_anim_sec{ t_timeBetweenAnimSec }
        , particles{}
    {
        sprite.scale({ t_scale, t_scale });

        sprite.setPosition({ (util::center(t_screenRect).x - sprite.getGlobalBounds().size.x),
                             (util::bottom(t_screenRect) - sprite.getGlobalBounds().size.y) });

        particles.reserve(32);
    }

    //

    LightningAnimationLayer::LightningAnimationLayer(
        const Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_lightningTexture{}
        , m_anims{}
        , m_timebetweenFrames{ 0.1f }
        , m_frameCount{ 8 }
        , m_sparkTexture{}
        , m_sparkElapsedTimeSec{ 0.0f }
        , m_sparkTimePerSpawnSec{ 0.05f }
    {
        HarmCollisionManager::instance().addOwner(*this);

        util::TextureLoader::load(
            m_sparkTexture,
            (t_context.settings.media_path / "image/particle/spark/spark-small.png"));

        util::TextureLoader::load(
            m_lightningTexture, (t_context.settings.media_path / "image/anim/lightning.png"));

        m_anims.reserve(t_rects.size());
        for (const sf::FloatRect & rect : t_rects)
        {
            m_anims.emplace_back(
                m_lightningTexture,
                textureRect(0),
                t_context.layout.calScaleBasedOnResolution(t_context, 1.0f),
                t_context.random.fromTo(2.0f, 6.0f),
                rect);
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

                t_states.blendMode = sf::BlendAdd;
                for (const SparkParticle & particle : anim.particles)
                {
                    t_target.draw(particle.sprite, t_states);
                }
            }
        }
    }

    void LightningAnimationLayer::move(const Context &, const float t_amount)
    {
        for (LightningAnim & anim : m_anims)
        {
            anim.sprite.move({ t_amount, 0.0f });

            for (SparkParticle & particle : anim.particles)
            {
                particle.sprite.move({ t_amount, 0.0f });
            }
        }
    }

    sf::IntRect LightningAnimationLayer::textureRect(const std::size_t t_frame) const noexcept
    {
        sf::IntRect rect;
        rect.size.x     = 64;
        rect.size.y     = static_cast<int>(m_lightningTexture.getSize().y);
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(t_frame) * rect.size.x);

        return rect;
    }

    void LightningAnimationLayer::update(Context & t_context, const float t_frameTimeSec)
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        // lightning animation
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

        // spawn spark particles
        m_sparkElapsedTimeSec += t_frameTimeSec;
        if (m_sparkElapsedTimeSec > m_sparkTimePerSpawnSec)
        {
            m_sparkElapsedTimeSec -= m_sparkTimePerSpawnSec;

            for (LightningAnim & anim : m_anims)
            {
                if (anim.is_animating)
                {
                    const sf::Vector2f position{ util::center(anim.sprite).x,
                                                 util::bottom(anim.sprite) };

                    anim.particles.emplace_back(t_context, m_sparkTexture, position);
                }
            }
        }

        // spark particles animation
        for (LightningAnim & anim : m_anims)
        {
            for (SparkParticle & particle : anim.particles)
            {
                particle.age_sec += t_frameTimeSec;
                particle.velocity.y += 10.0f;
                particle.sprite.move(particle.velocity * t_frameTimeSec);
                particle.sprite.rotate(sf::degrees(particle.rotation_speed * t_frameTimeSec));
                particle.sprite.scale({ particle.scale_ratio, particle.scale_ratio });
            }

            std::erase_if(anim.particles, [](const SparkParticle & p) { return !p.isAlive(); });
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

    void LightningAnimationLayer::dumpInfo() const
    {
        std::clog << "\tLightning Trap Animation Layer: x" << m_anims.size() << '\n';
    }

} // namespace bramblefore
