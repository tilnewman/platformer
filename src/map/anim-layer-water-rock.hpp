#ifndef ANIM_LAYER_WATER_ROCK_HPP_INCLUDED
#define ANIM_LAYER_WATER_ROCK_HPP_INCLUDED
//
// anim-layer-water-rock.hpp
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

    enum class WaterRock : std::size_t
    {
        Flat1,
        Flat2,
        Medium1,
        Medium2,
        Pillar,
        Count
    };

    [[nodiscard]] constexpr std::string_view toString(const WaterRock t_rock) noexcept
    {
        // clang-format off
        switch (t_rock)
        {
            case WaterRock::Flat1:      { return "flat1";   }
            case WaterRock::Flat2:      { return "flat2";   }
            case WaterRock::Medium1:    { return "medium1"; }
            case WaterRock::Medium2:    { return "medium2"; }
            case WaterRock::Pillar:     { return "pillar";  }
            case WaterRock::Count:      [[fallthrough]];
            default:    { return "Error_WaterRock_unknown"; }
        }
        // clang-format on
    }

    [[nodiscard]] inline WaterRock fromString(const std::string & t_str) noexcept
    {
        // clang-format off
        if      (t_str == "flat1")   return WaterRock::Flat1;
        else if (t_str == "flat2")   return WaterRock::Flat2;
        else if (t_str == "medium1") return WaterRock::Medium1;
        else if (t_str == "medium2") return WaterRock::Medium2;
        else if (t_str == "pillar")  return WaterRock::Pillar;
        else                         return WaterRock::Count;
        // clang-format on
    }

    //

    struct WaterRockRect
    {
        explicit WaterRockRect(const WaterRock t_rock, const sf::FloatRect & t_rect);

        WaterRock rock;
        sf::FloatRect rect;
    };

    //

    struct WaterRockAnim
    {
        explicit WaterRockAnim(
            const WaterRock t_rock,
            const sf::Texture & t_texture,
            const sf::IntRect & t_textureRect,
            const float t_timePerFrameSec,
            const float t_scale,
            const sf::FloatRect & t_screenRect);

        WaterRock rock;
        sf::Sprite sprite;
        float elapsed_time_sec;
        float time_per_frame_sec;
        std::size_t frame_index;
    };

    //

    class WaterRockAnimationLayer : public ITileLayer
    {
      public:
        WaterRockAnimationLayer(
            Context & t_context, const std::vector<WaterRockRect> & t_rockRects);

        virtual ~WaterRockAnimationLayer() final;

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

      private:
        [[nodiscard]] const sf::Texture & getTexture(const WaterRock t_rock) const;
        [[nodiscard]] std::size_t frameCount(const WaterRock t_rock) const noexcept;

        [[nodiscard]] sf::IntRect
            textureRect(const WaterRock t_rock, const std::size_t t_frame) const noexcept;

      private:
        std::vector<sf::Texture> m_textures;
        std::vector<WaterRockAnim> m_rockAnims;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_WATER_ROCK_HPP_INCLUDED
