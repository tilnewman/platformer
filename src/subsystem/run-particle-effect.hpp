#ifndef RUN_PARTICLE_EFFECT_HPP_INCLUDED
#define RUN_PARTICLE_EFFECT_HPP_INCLUDED
//
// run-particle-effect.hpp
//
#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace bramblefore
{

    struct Context;

    //

    struct RunParticle
    {
        explicit RunParticle(const Context & t_context, const sf::Texture & t_texture);

        [[nodiscard]] bool isAlive() const { return (sprite.getScale().x > 0.05f); }

        sf::Sprite sprite;
        sf::Vector2f velocity;
        float rotation_speed;
        float shrink_scale;
    };

    //

    class RunParticleEffect
    {
      public:
        RunParticleEffect();

        inline void start() noexcept { m_isSpawning = true; }
        inline void stop() noexcept { m_isSpawning = false; }
        void setup(const Context & t_context);
        void update(const Context & t_context, const float t_elapsedTimeSec);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        void move(const float t_amount);

      private:
        [[nodiscard]] const sf::Texture & randomStarTexture(const Context & t_context) const;

      private:
        bool m_isSpawning;
        sf::Texture m_star1Texture;
        sf::Texture m_star2Texture;
        sf::Texture m_star3Texture;
        sf::Texture m_star4Texture;
        sf::Texture m_star5Texture;
        sf::Texture m_star6Texture;
        std::vector<RunParticle> m_particles;
        float m_timePerSpawnSec;
        float m_elapsedTimeSec;
    };

} // namespace bramblefore

#endif // RUN_PARTICLE_EFFECT_HPP_INCLUDED
