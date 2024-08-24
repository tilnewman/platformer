#ifndef PLAYER_INFO_HPP_INCLUDED
#define PLAYER_INFO_HPP_INCLUDED
//
// player-info.hpp
//
#include "custom-types.hpp"

#include <string_view>
#include <vector>

namespace platformer
{

    enum class AvatarType : std::size_t
    {
        Assassin = 0,
        BlueKnight,
        Druid,
        Enchantress,
        Ninja,
        RedKnight,
        Rogue,
        Viking,
        Witch,
        Count
    };

    inline constexpr std::string_view toString(const AvatarType type)
    {
        // clang-format off
        switch (type)
        {
            case AvatarType::Assassin:      { return "assassin";    }
            case AvatarType::BlueKnight:    { return "blue-knight"; }
            case AvatarType::Druid:         { return "druid";       }
            case AvatarType::Enchantress:   { return "enchantress"; }
            case AvatarType::Ninja:         { return "ninja";       }
            case AvatarType::RedKnight:     { return "red-knight";  }
            case AvatarType::Rogue:         { return "rogue";       }
            case AvatarType::Viking:        { return "viking";      }
            case AvatarType::Witch:         { return "witch";       }
            case AvatarType::Count:         // intentional fallthrough
            default:           { return "error_AvatarType_unknown"; }
        }
        // clang-format on
    }

    inline constexpr bool isMale(const AvatarType type)
    {
        return (
            (AvatarType::Druid != type) && (AvatarType::Enchantress != type) &&
            (AvatarType::Witch != type));
    }

    inline constexpr bool isSpellCaster(const AvatarType type)
    {
        return (
            (AvatarType::Druid == type) || (AvatarType::Enchantress == type) ||
            (AvatarType::Witch == type));
    }

    //

    enum class Spell : std::size_t
    {
        // Witch spells
        Comet = 0,
        Explosion,
        Spikes2,
        Lightning2,
        KillAll,

        // Druid spells
        Fire,
        Freeze,
        Light,
        SunStrike,
        Tornado,

        // Enchantress Spells
        Lightning1,
        MidasHand,
        Spikes1,
        Gypno,
        TeslaBall,
        Water,

        Count
    };

    inline constexpr std::string_view toName(const Spell spell)
    {
        // clang-format off
        switch (spell)
        {
            case Spell::Comet:      { return "Comet";           }
            case Spell::Explosion:  { return "Explosion";       }
            case Spell::Fire:       { return "Inferno";         }
            case Spell::Freeze:     { return "Freeze";          }
            case Spell::Gypno:      { return "Hypno";           }
            case Spell::KillAll:    { return "Death Rattle";    }
            case Spell::Light:      { return "Blinding Light";  }
            case Spell::Lightning1: { return "Sparks";          }
            case Spell::Lightning2: { return "Lightning Strike";}
            case Spell::MidasHand:  { return "Midas";           }
            case Spell::Spikes1:    { return "Spikes";          }
            case Spell::Spikes2:    { return "Earth Errupt";    }
            case Spell::SunStrike:  { return "Sun Strike";      }
            case Spell::TeslaBall:  { return "Lightning Ball";  }
            case Spell::Tornado:    { return "Tornado";         }
            case Spell::Water:      { return "Water Lash";      }
            case Spell::Count:      //intentional fallthrough
            default:            { return "Error_Spell_unknown"; }
        }
        // clang-format on
    }

    inline constexpr std::string_view toFilesystemName(const Spell spell)
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

    // TODO figure out the real values
    inline constexpr Mana_t toManaCost(const Spell spell)
    {
        // clang-format off
        switch (spell)
        {
            case Spell::Comet:      { return 10; }
            case Spell::Explosion:  { return 10; }
            case Spell::Fire:       { return 10; }
            case Spell::Freeze:     { return 10; }
            case Spell::Gypno:      { return 10; }
            case Spell::KillAll:    { return 10; }
            case Spell::Light:      { return 10; }
            case Spell::Lightning1: { return 10; }
            case Spell::Lightning2: { return 10; }
            case Spell::MidasHand:  { return 10; }
            case Spell::Spikes1:    { return 10; }
            case Spell::Spikes2:    { return 10; }
            case Spell::SunStrike:  { return 10; }
            case Spell::TeslaBall:  { return 10; }
            case Spell::Tornado:    { return 10; }
            case Spell::Water:      { return 10; }
            case Spell::Count:      //intentional fallthrough
            default:                { return 0;  }
        }
        // clang-format on
    }

    // TODO figure out the real values
    inline constexpr Mana_t toDamage(const Spell spell)
    {
        // clang-format off
        switch (spell)
        {
            case Spell::Comet:      { return 10; }
            case Spell::Explosion:  { return 10; }
            case Spell::Fire:       { return 10; }
            case Spell::Freeze:     { return 10; }
            case Spell::Gypno:      { return 10; }
            case Spell::KillAll:    { return 10; }
            case Spell::Light:      { return 10; }
            case Spell::Lightning1: { return 10; }
            case Spell::Lightning2: { return 10; }
            case Spell::MidasHand:  { return 10; }
            case Spell::Spikes1:    { return 10; }
            case Spell::Spikes2:    { return 10; }
            case Spell::SunStrike:  { return 10; }
            case Spell::TeslaBall:  { return 10; }
            case Spell::Tornado:    { return 10; }
            case Spell::Water:      { return 10; }
            case Spell::Count:      //intentional fallthrough
            default:                { return 0;  }
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

    inline std::vector<Spell> makeSpellSet(const AvatarType type)
    {
        if (AvatarType::Witch == type)
        {
            return {
                Spell::Comet, Spell::Explosion, Spell::Spikes2, Spell::Lightning2, Spell::KillAll
            };
        }
        else if (AvatarType::Druid == type)
        {
            return { Spell::Water,     Spell::Fire,    Spell::Light,
                     Spell::SunStrike, Spell::Tornado, Spell::Freeze };
        }
        else if (AvatarType::Enchantress == type)
        {
            return {
                Spell::TeslaBall, Spell::Spikes1, Spell::Gypno, Spell::Lightning1, Spell::MidasHand
            };
        }
        else
        {
            return {};
        }
    }

    //

    struct PlayerSpell
    {
        Spell spell{ Spell::Count }; // anything works here
        Mana_t cost{ 0 };
        Health_t damage{ 0 };
        bool is_learned{ false };
    };

    //

    class PlayerInfo
    {
      public:
        PlayerInfo();

        void setup(const AvatarType type);

        inline AvatarType avatarType() const { return m_avatarType; }

        inline Health_t health() const { return m_health; }
        Health_t healthAdjust(const Health_t adjustment);

        inline Mana_t mana() const { return m_mana; }
        Mana_t manaAdjust(const Mana_t adjustment);

        inline Coin_t coins() const { return m_coins; }
        Coin_t coinsAdjust(const Coin_t adjustment);

        inline const std::vector<PlayerSpell> & spells() const { return m_spells; }
        void learnSpell(const Spell spell);

      private:
        AvatarType m_avatarType;
        Health_t m_health;
        Health_t m_healthMax;
        Mana_t m_mana;
        Mana_t m_manaMax;
        Coin_t m_coins;
        std::vector<PlayerSpell> m_spells;
    };

} // namespace platformer

#endif // PLAYER_INFO_HPP_INCLUDED
