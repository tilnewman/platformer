#ifndef PICKUPS_HPP_INCLUDED
#define PICKUPS_HPP_INCLUDED
//
// pickups.hpp
//
#include <string_view>
#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RenderTarget;
    struct RenderStates;
} // namespace sf

namespace bramblefore
{
    struct Context;
    struct Settings;

    //

    enum class Pickup : std::size_t
    {
        Arrow = 0,
        Bottle,
        Coin1,
        Coin10,
        Crystal1,
        Crystal2,
        Crystal3,
        Crystal4,
        Heart,
        Plus,
        Star,
        Count
    };

    [[nodiscard]] inline constexpr std::string_view toFilename(const Pickup t_pickup) noexcept
    {
        // clang-format off
        switch (t_pickup)
        {
            case Pickup::Arrow:     { return "arrow.png";    }
            case Pickup::Bottle:    { return "bottle.png";   }
            case Pickup::Coin1:     { return "coin1.png";    }
            case Pickup::Coin10:    { return "coin2.png";    }
            case Pickup::Crystal1:  { return "crystal1.png"; }
            case Pickup::Crystal2:  { return "crystal2.png"; }
            case Pickup::Crystal3:  { return "crystal3.png"; }
            case Pickup::Crystal4:  { return "crystal4.png"; }
            case Pickup::Heart:     { return "heart.png";    }
            case Pickup::Plus:      { return "plus.png";     }
            case Pickup::Star:      { return "star.png";     }
            case Pickup::Count:     [[fallthrough]];
            default:                { return "Error_Pickup_unknown"; }
        }
        // clang-format on
    }

    [[nodiscard]] inline constexpr Pickup stringToPickup(const std::string_view name) noexcept
    {
        // clang-format off
        if      (name == "arrow")    return Pickup::Arrow;
        else if (name == "bottle")   return Pickup::Bottle;
        else if (name == "coin1")    return Pickup::Coin1;
        else if (name == "coin10")   return Pickup::Coin10;
        else if (name == "crystal1") return Pickup::Crystal1;
        else if (name == "crystal2") return Pickup::Crystal2;
        else if (name == "crystal3") return Pickup::Crystal3;
        else if (name == "crystal4") return Pickup::Crystal4;
        else if (name == "heart")    return Pickup::Heart;
        else if (name == "plus")     return Pickup::Plus;
        else if (name == "star")     return Pickup::Star;
        else                         return Pickup::Count;
        // clang-format on
    }

    //

    struct PickupAnim
    {
        explicit PickupAnim(
            const Pickup t_pickup,
            const sf::Texture & t_texture,
            const sf::IntRect & t_textureRect,
            const std::size_t t_frameIndex,
            const float t_scale,
            const sf::FloatRect & t_screenRect);

        bool is_alive;
        Pickup which;
        std::size_t frame_index;
        sf::Sprite sprite;
    };

    //

    struct PickupFlareAnim
    {
        explicit PickupFlareAnim(const sf::Sprite & t_sprite, const sf::IntRect & t_textureRect);

        bool is_alive;
        sf::Sprite sprite;
    };

    //

    class PickupAnimations
    {
      public:
        PickupAnimations();

        void setup(const Context & t_context);

        void add(
            const Context & t_context, const sf::FloatRect & t_rect, const Pickup t_pickup);

        void update(Context & t_context, const float t_frameTimeSec);

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const;

        void move(const float amount);
        inline void clear() noexcept { m_anims.clear(); }
        void processCollisionWithAvatar(Context & t_context, const sf::FloatRect & t_avatarRect);

      private:
        [[nodiscard]] std::size_t frameCount(const Pickup which) const;
        [[nodiscard]] sf::IntRect textureRect(const Pickup which, const std::size_t frame) const;

      private:
        std::vector<sf::Texture> m_textures;
        std::vector<PickupAnim> m_anims;
        std::vector<PickupFlareAnim> m_flareAnims;
        float m_elapsedTimeSec;
        float m_timePerFrameSec;
    };

} // namespace bramblefore

#endif // PICKUPS_HPP_INCLUDED
