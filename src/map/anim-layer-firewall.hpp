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

    class FirewallAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        FirewallAnimationLayer(Context & t_context, const std::vector<sf::FloatRect> & t_rects);
        virtual ~FirewallAnimationLayer() override;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const override;

        void move(const Context & t_context, const float t_amount) override;
        void dumpInfo() const override {}

        void update(Context & t_context, const float t_frameTimeSec) override;

        void appendVertLayer(
            const Context &,
            const sf::Vector2f &,
            const sf::Vector2i &,
            const sf::Vector2i &,
            const sf::Vector2f &) override
        {}

        Harm avatarCollide(Context & t_context, const sf::FloatRect & t_avatarRect) override;

      private:
        std::vector<sf::Texture> m_textures;
        std::vector<sf::Sprite> m_sprites;
        float m_elapsedTimeSec;
        float m_timePerFrameSec;
        std::size_t m_frameIndex;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_FIREWALL_HPP_INCLUDED
