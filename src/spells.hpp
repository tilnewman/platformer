#ifndef SPELLS_HPP_INCLUDED
#define SPELLS_HPP_INCLUDED
//
// spells.hpp
//
#include <string_view>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
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

    enum class Spell : std::size_t
    {
        Comet = 0,
        Explosion,
        Fire,
        Freeze,
        Gypno,
        KillAll,
        Light,
        Lightning1,
        Lightning2,
        MidasHand,
        Spikes1,
        Spikes2,
        SunStrike,
        TeslaBall,
        Tornado,
        Water,
        Count
    };

    inline constexpr std::string_view toName(const Spell spell)
    {
        // clang-format off
        switch (spell)
        {
            case Spell::Comet:      { return "comet";       }
            case Spell::Explosion:  { return "explosion";   }
            case Spell::Fire:       { return "fire";        }
            case Spell::Freeze:     { return "freeze";      }
            case Spell::Gypno:      { return "gypno";       }
            case Spell::KillAll:    { return "kill-all";    }
            case Spell::Light:      { return "light";       }
            case Spell::Lightning1: { return "lightning1";  }
            case Spell::Lightning2: { return "lightning2";  }
            case Spell::MidasHand:  { return "midas-hand";  }
            case Spell::Spikes1:    { return "spikes1";     }
            case Spell::Spikes2:    { return "spikes2";     }
            case Spell::SunStrike:  { return "sun-strike";  }
            case Spell::TeslaBall:  { return "tesla-ball";  }
            case Spell::Tornado:    { return "tornado";     }
            case Spell::Water:      { return "water";       }
            case Spell::Count:      //intentional fallthrough
            default:        { return "Error_Spell_unknown"; }
        }
        // clang-format on
    }

    inline constexpr float timePerFrameSec(const Spell spell)
    {
        if (Spell::Light == spell)
        {
            return 0.25f;
        }
        else if (
            (Spell::KillAll == spell) || (Spell::SunStrike == spell) || (Spell::Water == spell))
        {
            return 0.175f;
        }
        else
        {
            return 0.125f;
        }
    }

    //

    struct SpellAnim
    {
        bool is_alive{ true };
        Spell spell{ Spell::Count }; // anything works here
        std::size_t frame_index{ 0 };
        float elapsed_time_sec{ 0.0f };
        float time_per_frame_sec{ 0.0f };
        sf::Sprite sprite{};
    };

    //

    struct SpellTextures
    {
        sf::Texture icon_texture;
        std::vector<sf::Texture> textures;
    };

    //

    class SpellAnimations
    {
      public:
        SpellAnimations();

        void setup(const Settings & settings);
        void add(const sf::Vector2f & pos, const Spell spell);
        void update(Context & context, const float frameTimeSec);
        void draw(sf::RenderTarget & target, sf::RenderStates states) const;
        void move(const float amount);

        inline const sf::Texture iconTexture(const Spell spell) const
        {
            return m_textureSets.at(static_cast<std::size_t>(spell)).icon_texture;
        }

        inline void clear() { m_anims.clear(); }

      private:
        std::vector<SpellTextures> m_textureSets;
        std::vector<SpellAnim> m_anims;
        sf::Vector2f m_scale;
    };

} // namespace platformer

#endif // SPELLS_HPP_INCLUDED
