#ifndef ANIM_LAYER_FLAMING_SKULL_HPP_INCLUDED
#define ANIM_LAYER_FLAMING_SKULL_HPP_INCLUDED
//
// anim-layer-flaming-skull.hpp
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

    enum class FlameDirection
    {
        Up,
        Down,
        Left,
        Right
    };

    //

    struct FlamesAnim
    {
        explicit FlamesAnim(
            const sf::Texture & t_texture,
            const sf::IntRect & t_textureRect,
            const FlameDirection t_dir,
            const float t_timeBetweenFlamingSec,
            const float t_scale,
            const sf::FloatRect & t_screenRect);

        float elapsed_time_sec;
        float time_between_flaming_sec;
        float time_between_frames_sec;
        std::size_t frame_index;
        sf::Sprite sprite;
        bool is_flaming;
        FlameDirection direction;
        sf::FloatRect coll_rect;
    };

    //

    class FlamingSkullAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        FlamingSkullAnimationLayer(Context & t_context, const std::vector<sf::FloatRect> & t_rects);
        virtual ~FlamingSkullAnimationLayer() final;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void move(const Context & t_context, const float t_amount) final;
        void dumpInfo() const final;
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
        [[nodiscard]] std::size_t frameCount(const sf::Texture & t_texture) const noexcept;

        [[nodiscard]] sf::IntRect
            textureRect(const sf::Texture & t_texture, const std::size_t t_frame) const noexcept;

        [[nodiscard]] const sf::Texture & getTexture(const FlameDirection t_direction) const;

      private:
        sf::Texture m_skullBlockTexture;
        sf::Texture m_flamesUpTexture;
        sf::Texture m_flamesDownTexture;
        sf::Texture m_flamesLeftTexture;
        sf::Texture m_flamesRightTexture;
        std::vector<sf::Sprite> m_skullBlockSprites;
        std::vector<FlamesAnim> m_anims;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_FLAMING_SKULL_HPP_INCLUDED
