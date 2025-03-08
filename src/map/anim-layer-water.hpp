#ifndef WATER_ANIM_LAYER_HPP_INCLUDED
#define WATER_ANIM_LAYER_HPP_INCLUDED
//
// water-anim-layer.hpp
//
#include "map/tile-layer.hpp"
#include "subsystem/harm-collision-manager.hpp"

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

    enum class WaterType : std::size_t
    {
        Surface1 = 0,
        Surface2,
        Basin1,
        Basin2,
        Count
    };

    [[nodiscard]] constexpr std::string_view toFilename(const WaterType t_type) noexcept
    {
        // clang-format off
        switch (t_type)
        {
            case WaterType::Surface1: { return "water-surface1.png"; }
            case WaterType::Surface2: { return "water-surface2.png"; }
            case WaterType::Basin1:   { return "water-basin1.png";   }
            case WaterType::Basin2:   { return "water-basin2.png";   }
            case WaterType::Count:    [[fallthrough]];
            default:             { return "Error_unknown_WaterType"; }
        }
        // clang-format on
    }

    //

    struct WaterTypeRect
    {
        explicit WaterTypeRect(const bool t_isSurface, const sf::FloatRect & t_rect);

        bool is_surface;
        sf::FloatRect rect;
    };

    //

    struct WaterAnim
    {
        explicit WaterAnim(
            const WaterType t_type,
            const sf::Texture & t_texture,
            const sf::IntRect & t_textureRect,
            const float t_timePerFrameSec,
            const sf::FloatRect & t_screenRect);

        WaterType type;
        sf::Sprite sprite;
        float elapsed_time_sec;
        float time_per_frame_sec;
        std::size_t frame_index;
    };

    //

    class WaterAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        WaterAnimationLayer(Context & t_context, const std::vector<WaterTypeRect> & t_typeRects);
        virtual ~WaterAnimationLayer() final;

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

        [[nodiscard]] Harm
            avatarCollide(Context & t_context, const sf::FloatRect & t_avatarRect) final;

      private:
        [[nodiscard]] const sf::Texture & getTexture(const WaterType t_type) const;
        [[nodiscard]] std::size_t frameCount(const WaterType t_type) const;

        [[nodiscard]] sf::IntRect
            textureRect(const WaterType t_type, const std::size_t frame) const;

      private:
        std::vector<sf::Texture> m_textures;
        std::vector<WaterAnim> m_anims;
    };

} // namespace bramblefore

#endif // WATER_ANIM_LAYER_HPP_INCLUDED
