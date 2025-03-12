#ifndef ANIM_LAYER_FIREWALL_HPP_INCLUDED
#define ANIM_LAYER_FIREWALL_HPP_INCLUDED
//
// anim-layer-firewall.hpp
//
#include "map/tile-layer.hpp"
#include "subsystem/harm-collision-manager.hpp"

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sf
{
    class RenderTarget;
} // namespace sf

namespace bramblefore
{

    struct Context;

    //

    struct SmokeParticle
    {
        explicit SmokeParticle(
            const Context & t_context,
            const sf::Texture & t_texture,
            const sf::Vector2f & t_position);

        [[nodiscard]] bool isAlive() const { return (age_sec < age_limit_sec); }

        sf::Sprite sprite;
        sf::Vector2f velocity;
        float rotation_speed;
        float shrink_scale;
        float age_sec;
        float age_limit_sec;
    };

    //

    struct SmokeEffect
    {
        explicit SmokeEffect(const sf::FloatRect & t_region);

        void update(Context & t_context, const float t_frameTimeSec);

        sf::FloatRect region;
        std::vector<SmokeParticle> particles;
    };

    //

    class FirewallAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        FirewallAnimationLayer(
            const Context & t_context, const std::vector<sf::FloatRect> & t_rects);

        virtual ~FirewallAnimationLayer() final;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void move(const Context & t_context, const float t_amount) final;
        void dumpInfo() const final;
        void update(Context & t_context, const float t_frameTimeSec) final;

        void appendVertLayer(
            const Context &,
            const sf::Vector2f &,
            const sf::Vector2i &,
            const sf::Vector2i &,
            const sf::Vector2f &) final
        {}

        Harm avatarCollide(Context & t_context, const sf::FloatRect & t_avatarRect) final;

      private:
        std::vector<sf::Texture> m_fireTextures;
        std::vector<sf::Sprite> m_fireSprites;
        float m_fireElapsedTimeSec;
        float m_fireTimePerFrameSec;
        std::size_t m_fireFrameIndex;
        std::vector<SmokeEffect> m_smokeEffects;
        float m_smokeElapsedTimeSec;
        float m_smokeTimePerSpawnSec;
        std::vector<sf::Texture> m_smokeTextures;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_FIREWALL_HPP_INCLUDED
