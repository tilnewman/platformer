// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// run-particle-effect.cpp
//
#include "run-particle-effect.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    Particle::Particle(const Context & t_context, const sf::Texture & t_texture)
        : sprite{ t_texture }
        , velocity{ 0.0f, 0.0f }
        , rotation_speed{ t_context.random.fromTo(50.0f, 100.0f) }
        , shrink_scale{ t_context.random.fromTo(0.95f, 0.99f) }
    {
        util::setOriginToCenter(sprite);

        const float scale{ t_context.random.fromTo(0.1f, 0.25f) };
        sprite.setScale({ scale, scale });

        sprite.rotate(sf::degrees(t_context.random.fromTo(0.0f, 360.0f)));

        const sf::FloatRect avatarRect{ t_context.avatar.collisionRect() };

        sprite.setPosition(
            { util::center(avatarRect).x,
              t_context.random.fromTo(avatarRect.position.y, util::bottom(avatarRect)) });

        sprite.setColor(sf::Color(245, 205, 150));
    }

    //

    RunParticleEffect::RunParticleEffect()
        : m_isSpawning{ false }
        , m_star1Texture{}
        , m_star2Texture{}
        , m_star3Texture{}
        , m_star4Texture{}
        , m_star5Texture{}
        , m_star6Texture{}
        , m_particles{}
        , m_timePerSpawnSec{ 0.05f }
        , m_elapsedTimeSec{ 0.0f }
    {}

    void RunParticleEffect::setup(const Context & t_context)
    {
        util::TextureLoader::load(
            m_star1Texture, (t_context.settings.media_path / "image/particle/star1.png"));

        util::TextureLoader::load(
            m_star2Texture, (t_context.settings.media_path / "image/particle/star2.png"));

        util::TextureLoader::load(
            m_star3Texture, (t_context.settings.media_path / "image/particle/star3.png"));

        util::TextureLoader::load(
            m_star4Texture, (t_context.settings.media_path / "image/particle/star4.png"));

        util::TextureLoader::load(
            m_star5Texture, (t_context.settings.media_path / "image/particle/star5.png"));

        util::TextureLoader::load(
            m_star6Texture, (t_context.settings.media_path / "image/particle/star6.png"));

        m_particles.reserve(64);
    }

    void RunParticleEffect::update(const Context & t_context, const float t_frameTimeSec)
    {
        m_elapsedTimeSec += t_frameTimeSec;
        if (m_elapsedTimeSec > m_timePerSpawnSec)
        {
            m_elapsedTimeSec -= m_timePerSpawnSec;

            if (m_isSpawning)
            {
                m_particles.emplace_back(t_context, randomStarTexture(t_context));
            }
        }

        for (Particle & particle : m_particles)
        {
            particle.velocity.y += 0.333f;
            particle.sprite.move(particle.velocity * t_frameTimeSec);
            particle.sprite.rotate(sf::degrees(particle.rotation_speed * t_frameTimeSec));
            particle.sprite.scale({ particle.shrink_scale, particle.shrink_scale });
        }

        std::erase_if(m_particles, [](const Particle & p) { return !p.isAlive(); });
    }

    void RunParticleEffect::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_states.blendMode = sf::BlendAdd;

        for (const Particle & particle : m_particles)
        {
            t_target.draw(particle.sprite, t_states);
        }
    }

    const sf::Texture & RunParticleEffect::randomStarTexture(const Context & t_context) const
    {
        const int selection{ t_context.random.fromTo(1, 6) };

        // clang-format off
        if      (selection == 1) return m_star1Texture;
        else if (selection == 2) return m_star2Texture;
        else if (selection == 3) return m_star3Texture;
        else if (selection == 4) return m_star4Texture;
        else if (selection == 5) return m_star5Texture;
        else                     return m_star6Texture;
        // clang-format on
    }

    void RunParticleEffect::move(const float t_amount)
    {
        for (Particle & particle : m_particles)
        {
            particle.sprite.move({ t_amount, 0.0f });
        }
    }

} // namespace bramblefore
