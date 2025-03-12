// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-firewall.cpp
//
#include "anim-layer-firewall.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    SmokeParticle::SmokeParticle(
        const Context & t_context, const sf::Texture & t_texture, const sf::Vector2f & t_position)
        : sprite{ t_texture }
        , velocity{ 0.0f, 0.0f }
        , rotation_speed{ t_context.random.fromTo(50.0f, 100.0f) }
        , shrink_scale{ t_context.random.fromTo(0.95f, 0.99f) }
        , age_sec{ 0.0f }
        , age_limit_sec{ t_context.random.fromTo(2.0f, 3.0f) }
    {
        util::setOriginToCenter(sprite);

        const float scale{ t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.random.fromTo(0.1f, 0.25f)) };

        sprite.setScale({ scale, scale });
        sprite.rotate(sf::degrees(t_context.random.fromTo(0.0f, 360.0f)));
        sprite.setPosition(t_position);
        sprite.setColor(sf::Color(255, 255, 255, 0));
    }

    //

    SmokeEffect::SmokeEffect(const sf::FloatRect & t_region)
        : region{ t_region }
        , particles{}
    {
        particles.reserve(64);
    }

    //

    FirewallAnimationLayer::FirewallAnimationLayer(
        const Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_fireTextures{}
        , m_fireSprites{}
        , m_fireElapsedTimeSec{ 0.0f }
        , m_fireTimePerFrameSec{ 0.1f }
        , m_fireFrameIndex{ 0 }
        , m_smokeEffects{}
        , m_smokeElapsedTimeSec{ 0.0f }
        , m_smokeTimePerSpawnSec{ 0.2f }
        , m_smokeTextures{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        // load smoke particle images
        const std::size_t smokeTextureCount{ 10 };
        m_smokeTextures.resize(smokeTextureCount);
        for (std::size_t index{ 0 }; index < smokeTextureCount; ++index)
        {
            std::string pathStr{
                (t_context.settings.media_path / "image/particle/smoke/smoke").string()
            };

            pathStr += std::to_string(index);
            pathStr += ".png";

            util::TextureLoader::load(m_smokeTextures.at(index), pathStr);
        }

        // load fire animation images
        m_fireTextures.resize(4); // prevent reallocations

        util::TextureLoader::load(
            m_fireTextures.at(0),
            (t_context.settings.media_path / "image/anim/firewall-1-center.png"));

        util::TextureLoader::load(
            m_fireTextures.at(1),
            (t_context.settings.media_path / "image/anim/firewall-2-center.png"));

        util::TextureLoader::load(
            m_fireTextures.at(2),
            (t_context.settings.media_path / "image/anim/firewall-3-center.png"));

        util::TextureLoader::load(
            m_fireTextures.at(3),
            (t_context.settings.media_path / "image/anim/firewall-4-center.png"));

        // setup firewall animated sprites
        m_fireSprites.reserve(t_rects.size());
        for (const sf::FloatRect & rect : t_rects)
        {
            sf::Sprite & sprite{ m_fireSprites.emplace_back(m_fireTextures.at(0)) };
            util::fitAndCenterInside(sprite, rect);
        }

        // setup smoke effects
        m_smokeEffects.reserve(t_rects.size());
        for (const sf::FloatRect & rect : t_rects)
        {
            m_smokeEffects.emplace_back(rect);
        }
    }

    FirewallAnimationLayer::~FirewallAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void FirewallAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const SmokeEffect & smokeEffect : m_smokeEffects)
        {
            if (wholeScreenRect.findIntersection(smokeEffect.region))
            {
                for (const SmokeParticle & particle : smokeEffect.particles)
                {
                    t_target.draw(particle.sprite, t_states);
                }
            }
        }

        for (const sf::Sprite & sprite : m_fireSprites)
        {
            if (wholeScreenRect.findIntersection(sprite.getGlobalBounds()))
            {
                t_target.draw(sprite, t_states);
            }
        }
    }

    void FirewallAnimationLayer::move(const Context &, const float t_amount)
    {
        for (sf::Sprite & sprite : m_fireSprites)
        {
            sprite.move({ t_amount, 0.0f });
        }

        for (SmokeEffect & smokeEffect : m_smokeEffects)
        {
            smokeEffect.region.position.x += t_amount;

            for (SmokeParticle & particle : smokeEffect.particles)
            {
                particle.sprite.move({ t_amount, 0.0f });
            }
        }
    }

    void FirewallAnimationLayer::update(Context & t_context, const float t_frameTimeSec)
    {
        // animate firewall sprites
        m_fireElapsedTimeSec += t_frameTimeSec;
        if (m_fireElapsedTimeSec > m_fireTimePerFrameSec)
        {
            m_fireElapsedTimeSec -= m_fireTimePerFrameSec;

            ++m_fireFrameIndex;
            if (m_fireFrameIndex >= 4)
            {
                m_fireFrameIndex = 0;
            }

            for (sf::Sprite & sprite : m_fireSprites)
            {
                sprite.setTexture(m_fireTextures.at(m_fireFrameIndex));
            }
        }

        // spawn smoke particles
        m_smokeElapsedTimeSec += t_frameTimeSec;
        if (m_smokeElapsedTimeSec > m_smokeTimePerSpawnSec)
        {
            m_smokeElapsedTimeSec -= m_smokeTimePerSpawnSec;

            for (SmokeEffect & smokeEffect : m_smokeEffects)
            {
                const sf::Vector2f position{
                    t_context.random.fromTo(
                        smokeEffect.region.position.x, util::right(smokeEffect.region)),
                    (smokeEffect.region.position.y + (smokeEffect.region.size.y * 0.2f))
                };

                smokeEffect.particles.emplace_back(
                    t_context, t_context.random.from(m_smokeTextures), position);
            }
        }

        // animate smoke particles
        for (SmokeEffect & smokeEffect : m_smokeEffects)
        {
            smokeEffect.update(t_context, t_frameTimeSec);
        }
    }

    void SmokeEffect::update(Context &, const float t_frameTimeSec)
    {
        for (SmokeParticle & particle : particles)
        {
            particle.age_sec += t_frameTimeSec;

            const float ageRatio{ particle.age_sec / particle.age_limit_sec };
            const float initialRatio{ 0.2f };
            if (ageRatio < initialRatio)
            {
                // alpha fades in initially
                const float ageRatioInitial{ particle.age_sec / initialRatio };
                const float alphaRange{ 127 };
                const std::uint8_t alpha{ static_cast<std::uint8_t>(alphaRange * ageRatioInitial) };
                particle.sprite.setColor(sf::Color(255, 255, 255, alpha));
            }
            else
            {
                // alpha fades out after initial fade in
                const float alphaRange{ 100 };
                const std::uint8_t alphaShift{ static_cast<std::uint8_t>(alphaRange * ageRatio) };
                particle.sprite.setColor(sf::Color(255, 255, 255, (127 - alphaShift)));
            }

            particle.velocity.y -= 1.0f;
            particle.sprite.move(particle.velocity * t_frameTimeSec);

            particle.sprite.rotate(sf::degrees(particle.rotation_speed * t_frameTimeSec));

            particle.sprite.scale({ particle.shrink_scale, particle.shrink_scale });
        }

        std::erase_if(particles, [](const SmokeParticle & p) { return !p.isAlive(); });
    }

    Harm FirewallAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        for (const sf::Sprite & sprite : m_fireSprites)
        {
            const sf::FloatRect fireRect{ sprite.getGlobalBounds() };
            if (t_avatarRect.findIntersection(fireRect))
            {
                harm.rect   = fireRect;
                harm.damage = 99999; // TOOD lookup real max health somewhere
                harm.sfx    = "fire-puff";
                break;
            }
        }

        return harm;
    }

    void FirewallAnimationLayer::dumpInfo() const
    {
        std::clog << "\tFirewall Animation Layer: x" << m_fireSprites.size() << '\n';
    }

} // namespace bramblefore
