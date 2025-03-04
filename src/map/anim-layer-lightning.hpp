#ifndef ANIM_LAYER_LIGHTNING_HPP_INCLUDED
#define ANIM_LAYER_LIGHTNING_HPP_INCLUDED
//
// anim-layer-lightning.hpp
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

    struct LightningAnim
    {
        explicit LightningAnim(const sf::Texture & t_texture)
            : sprite{ t_texture }
            , is_animating{ false }
            , frame_index{ 0 }
            , elapsed_time_sec{ 0.0f }
            , time_between_anim_sec{ 0.0f }
        {}

        sf::Sprite sprite;
        bool is_animating;
        std::size_t frame_index;
        float elapsed_time_sec;
        float time_between_anim_sec;
    };

    //

    class LightningAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        LightningAnimationLayer(Context & t_context, const std::vector<sf::FloatRect> & t_rects);

        virtual ~LightningAnimationLayer() override;

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

        [[nodiscard]] Harm
            avatarCollide(Context & t_context, const sf::FloatRect & t_avatarRect) override;

      private:
        [[nodiscard]] sf::IntRect textureRect(const std::size_t frame) const noexcept;

      private:
        sf::Texture m_texture;
        std::vector<LightningAnim> m_anims;
        float m_timebetweenFrames;
        std::size_t m_frameCount;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_LIGHTNING_HPP_INCLUDED
