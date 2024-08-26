#ifndef ANIM_LAYER_ACID_HPP_INCLUDED
#define ANIM_LAYER_ACID_HPP_INCLUDED
//
// anim-layer-acid.hpp
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

    class AcidAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        AcidAnimationLayer(Context & t_context, const std::vector<sf::FloatRect> & t_rects);
        virtual ~AcidAnimationLayer() override;

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

        Harm avatarCollide(Context & t_context, const sf::FloatRect & t_avatarRect) override;

      private:
        [[nodiscard]] std::size_t frameCount() const noexcept;
        [[nodiscard]] sf::IntRect textureRect(const std::size_t t_frame) const noexcept;

      private:
        sf::Texture m_texture;
        std::vector<sf::Sprite> m_sprites;
        float m_elapsedTimeSec;
        float m_timePerFrameSec;
        std::size_t m_frameIndex;
    };

} // namespace platformer

#endif // ANIM_LAYER_ACID_HPP_INCLUDED
