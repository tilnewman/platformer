#ifndef PLAYER_INFO_HPP_INCLUDED
#define PLAYER_INFO_HPP_INCLUDED
//
// player-info.hpp
//
#include "player/custom-types.hpp"

#include <algorithm>
#include <string_view>
#include <vector>

namespace bramblefore
{

    struct Context;

    //

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

    [[nodiscard]] inline constexpr std::string_view toString(const AvatarType t_type) noexcept
    {
        // clang-format off
        switch (t_type)
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
            case AvatarType::Count:         [[fallthrough]];
            default:           { return "error_AvatarType_unknown"; }
        }
        // clang-format on
    }

    [[nodiscard]] inline constexpr bool isMale(const AvatarType t_type) noexcept
    {
        return (
            (AvatarType::Druid != t_type) && (AvatarType::Enchantress != t_type) &&
            (AvatarType::Witch != t_type));
    }

    [[nodiscard]] inline constexpr bool isSpellCaster(const AvatarType t_type) noexcept
    {
        return (
            (AvatarType::Druid == t_type) || (AvatarType::Enchantress == t_type) ||
            (AvatarType::Witch == t_type));
    }

    [[nodiscard]] inline constexpr Health_t startingHealth(const AvatarType t_type) noexcept
    {
        // clang-format off
        switch (t_type)
        {
            case AvatarType::Assassin:      { return 86;  }
            case AvatarType::BlueKnight:    { return 95;  }
            case AvatarType::Druid:         { return 70;  }
            case AvatarType::Enchantress:   { return 65;  }
            case AvatarType::Ninja:         { return 84;  }
            case AvatarType::RedKnight:     { return 90;  }
            case AvatarType::Rogue:         { return 80;  }
            case AvatarType::Viking:        { return 100; }
            case AvatarType::Witch:         { return 60;  }
            case AvatarType::Count:         [[fallthrough]];
            default:                        { return 0;   }
        }
        // clang-format on
    }

    [[nodiscard]] inline constexpr Mana_t startingMana(const AvatarType t_type) noexcept
    {
        if (AvatarType::Witch == t_type)
        {
            return 100;
        }
        else if (AvatarType::Druid == t_type)
        {
            return 80;
        }
        else if (AvatarType::Enchantress == t_type)
        {
            return 90;
        }
        else
        {
            return 0;
        }
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

    [[nodiscard]] inline constexpr std::string_view toName(const Spell t_spell) noexcept
    {
        // clang-format off
        switch (t_spell)
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
            case Spell::Count:      [[fallthrough]];
            default:            { return "Error_Spell_unknown"; }
        }
        // clang-format on
    }

    [[nodiscard]] inline constexpr std::string_view toFilesystemName(const Spell t_spell) noexcept
    {
        // clang-format off
        switch (t_spell)
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
            case Spell::Count:      [[fallthrough]];
            default:        { return "Error_Spell_unknown"; }
        }
        // clang-format on
    }

    // TODO figure out the real values
    [[nodiscard]] inline constexpr Mana_t toManaCost(const Spell t_spell) noexcept
    {
        // clang-format off
        switch (t_spell)
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
            case Spell::Count:    [[fallthrough]];
            default:                { return 0;  }
        }
        // clang-format on
    }

    // TODO figure out the real values
    [[nodiscard]] inline constexpr Mana_t toDamage(const Spell t_spell) noexcept
    {
        // clang-format off
        switch (t_spell)
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
            case Spell::Count:    [[fallthrough]];
            default:                { return 0;  }
        }
        // clang-format on
    }

    [[nodiscard]] inline constexpr float timePerFrameSec(const Spell t_spell) noexcept
    {
        if (Spell::Light == t_spell)
        {
            return 0.25f;
        }
        else if (
            (Spell::KillAll == t_spell) || (Spell::SunStrike == t_spell) ||
            (Spell::Water == t_spell))
        {
            return 0.175f;
        }
        else
        {
            return 0.125f;
        }
    }

    [[nodiscard]] inline std::vector<Spell> makeSpellSet(const AvatarType t_type) noexcept
    {
        if (AvatarType::Witch == t_type)
        {
            return {
                Spell::Comet, Spell::Explosion, Spell::Spikes2, Spell::Lightning2, Spell::KillAll
            };
        }
        else if (AvatarType::Druid == t_type)
        {
            return { Spell::Water,     Spell::Fire,    Spell::Light,
                     Spell::SunStrike, Spell::Tornado, Spell::Freeze };
        }
        else if (AvatarType::Enchantress == t_type)
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

        void setup(Context & t_context, const AvatarType t_type);

        [[nodiscard]] inline constexpr AvatarType avatarType() const noexcept
        {
            return m_avatarType;
        }

        [[nodiscard]] inline constexpr Health_t health() const noexcept { return m_health; }

        [[nodiscard]] inline constexpr Health_t healthMax() const noexcept { return m_healthMax; }

        Health_t healthAdjust(Context & t_context, const Health_t t_adjustment);
        void healthReset(Context & t_context);

        [[nodiscard]] inline constexpr Mana_t mana() const noexcept { return m_mana; }

        [[nodiscard]] inline constexpr Mana_t manaMax() const noexcept { return m_manaMax; }

        Mana_t manaAdjust(Context & t_context, const Mana_t t_adjustment);
        void manaReset(Context & t_context);

        [[nodiscard]] inline constexpr Coin_t coin() const noexcept { return m_coins; }

        Coin_t coinAdjust(Context & t_context, const Coin_t t_adjustment);

        [[nodiscard]] inline constexpr Experience_t experience() const noexcept
        {
            return m_experience;
        }

        Experience_t experienceAdjust(const Experience_t adjustment);

        [[nodiscard]] inline constexpr int mapStarCount() const noexcept { return m_mapStarCount; }

        void mapStarCollect(Context & t_context);
        void mapStarReset(Context & t_context);

        [[nodiscard]] inline constexpr std::size_t currentSpellIndex() const noexcept
        {
            return m_currentSpell;
        }

        void setCurentSpellIndex(const std::size_t newSpellIndex);

        [[nodiscard]] Spell currentSpell() const;

        [[nodiscard]] inline const std::vector<PlayerSpell> & spells() const noexcept
        {
            return m_spells;
        }

        void learnSpell(const Spell t_spell);

      private:
        AvatarType m_avatarType;
        Health_t m_health;
        Health_t m_healthMax;
        Mana_t m_mana;
        Mana_t m_manaMax;
        Coin_t m_coins;
        Experience_t m_experience;
        int m_mapStarCount;
        std::size_t m_currentSpell;
        std::vector<PlayerSpell> m_spells;
    };

} // namespace bramblefore

#endif // PLAYER_INFO_HPP_INCLUDED
