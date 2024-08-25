#ifndef GHOST_BOTTLE_ANIM_LAYER_HPP_INCLUDED
#define GHOST_BOTTLE_ANIM_LAYER_HPP_INCLUDED
//
// ghost-bottle-anim-layer.hpp
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

    struct GhostBottleAnim
    {
        bool has_emerged{ false };
        float elapsed_time_sec{ 0.0f };
        float time_between_frames_sec{ 0.175f };
        std::size_t frame_index{ 0 };
        sf::Sprite sprite{};
        sf::FloatRect coll_rect{};
    };

    //

    class GhostBottleAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        GhostBottleAnimationLayer(Context & t_context, const std::vector<sf::FloatRect> & t_rects);
        virtual ~GhostBottleAnimationLayer() override;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void move(const Context & t_context, const float t_amount) final;
        inline float findFarthestHorizVert() const final { return 0.0f; }
        inline void dumpInfo() const final {}

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
        void updateSpouts(const float t_frameTimeSec);
        void updateDrops(const float t_frameTimeSec);
        void updateSplashes(const float t_frameTimeSec);

        [[nodiscard]] std::size_t frameCount() const;
        [[nodiscard]] sf::IntRect textureRect(const std::size_t t_frame) const;

      private:
        sf::Texture m_texture;
        std::vector<GhostBottleAnim> m_anims;
    };

} // namespace platformer

#endif // GHOST_BOTTLE_ANIM_LAYER_HPP_INCLUDED
