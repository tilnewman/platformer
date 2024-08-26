#ifndef ANIM_LAYER_LIGHTNING_HPP_INCLUDED
#define ANIM_LAYER_LIGHTNING_HPP_INCLUDED
//
// anim-layer-lightning.hpp
//
#include "harm-collision-manager.hpp"
#include "tile-layer.hpp"

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
} // namespace sf

namespace platformer
{

    struct Context;

    //

    struct LightningAnim
    {
        sf::Sprite sprite{};
        bool is_animating{ false };
        std::size_t frame_index{ 0 };
        float elapsed_time_sec{ 0.0f };
        float time_between_anim_sec{ 0.0f };
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
        float findFarthestHorizVert() const override { return 0.0f; }
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

} // namespace platformer

#endif // ANIM_LAYER_LIGHTNING_HPP_INCLUDED
