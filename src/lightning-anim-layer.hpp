#ifndef LIGHTNING_ANIM_LAYER_HPP_INCLUDED
#define LIGHTNING_ANIM_LAYER_HPP_INCLUDED
//
// lightning-anim-layer.hpp
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
        LightningAnimationLayer(Context & context, const std::vector<sf::FloatRect> & rects);
        virtual ~LightningAnimationLayer() override;

        void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const override;
        void move(const Context & context, const float amount) override;
        float findFarthestHorizVert() const override { return 0.0f; }
        void dumpInfo() const override {}

        void update(Context & context, const float frameTimeSec) override;

        void appendVertLayer(
            const Context &,
            const sf::Vector2f &,
            const sf::Vector2i &,
            const sf::Vector2i &,
            const sf::Vector2f &) override
        {}

        const Harm avatarCollide(Context & context, const sf::FloatRect & avatarRect) override;

      private:
        const sf::IntRect textureRect(const std::size_t frame) const;

      private:
        sf::Texture m_texture;
        std::vector<LightningAnim> m_anims;
        float m_timebetweenFrames;
        std::size_t m_frameCount;
    };

} // namespace platformer

#endif // LIGHTNING_ANIM_LAYER_HPP_INCLUDED
