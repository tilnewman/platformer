#ifndef ANIM_LAYER_SPIKED_BALL_HPP_INCLUDED
#define ANIM_LAYER_SPIKED_BALL_HPP_INCLUDED
//
// anim-layer-spiked-ball.hpp
//
#include "map/tile-layer.hpp"
#include "subsystem/harm-collision-manager.hpp"
#include "util/sliders.hpp"

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

    struct SpikedBallAnim
    {
        explicit SpikedBallAnim(
            const sf::Texture & t_texture, const sf::FloatRect & t_rect, const float t_speed);

        [[nodiscard]] inline bool isHoriz() const { return (region.size.x > region.size.y); }

        sf::Sprite sprite;
        sf::FloatRect region;
        util::SliderOscillator<float, float> slider;
    };

    //

    class SpikedBallAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        SpikedBallAnimationLayer(Context & t_context, const std::vector<sf::FloatRect> & t_rects);
        virtual ~SpikedBallAnimationLayer() final;

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
        float m_speed;
        sf::Texture m_texture;
        std::vector<SpikedBallAnim> m_anims;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_SPIKED_BALL_HPP_INCLUDED
