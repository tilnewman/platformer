#ifndef ANIM_LAYER_SAW_HPP_INCLUDED
#define ANIM_LAYER_SAW_HPP_INCLUDED
//
// anim-layer-saw.hpp
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

    class SawAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        SawAnimationLayer(const Context & t_context, const std::vector<sf::FloatRect> & t_rects);
        virtual ~SawAnimationLayer() final;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void move(const Context & t_context, const float t_amount) final;
        void dumpInfo() const final;
        void update(Context & t_context, const float t_frameTimeSec) final;

        inline void appendVertLayer(
            const Context &,
            const sf::Vector2f &,
            const sf::Vector2i &,
            const sf::Vector2i &,
            const sf::Vector2f &) final
        {}

        Harm avatarCollide(Context & t_context, const sf::FloatRect & t_avatarRect) final;

      private:
        sf::Texture m_texture;
        std::vector<sf::Sprite> m_anims;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_SAW_HPP_INCLUDED
