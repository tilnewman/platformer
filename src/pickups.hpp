#ifndef PICKUPS_HPP_INCLUDED
#define PICKUPS_HPP_INCLUDED
//
// pickups.hpp
//
#include <string_view>
#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace platformer
{
    struct Context;
    struct Settings;

    //

    enum class Pickup : std::size_t
    {
        Arrow = 0,
        Bottle,
        Coin1,
        Coin2,
        Crystal1,
        Crystal2,
        Crystal3,
        Crystal4,
        Heart,
        Plus,
        Star,
        Count
    };

    inline constexpr std::string_view toFilename(const Pickup pickup)
    {
        // clang-format off
        switch (pickup)
        {
            case Pickup::Arrow:     { return "arrow.png";    }
            case Pickup::Bottle:    { return "bottle.png";   }
            case Pickup::Coin1:     { return "coin1.png";    }
            case Pickup::Coin2:     { return "coin2.png";    }
            case Pickup::Crystal1:  { return "crystal1.png"; }
            case Pickup::Crystal2:  { return "crystal2.png"; }
            case Pickup::Crystal3:  { return "crystal3.png"; }
            case Pickup::Crystal4:  { return "crystal4.png"; }
            case Pickup::Heart:     { return "heart.png";    }
            case Pickup::Plus:      { return "plus.png";     }
            case Pickup::Star:      { return "star.png";     }
            case Pickup::Count:     //intentional fallthrough
            default:                { return "Error_Pickup_unknown"; }
        }
        // clang-format on
    }

    inline Pickup stringToPickup(const std::string & name)
    {
        // clang-format off
        if (name == "arrow")         return Pickup::Arrow;
        else if (name == "bottle")   return Pickup::Bottle;
        else if (name == "coin1")    return Pickup::Coin1;
        else if (name == "coin2")    return Pickup::Coin2;
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
        Pickup which{ Pickup::Arrow }; // any pickup works here
        std::size_t anim_index{ 0 };
        sf::Sprite sprite{};
    };

    //

    class PickupAnimations
    {
      public:
        PickupAnimations();

        void setup(const Settings & settings);
        void add(const Context & context, const sf::FloatRect & rect, const std::string & name);
        void update(Context & context, const float frameTimeSec);
        void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const;
        void move(const float amount);
        inline void clear() { m_anims.clear(); }

      private:
        std::size_t frameCount(const Pickup which) const;
        const sf::IntRect textureRect(const Pickup which, const std::size_t frame) const;

      private:
        std::vector<sf::Texture> m_textures;
        std::vector<PickupAnim> m_anims;
        float m_elapsedTimeSec;
        float m_timePerFrameSec;
        sf::Vector2f m_scale;
    };

} // namespace platformer

#endif // PICKUPS_HPP_INCLUDED
