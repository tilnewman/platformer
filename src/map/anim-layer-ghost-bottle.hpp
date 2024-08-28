#ifndef ANIM_LAYER_GHOST_BOTTLE_HPP_INCLUDED
#define ANIM_LAYER_GHOST_BOTTLE_HPP_INCLUDED
//
// anim-layer-ghost-bottle.hpp
//
#include "map/tile-layer.hpp"
#include "subsystem/harm-collision-manager.hpp"

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
} // namespace sf

namespace bramblefore
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
        [[nodiscard]] std::size_t frameCount() const;
        [[nodiscard]] sf::IntRect textureRect(const std::size_t t_frame) const;

      private:
        sf::Texture m_texture;
        std::vector<GhostBottleAnim> m_anims;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_GHOST_BOTTLE_HPP_INCLUDED
