#ifndef ANIM_LAYER_FLAME_TRAP_HPP_INCLUDED
#define ANIM_LAYER_FLAME_TRAP_HPP_INCLUDED
//
// anim-layer-flame-trap.hpp
//
#include "map/tile-layer.hpp"
#include "subsystem/harm-collision-manager.hpp"

#include <string>
#include <string_view>
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

    enum class TrapDirection
    {
        Up,
        Down,
        Left,
        Right
    };

    [[nodiscard]] inline constexpr std::string_view toString(const TrapDirection t_dir) noexcept
    {
        // clang-format off
        switch (t_dir)
        {
            case TrapDirection::Up:     { return "up";       }
            case TrapDirection::Down:   { return "down";     }
            case TrapDirection::Left:   { return "left";     }
            case TrapDirection::Right:  { return "right";    }
            default: { return "error_TrapDirection_unknown"; }
        }
        // clang-format on
    }

    inline TrapDirection stringToTrapDirection(const std::string & t_str) noexcept
    {
        // clang-format off
        if      (t_str == "up")     return TrapDirection::Up;
        else if (t_str == "down")   return TrapDirection::Down;
        else if (t_str == "left")   return TrapDirection::Left;
        else                        return TrapDirection::Right;
        // clang-format on
    }

    //

    struct FlameTrapRectDir
    {
        explicit FlameTrapRectDir(const sf::FloatRect & t_rect, const TrapDirection t_direction);

        sf::FloatRect rect;
        TrapDirection direction;
    };

    //

    struct FlameAnim
    {
        explicit FlameAnim(
            const TrapDirection t_dir,
            const sf::Texture & t_texture,
            const sf::IntRect & t_textureRect,
            const float t_timeBetweenFlamingSec,
            const float t_scale,
            const sf::FloatRect & t_screenRect,
            const sf::FloatRect & t_emitterRect);

        float elapsed_time_sec;
        float time_between_flaming_sec;
        float time_between_frames_sec;
        std::size_t frame_index;
        sf::Sprite sprite;
        bool is_flaming;
        TrapDirection direction;
        sf::FloatRect coll_rect;
    };

    //

    class FlameTrapAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        FlameTrapAnimationLayer(
            Context & t_context, const std::vector<FlameTrapRectDir> & t_rectDirs);

        virtual ~FlameTrapAnimationLayer() override;

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
        [[nodiscard]] sf::IntRect
            textureRect(const TrapDirection t_direction, const std::size_t t_frame) const noexcept;

      private:
        float m_emitterScale;
        sf::Texture m_emitterUpTexture;
        sf::Texture m_emitterDownTexture;
        sf::Texture m_emitterLeftTexture;
        sf::Texture m_emitterRightTexture;

        float m_flameScale;
        sf::Texture m_flamesUpTexture;
        sf::Texture m_flamesDownTexture;
        sf::Texture m_flamesLeftTexture;
        sf::Texture m_flamesRightTexture;

        std::vector<sf::Sprite> m_emitterSprites;
        std::vector<FlameAnim> m_anims;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_FLAME_TRAP_HPP_INCLUDED
