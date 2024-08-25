#ifndef TRAP_ANIM_LAYER_HPP_INCLUDED
#define TRAP_ANIM_LAYER_HPP_INCLUDED
//
// trap-anim-layer.hpp
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

    struct TrapAnim
    {
        bool is_resetting{ false };
        bool is_springing{ false };
        float elapsed_time_sec{ 0.0f };
        float time_between_springs_sec{ 2.0f };
        float time_between_frames_sec{ 0.1f };
        std::size_t frame_index{ 0 };
        sf::Sprite sprite{};
        sf::FloatRect spring_rect{};
        sf::FloatRect coll_rect{};
    };

    //

    class TrapAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        TrapAnimationLayer(Context & t_context, const std::vector<sf::FloatRect> & t_rects);
        virtual ~TrapAnimationLayer() override;

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
        std::vector<TrapAnim> m_anims;
    };

} // namespace platformer

#endif // TRAP_ANIM_LAYER_HPP_INCLUDED
