#ifndef ANIM_LAYER_FLAME_TRAP_HPP_INCLUDED
#define ANIM_LAYER_FLAME_TRAP_HPP_INCLUDED
//
// anim-layer-flame-trap.hpp
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
        FlameTrapRectDir(const sf::FloatRect & t_rect, const TrapDirection t_direction)
            : rect{ t_rect }
            , direction{ t_direction }
        {}

        sf::FloatRect rect{};
        TrapDirection direction{ TrapDirection::Up }; // any will work here
    };

    //

    struct FlameAnim
    {
        float elapsed_time_sec{ 0.0f };
        float time_between_flaming_sec{ 0.0f };
        float time_between_frames_sec{ 0.15f };
        std::size_t frame_index{ 0 };
        sf::Sprite sprite{};
        bool is_flaming{ false };
        TrapDirection direction{ TrapDirection::Up }; // anything works here
        sf::FloatRect coll_rect{};
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
        [[nodiscard]] sf::IntRect
            textureRect(const TrapDirection t_direction, const std::size_t t_frame) const noexcept;

      private:
        float m_scale;
        sf::Texture m_emitterUpTexture;
        sf::Texture m_emitterDownTexture;
        sf::Texture m_emitterLeftTexture;
        sf::Texture m_emitterRightTexture;

        sf::Texture m_flamesUpTexture;
        sf::Texture m_flamesDownTexture;
        sf::Texture m_flamesLeftTexture;
        sf::Texture m_flamesRightTexture;

        std::vector<sf::Sprite> m_emitterSprites;
        std::vector<FlameAnim> m_anims;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_FLAME_TRAP_HPP_INCLUDED
