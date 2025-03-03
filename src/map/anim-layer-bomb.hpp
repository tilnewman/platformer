#ifndef ANIM_LAYER_BOMB_HPP_INCLUDED
#define ANIM_LAYER_BOMB_HPP_INCLUDED
//
// anim-layer-bomb.hpp
//
#include "map/tile-layer.hpp"
#include "subsystem/harm-collision-manager.hpp"
#include "util/sfml-defaults.hpp"

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

    struct BombAnim
    {
        bool has_exploded{ false };
        float elapsed_time_sec{ 0.0f };
        float time_between_frames_sec{ 0.1f };
        std::size_t frame_index{ 0 };
        sf::Sprite sprite{ util::SfmlDefaults::instance().texture() };

        // the rect drawn on the map is the collision rect
        sf::FloatRect coll_rect{};
    };

    //

    class BombAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        BombAnimationLayer(Context & t_context, const std::vector<sf::FloatRect> & t_rects);
        virtual ~BombAnimationLayer() override;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void move(const Context & t_context, const float t_amount) final;
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
        [[nodiscard]] std::size_t frameCount() const noexcept;
        [[nodiscard]] sf::IntRect textureRect(const std::size_t t_frame) const noexcept;

      private:
        sf::Texture m_texture;
        std::vector<BombAnim> m_anims;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_BOMB_HPP_INCLUDED
