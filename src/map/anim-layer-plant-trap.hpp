#ifndef ANIM_LAYER_PLANT_TRAP_HPP_INCLUDED
#define ANIM_LAYER_PLANT_TRAP_HPP_INCLUDED
//
// anim-layer-plant-trap.hpp
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

    struct PlantTrapAnim
    {
        explicit PlantTrapAnim(const sf::Texture & t_texture)
            : is_resetting{ false }
            , is_springing{ false }
            , elapsed_time_sec{ 0.0f }
            , time_between_springs_sec{ 2.0f }
            , time_between_frames_sec{ 0.1f }
            , frame_index{ 0 }
            , sprite{ t_texture }
            , spring_rect{}
            , coll_rect{}
        {}

        bool is_resetting;
        bool is_springing;
        float elapsed_time_sec;
        float time_between_springs_sec;
        float time_between_frames_sec;
        std::size_t frame_index;
        sf::Sprite sprite;
        sf::FloatRect spring_rect;
        sf::FloatRect coll_rect;
    };

    //

    class PlantTrapAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        PlantTrapAnimationLayer(Context & t_context, const std::vector<sf::FloatRect> & t_rects);
        virtual ~PlantTrapAnimationLayer() override;

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
        std::vector<PlantTrapAnim> m_anims;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_PLANT_TRAP_HPP_INCLUDED
