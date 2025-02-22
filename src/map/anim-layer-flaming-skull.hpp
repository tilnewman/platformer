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
        float elapsed_time_sec{ 0.0f };
        float time_between_flaming{ 0.0f };
        float time_between_frames_sec{ 0.15f };
        std::size_t frame_index{ 0 };
        sf::Sprite sprite{};
        bool is_flaming{ false };
        FlameDirection direction{ FlameDirection::Up }; // anything works here
        sf::FloatRect coll_rect{};
    };

    //

    class FlamingSkullAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        FlamingSkullAnimationLayer(Context & t_context, const std::vector<sf::FloatRect> & t_rects);
        virtual ~FlamingSkullAnimationLayer() override;

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
        [[nodiscard]] std::size_t frameCount(const sf::Texture & t_texture) const noexcept;

        [[nodiscard]] sf::IntRect
            textureRect(const sf::Texture & t_texture, const std::size_t t_frame) const noexcept;

        [[nodiscard]] const sf::Texture & getTexture(const FlameDirection t_direction) const;

      private:
        float m_scale;
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
