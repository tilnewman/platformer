#ifndef ANIM_LAYER_FALLING_ROCK_HPP_INCLUDED
#define ANIM_LAYER_FALLING_ROCK_HPP_INCLUDED
//
// anim-layer-falling-rock.hpp
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

    enum class Rock
    {
        Rock1, // largest
        Rock2,
        Rock3,
        Rock4 // smallest
    };

    [[nodiscard]] inline constexpr std::string_view toString(const Rock t_rock) noexcept
    {
        // clang-format off
        switch (t_rock)
        {
            case Rock::Rock1:   { return "rock1";   }
            case Rock::Rock2:   { return "rock2";   }
            case Rock::Rock3:   { return "rock3";   }
            case Rock::Rock4:   { return "rock4";   }
            default: { return "error_Rock_unknown"; }
        }
        // clang-format on
    }

    [[nodiscard]] inline Rock stringToRock(const std::string & t_str) noexcept
    {
        // clang-format off
        if      (t_str == "rock1")  return Rock::Rock1;
        else if (t_str == "rock2")  return Rock::Rock2;
        else if (t_str == "rock3")  return Rock::Rock3;
        else                        return Rock::Rock4;
        // clang-format on
    }

    //

    struct RectRock
    {
        explicit RectRock(const sf::FloatRect & t_rect, const Rock t_rock)
            : rect{ t_rect }
            , rock{ t_rock }
        {}

        sf::FloatRect rect;
        Rock rock;
    };

    //

    struct RockShatterAnim
    {
        explicit RockShatterAnim(
            const sf::Sprite & t_sprite, const Rock t_rock, const sf::FloatRect & t_collRect);

        bool is_alive;
        float elapsed_time_sec;
        float time_between_frames_sec;
        std::size_t frame_index;
        sf::Sprite sprite;
        sf::FloatRect coll_rect;
        Rock rock;
    };

    //

    struct RockDropAnim
    {
        explicit RockDropAnim(
            const sf::Sprite & t_sprite, const Rock t_rock, const sf::FloatRect & t_rect);

        bool is_alive;
        float velocity;
        sf::Sprite sprite;
        Rock rock;
        sf::FloatRect fall_region;
    };

    //

    struct RockHangingAnim
    {
        explicit RockHangingAnim(
            const Context & t_context,
            const sf::Texture & t_texture,
            const sf::IntRect & t_textureRect,
            const Rock t_rock,
            const sf::FloatRect & t_rect,
            const float t_scale);

        bool has_dropped;
        sf::Sprite sprite;
        sf::FloatRect trigger_region;
        Rock rock;
        sf::FloatRect fall_region;
    };

    //

    class FallingRockAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        FallingRockAnimationLayer(Context & t_context, const std::vector<RectRock> & t_rectRocks);
        virtual ~FallingRockAnimationLayer() final;

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
        void updateHangingRocks(const Context & t_context, const float t_frameTimeSec);
        void updateDroppingRocks(const Context & t_context, const float t_frameTimeSec);
        void updateShatteringRocks(const Context & t_context, const float t_frameTimeSec);

        [[nodiscard]] const sf::Texture & texture(const Rock t_rock) const;
        [[nodiscard]] std::size_t frameCount(const sf::Texture & t_texture) const noexcept;

        [[nodiscard]] sf::IntRect
            textureRect(const sf::Texture & t_texture, const std::size_t t_frame) const noexcept;

        [[nodiscard]] Harm makeHarm(const Rock t_rock, const sf::FloatRect & t_rect) const noexcept;

      private:
        sf::Texture m_texture1;
        sf::Texture m_texture2;
        sf::Texture m_texture3;
        sf::Texture m_texture4;
        std::vector<RockHangingAnim> m_hangingAnims;
        std::vector<RockDropAnim> m_dropAnims;
        std::vector<RockShatterAnim> m_shatterAnims;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_FALLING_ROCK_HPP_INCLUDED
