#ifndef ACCENTS_HPP_INCLUDED
#define ACCENTS_HPP_INCLUDED
//
// accents.hpp
//
#include <string_view>
#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
} // namespace sf

namespace bramblefore
{
    struct Context;
    struct Settings;

    //

    enum class Accent : std::size_t
    {
        LampGobletLarge = 0,
        LampGobletMedium,
        LampGobletSmall,
        LampHanging,
        LampShort,
        LampTall,
        LampTri,
        VineLoop1,
        VineLoop2,
        VineLoop3,
        VineLoop4,
        VineLoop5,
        VineLoop6,
        Vine1,
        Vine2,
        Vine3,
        Count
    };

    inline constexpr std::string_view toFilename(const Accent t_pickup)
    {
        // clang-format off
        switch (t_pickup)
        {
            case Accent::LampGobletLarge:   { return "lamp-goblet-large.png";   }
            case Accent::LampGobletMedium:  { return "lamp-goblet-medium.png";  }
            case Accent::LampGobletSmall:   { return "lamp-goblet-small.png";   }
            case Accent::LampHanging:       { return "lamp-hanging.png";        }
            case Accent::LampShort:         { return "lamp-short.png";          }
            case Accent::LampTall:          { return "lamp-tall.png";           }
            case Accent::LampTri:           { return "lamp-tri.png";            }
            case Accent::VineLoop1:         { return "vine-loop1.png";          }
            case Accent::VineLoop2:         { return "vine-loop2.png";          }
            case Accent::VineLoop3:         { return "vine-loop3.png";          }
            case Accent::VineLoop4:         { return "vine-loop4.png";          }
            case Accent::VineLoop5:         { return "vine-loop5.png";          }
            case Accent::VineLoop6:         { return "vine-loop6.png";          }
            case Accent::Vine1:             { return "vine1.png";               }
            case Accent::Vine2:             { return "vine2.png";               }
            case Accent::Vine3:             { return "vine3.png";               }
            case Accent::Count:             [[fallthrough]];
            default:                        { return "Error_Accent_unknown";    }
        }
        // clang-format on
    }

    inline constexpr bool isVine(const Accent t_accent)
    {
        // clang-format off
        switch (t_accent)
        {
            case Accent::LampGobletLarge:
            case Accent::LampGobletMedium:
            case Accent::LampGobletSmall:
            case Accent::LampHanging:
            case Accent::LampShort:
            case Accent::LampTall:
            case Accent::LampTri:           { return false; }
            case Accent::VineLoop1:
            case Accent::VineLoop2:
            case Accent::VineLoop3:
            case Accent::VineLoop4:
            case Accent::VineLoop5:
            case Accent::VineLoop6:
            case Accent::Vine1:
            case Accent::Vine2:
            case Accent::Vine3:
            case Accent::Count:
            default:                        { return true; }
        }
        // clang-format on
    }

    inline Accent stringToAccent(const std::string & t_name)
    {
        // clang-format off
        if      (t_name == "lamp-goblet-large")   return Accent::LampGobletLarge;
        else if (t_name == "lamp-goblet-medium")  return Accent::LampGobletMedium;
        else if (t_name == "lamp-goblet-small")   return Accent::LampGobletSmall;
        else if (t_name == "lamp-hanging")        return Accent::LampHanging;
        else if (t_name == "lamp-short")          return Accent::LampShort;
        else if (t_name == "lamp-tall")           return Accent::LampTall;
        else if (t_name == "lamp-tri")            return Accent::LampTri;
        else if (t_name == "vine-loop1")          return Accent::VineLoop1;
        else if (t_name == "vine-loop2")          return Accent::VineLoop2;
        else if (t_name == "vine-loop3")          return Accent::VineLoop3;
        else if (t_name == "vine-loop4")          return Accent::VineLoop4;
        else if (t_name == "vine-loop5")          return Accent::VineLoop5;
        else if (t_name == "vine-loop6")          return Accent::VineLoop6;
        else if (t_name == "vine1")               return Accent::Vine1;
        else if (t_name == "vine2")               return Accent::Vine2;
        else if (t_name == "vine3")               return Accent::Vine3;
        else                                      return Accent::Count;
        // clang-format on
    }

    //

    struct AccentAnim
    {
        Accent which{ Accent::Vine1 }; // anything works here
        std::size_t anim_index{ 0 };
        sf::Sprite sprite{};

        float elapsed_time_sec{ 0.0f };
        float time_per_frame_sec{ 0.0f };
    };

    //

    class AccentAnimations
    {
      public:
        AccentAnimations();

        void setup(const Context & t_context);

        // add() with no remove() because lamps and vines never leave the map
        void add(
            const Context & t_context, const sf::FloatRect & t_rect, const std::string & t_name);

        void update(Context & t_context, const float t_frameTimeSec);

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const;

        void move(const float t_amount);

        inline void clear() noexcept { m_anims.clear(); }

      private:
        [[nodiscard]] std::size_t frameCount(const Accent t_which) const;

        [[nodiscard]] sf::IntRect
            textureRect(const Accent t_which, const std::size_t t_frame) const;

      private:
        std::vector<sf::Texture> m_textures;
        std::vector<AccentAnim> m_anims;
        sf::Vector2f m_scale;
    };

} // namespace bramblefore

#endif // ACCENTS_HPP_INCLUDED
