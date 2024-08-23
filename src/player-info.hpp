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
            default:                        { return "error_AvatarType_not_found"; }
        }
        // clang-format on
    }

    inline constexpr bool isMale(const AvatarType type)
    {
        // clang-format off
        switch (type)
        {
            case AvatarType::Assassin:      { return true;  }
            case AvatarType::BlueKnight:    { return true;  }
            case AvatarType::Druid:         { return false; }
            case AvatarType::Enchantress:   { return false; }
            case AvatarType::Ninja:         { return true;  }
            case AvatarType::RedKnight:     { return true;  }
            case AvatarType::Rogue:         { return true;  }
            case AvatarType::Viking:        { return true;  }
            case AvatarType::Witch:         { return false; }
            case AvatarType::Count:         // intentional fallthrough
            default:                        { return true;  }
        }
        // clang-format on
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

    inline std::vector<Spell> spellSet(const AvatarType type)
    {
        if (AvatarType::Witch == type)
        {
            return {
                Spell::Comet, Spell::Explosion, Spell::Spikes2, Spell::Lightning2, Spell::KillAll
            };
        }
        else if (AvatarType::Druid == type)
        {
            return { Spell::Freeze,    Spell::Fire,    Spell::Light,
                     Spell::SunStrike, Spell::Tornado, Spell::Water };
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

      private:
        AvatarType m_avatarType;
        Health_t m_health;
        Health_t m_healthMax;
        Mana_t m_mana;
        Mana_t m_manaMax;
        Coin_t m_coins;
    };

} // namespace platformer

#endif // PLAYER_INFO_HPP_INCLUDED
