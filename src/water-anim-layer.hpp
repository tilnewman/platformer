#ifndef WATER_ANIM_LAYER_HPP_INCLUDED
#define WATER_ANIM_LAYER_HPP_INCLUDED
//
// water-anim-layer.hpp
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

    struct WaterAnim
    {
        sf::Sprite sprite{};
        float elapsed_time_sec{ 0.0f };
        float time_per_frame_sec{ 0.0f };
        std::size_t frame_index{ 0 };
    };

    //

    class WaterAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        WaterAnimationLayer(Context & context, const std::vector<sf::FloatRect> & rects);
        virtual ~WaterAnimationLayer() override;

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

        Harm avatarCollide(Context & context, const sf::FloatRect & avatarRect) override;

      private:
        std::size_t frameCount() const;
        sf::IntRect textureRect(const std::size_t frame) const;

      private:
        sf::Texture m_texture;
        std::vector<WaterAnim> m_anims;
    };

} // namespace platformer

#endif // WATER_ANIM_LAYER_HPP_INCLUDED
