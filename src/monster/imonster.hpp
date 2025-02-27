#ifndef IMONSTER_HPP_INCLUDED
#define IMONSTER_HPP_INCLUDED
//
// imonster.hpp
//
#include "player/custom-types.hpp"
#include "subsystem/harm.hpp"

#include <string_view>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace sf
{
    class RenderTarget;
} // namespace sf

namespace bramblefore
{

    struct Context;

    //

    enum class MonsterType : std::size_t
    {
        BabyDragon = 0,
        Bear,
        BigKnight,
        BoneDragon,
        Demon,
        Dino,
        Djinn,
        Dragon,
        Dwarf,
        Ent,
        FireImp,
        FireKnight,
        Ghost,
        Goblin,
        Hound,
        Imp,
        LittleKnight,
        Lizard,
        Medusa,
        Orc,
        Salamander,
        Skeleton,
        Skull,
        Snake,
        Spider,
        Vampire,
        Yeti,
        BossTribal,
        BossKnight,
        BossWizard,
        Count
    };

    inline constexpr std::string_view toString(const MonsterType t_type) noexcept
    {
        // clang-format off
        switch (t_type)
        {
            case MonsterType::BabyDragon:    { return "baby-dragon";    }
            case MonsterType::Bear:          { return "bear";           }
            case MonsterType::BigKnight:     { return "big-knight";     }
            case MonsterType::BoneDragon:    { return "bone-dragon";    }
            case MonsterType::Demon:         { return "demon";          }
            case MonsterType::Dino:          { return "dino";           }
            case MonsterType::Djinn:         { return "djinn";          }
            case MonsterType::Dragon:        { return "dragon";         }
            case MonsterType::Dwarf:         { return "dwarf";          }
            case MonsterType::Ent:           { return "ent";            }
            case MonsterType::FireImp:       { return "fire-imp";       }
            case MonsterType::FireKnight:    { return "fire-knight";    }
            case MonsterType::Ghost:         { return "ghost";          }
            case MonsterType::Goblin:        { return "goblin";         }
            case MonsterType::Hound:         { return "hound";          }
            case MonsterType::Imp:           { return "imp";            }
            case MonsterType::LittleKnight:  { return "little-knight";  }
            case MonsterType::Lizard:        { return "lizard";         }
            case MonsterType::Medusa:        { return "medusa";         }
            case MonsterType::Orc:           { return "orc";            }
            case MonsterType::Salamander:    { return "salamander";     }
            case MonsterType::Skeleton:      { return "skeleton";       }
            case MonsterType::Skull:         { return "skull";          }
            case MonsterType::Snake:         { return "snake";          }
            case MonsterType::Spider:        { return "spider";         }
            case MonsterType::Vampire:       { return "vampire";        }
            case MonsterType::Yeti:          { return "yeti";           }
            case MonsterType::BossTribal:    { return "boss-tribal";    }
            case MonsterType::BossKnight:    { return "boss-knight";    }
            case MonsterType::BossWizard:    { return "boss-wizard";    }
            case MonsterType::Count:         [[fallthrough]];     
            default:              { return "Error_unknown_MonsterType"; }
        }
        // clang-format on
    }

    inline constexpr Health_t startingHealth(const MonsterType t_type) noexcept
    {
        // clang-format off
        switch (t_type)
        {
            case MonsterType::BabyDragon:    { return 25;  }
            case MonsterType::Bear:          { return 30;  }
            case MonsterType::BigKnight:     { return 75;  }
            case MonsterType::BoneDragon:    { return 100; }
            case MonsterType::Demon:         { return 100; }
            case MonsterType::Dino:          { return 38;  }
            case MonsterType::Djinn:         { return 45;  }
            case MonsterType::Dragon:        { return 80;  }
            case MonsterType::Dwarf:         { return 22;  }
            case MonsterType::Ent:           { return 40;  }
            case MonsterType::FireImp:       { return 38;  }
            case MonsterType::FireKnight:    { return 100; }
            case MonsterType::Ghost:         { return 32;  }
            case MonsterType::Goblin:        { return 20;  }
            case MonsterType::Hound:         { return 28;  }
            case MonsterType::Imp:           { return 25;  }
            case MonsterType::LittleKnight:  { return 50;  }
            case MonsterType::Lizard:        { return 30;  }
            case MonsterType::Medusa:        { return 66;  }
            case MonsterType::Orc:           { return 58;  }
            case MonsterType::Salamander:    { return 28;  }
            case MonsterType::Skeleton:      { return 32;  }
            case MonsterType::Skull:         { return 45;  }
            case MonsterType::Snake:         { return 18;  }
            case MonsterType::Spider:        { return 26;  }
            case MonsterType::Vampire:       { return 80;  }
            case MonsterType::Yeti:          { return 50;  }
            case MonsterType::BossTribal:    { return 200; }
            case MonsterType::BossKnight:    { return 200; }
            case MonsterType::BossWizard:    { return 200; }
            case MonsterType::Count:         [[fallthrough]]; 
            default:                         { return 0;   }
        }
        // clang-format on
    }

    inline constexpr float walkSpeed(const MonsterType t_type) noexcept
    {
        // clang-format off
        switch (t_type)
        {
            case MonsterType::BabyDragon:    { return 40.0f; }
            case MonsterType::Bear:          { return 40.0f; }
            case MonsterType::BigKnight:     { return 45.0f; }
            case MonsterType::BoneDragon:    { return 40.0f; }
            case MonsterType::Demon:         { return 45.0f; }
            case MonsterType::Dino:          { return 30.0f; }
            case MonsterType::Djinn:         { return 40.0f; }
            case MonsterType::Dragon:        { return 99.0f; }
            case MonsterType::Dwarf:         { return 40.0f; }
            case MonsterType::Ent:           { return 30.0f; }
            case MonsterType::FireImp:       { return 30.0f; }
            case MonsterType::FireKnight:    { return 40.0f; }
            case MonsterType::Ghost:         { return 40.0f; }
            case MonsterType::Goblin:        { return 40.0f; }
            case MonsterType::Hound:         { return 40.0f; }
            case MonsterType::Imp:           { return 30.0f; }
            case MonsterType::LittleKnight:  { return 30.0f; }
            case MonsterType::Lizard:        { return 40.0f; }
            case MonsterType::Medusa:        { return 40.0f; }
            case MonsterType::Orc:           { return 45.0f; }
            case MonsterType::Salamander:    { return 40.0f; }
            case MonsterType::Skeleton:      { return 40.0f; }
            case MonsterType::Skull:         { return 30.0f; }
            case MonsterType::Snake:         { return 30.0f; }
            case MonsterType::Spider:        { return 50.0f; }
            case MonsterType::Vampire:       { return 40.0f; }
            case MonsterType::Yeti:          { return 40.0f; }
            case MonsterType::BossTribal:    { return 75.0f; }
            case MonsterType::BossKnight:    { return 75.0f; }
            case MonsterType::BossWizard:    { return 75.0f; }
            case MonsterType::Count:         [[fallthrough]];    
            default:                         { return 0.0f;  }
        }
        // clang-format on
    }

    // TODO need real values here
    inline constexpr Health_t attackDamage(const MonsterType t_type) noexcept
    {
        // clang-format off
        switch (t_type)
        {
            case MonsterType::BabyDragon:    { return 1; }
            case MonsterType::Bear:          { return 1; }
            case MonsterType::BigKnight:     { return 1; }
            case MonsterType::BoneDragon:    { return 1; }
            case MonsterType::Demon:         { return 1; }
            case MonsterType::Dino:          { return 16; }
            case MonsterType::Djinn:         { return 1; }
            case MonsterType::Dragon:        { return 1; }
            case MonsterType::Dwarf:         { return 1; }
            case MonsterType::Ent:           { return 1; }
            case MonsterType::FireImp:       { return 1; }
            case MonsterType::FireKnight:    { return 1; }
            case MonsterType::Ghost:         { return 1; }
            case MonsterType::Goblin:        { return 8; }
            case MonsterType::Hound:         { return 1; }
            case MonsterType::Imp:           { return 1; }
            case MonsterType::LittleKnight:  { return 1; }
            case MonsterType::Lizard:        { return 1; }
            case MonsterType::Medusa:        { return 1; }
            case MonsterType::Orc:           { return 1; }
            case MonsterType::Salamander:    { return 1; }
            case MonsterType::Skeleton:      { return 1; }
            case MonsterType::Skull:         { return 1; }
            case MonsterType::Snake:         { return 16; }
            case MonsterType::Spider:        { return 16; }
            case MonsterType::Vampire:       { return 1; }
            case MonsterType::Yeti:          { return 1; }
            case MonsterType::BossTribal:    { return 1; }
            case MonsterType::BossKnight:    { return 1; }
            case MonsterType::BossWizard:    { return 1; }
            case MonsterType::Count:         [[fallthrough]];    
            default:                         { return 0;  }
        }
        // clang-format on
    }

    // TODO need real sound effects here
    inline constexpr std::string_view hitSfx(const MonsterType t_type) noexcept
    {
        // clang-format off
        switch (t_type)
        {
            case MonsterType::BabyDragon:    { return ""; }
            case MonsterType::Bear:          { return ""; }
            case MonsterType::BigKnight:     { return ""; }
            case MonsterType::BoneDragon:    { return ""; }
            case MonsterType::Demon:         { return ""; }
            case MonsterType::Dino:          { return ""; }
            case MonsterType::Djinn:         { return ""; }
            case MonsterType::Dragon:        { return ""; }
            case MonsterType::Dwarf:         { return "hit-wood"; }
            case MonsterType::Ent:           { return "hit-wood"; }
            case MonsterType::FireImp:       { return ""; }
            case MonsterType::FireKnight:    { return ""; }
            case MonsterType::Ghost:         { return ""; }
            case MonsterType::Goblin:        { return "hit-wood"; }
            case MonsterType::Hound:         { return ""; }
            case MonsterType::Imp:           { return ""; }
            case MonsterType::LittleKnight:  { return ""; }
            case MonsterType::Lizard:        { return ""; }
            case MonsterType::Medusa:        { return ""; }
            case MonsterType::Orc:           { return "hit-wood"; }
            case MonsterType::Salamander:    { return ""; }
            case MonsterType::Skeleton:      { return "hit-wood"; }
            case MonsterType::Skull:         { return ""; }
            case MonsterType::Snake:         { return ""; }
            case MonsterType::Spider:        { return ""; }
            case MonsterType::Vampire:       { return ""; }
            case MonsterType::Yeti:          { return ""; }
            case MonsterType::BossTribal:    { return ""; }
            case MonsterType::BossKnight:    { return ""; }
            case MonsterType::BossWizard:    { return ""; }
            case MonsterType::Count:         [[fallthrough]];
            default:                         { return "";  }
        }
        // clang-format on
    }

    // TODO need to go and get real sfx here
    inline constexpr std::string_view attackSfx(const MonsterType t_type) noexcept
    {
        // clang-format off
        switch (t_type)
        {
            case MonsterType::BabyDragon:    { return ""; }
            case MonsterType::Bear:          { return ""; }
            case MonsterType::BigKnight:     { return ""; }
            case MonsterType::BoneDragon:    { return ""; }
            case MonsterType::Demon:         { return ""; }
            case MonsterType::Dino:          { return "attack-dino"; }
            case MonsterType::Djinn:         { return ""; }
            case MonsterType::Dragon:        { return "attack-dragon"; }
            case MonsterType::Dwarf:         { return ""; }
            case MonsterType::Ent:           { return "attack-ent"; }
            case MonsterType::FireImp:       { return ""; }
            case MonsterType::FireKnight:    { return ""; }
            case MonsterType::Ghost:         { return "swipe"; }
            case MonsterType::Goblin:        { return ""; }
            case MonsterType::Hound:         { return ""; }
            case MonsterType::Imp:           { return ""; }
            case MonsterType::LittleKnight:  { return ""; }
            case MonsterType::Lizard:        { return ""; }
            case MonsterType::Medusa:        { return ""; }
            case MonsterType::Orc:           { return ""; }
            case MonsterType::Salamander:    { return ""; }
            case MonsterType::Skeleton:      { return ""; }
            case MonsterType::Skull:         { return ""; }
            case MonsterType::Snake:         { return ""; }
            case MonsterType::Spider:        { return "attack-spider"; }
            case MonsterType::Vampire:       { return ""; }
            case MonsterType::Yeti:          { return ""; }
            case MonsterType::BossTribal:    { return ""; }
            case MonsterType::BossKnight:    { return ""; }
            case MonsterType::BossWizard:    { return ""; }
            case MonsterType::Count:         [[fallthrough]];
            default:                         { return "";  }
        }
        // clang-format on
    }

    // TODO need to go and get real sfx here
    inline constexpr std::string_view hurtSfx(const MonsterType t_type) noexcept
    {
        // clang-format off
        switch (t_type)
        {
            case MonsterType::BabyDragon:    { return ""; }
            case MonsterType::Bear:          { return ""; }
            case MonsterType::BigKnight:     { return ""; }
            case MonsterType::BoneDragon:    { return ""; }
            case MonsterType::Demon:         { return ""; }
            case MonsterType::Dino:          { return "hurt-dino"; }
            case MonsterType::Djinn:         { return ""; }
            case MonsterType::Dragon:        { return "hurt-dragon"; }
            case MonsterType::Dwarf:         { return ""; }
            case MonsterType::Ent:           { return "hurt-ent"; }
            case MonsterType::FireImp:       { return ""; }
            case MonsterType::FireKnight:    { return ""; }
            case MonsterType::Ghost:         { return ""; }
            case MonsterType::Goblin:        { return "hurt-goblin"; }
            case MonsterType::Hound:         { return ""; }
            case MonsterType::Imp:           { return ""; }
            case MonsterType::LittleKnight:  { return ""; }
            case MonsterType::Lizard:        { return ""; }
            case MonsterType::Medusa:        { return ""; }
            case MonsterType::Orc:           { return ""; }
            case MonsterType::Salamander:    { return ""; }
            case MonsterType::Skeleton:      { return ""; }
            case MonsterType::Skull:         { return ""; }
            case MonsterType::Snake:         { return ""; }
            case MonsterType::Spider:        { return "hurt-spider"; }
            case MonsterType::Vampire:       { return ""; }
            case MonsterType::Yeti:          { return ""; }
            case MonsterType::BossTribal:    { return ""; }
            case MonsterType::BossKnight:    { return ""; }
            case MonsterType::BossWizard:    { return ""; }
            case MonsterType::Count:         [[fallthrough]];
            default:                         { return "";  }
        }
        // clang-format on
    }

    // TODO need to go and get real sfx here
    inline constexpr std::string_view deathSfx(const MonsterType t_type) noexcept
    {
        // clang-format off
        switch (t_type)
        {
            case MonsterType::BabyDragon:    { return ""; }
            case MonsterType::Bear:          { return ""; }
            case MonsterType::BigKnight:     { return ""; }
            case MonsterType::BoneDragon:    { return ""; }
            case MonsterType::Demon:         { return ""; }
            case MonsterType::Dino:          { return "death-dino"; }
            case MonsterType::Djinn:         { return ""; }
            case MonsterType::Dragon:        { return "death-dragon"; }
            case MonsterType::Dwarf:         { return ""; }
            case MonsterType::Ent:           { return "death-ent"; }
            case MonsterType::FireImp:       { return ""; }
            case MonsterType::FireKnight:    { return ""; }
            case MonsterType::Ghost:         { return ""; }
            case MonsterType::Goblin:        { return "death-goblin"; }
            case MonsterType::Hound:         { return ""; }
            case MonsterType::Imp:           { return ""; }
            case MonsterType::LittleKnight:  { return ""; }
            case MonsterType::Lizard:        { return ""; }
            case MonsterType::Medusa:        { return ""; }
            case MonsterType::Orc:           { return ""; }
            case MonsterType::Salamander:    { return ""; }
            case MonsterType::Skeleton:      { return ""; }
            case MonsterType::Skull:         { return ""; }
            case MonsterType::Snake:         { return ""; }
            case MonsterType::Spider:        { return "death-spider"; }
            case MonsterType::Vampire:       { return ""; }
            case MonsterType::Yeti:          { return ""; }
            case MonsterType::BossTribal:    { return ""; }
            case MonsterType::BossKnight:    { return ""; }
            case MonsterType::BossWizard:    { return ""; }
            case MonsterType::Count:         [[fallthrough]];
            default:                         { return "";  }
        }
        // clang-format on
    }

    //

    enum class MonsterAnim : std::size_t
    {
        Attack = 0,
        Death,
        Hurt,
        Idle,
        Walk,
        Count
    };

    inline constexpr std::string_view toString(const MonsterAnim t_anim) noexcept
    {
        // clang-format off
        switch (t_anim)
        {
            case MonsterAnim::Attack: { return "attack"; }
            case MonsterAnim::Death:  { return "death";  }
            case MonsterAnim::Hurt:   { return "hurt";   }
            case MonsterAnim::Idle:   { return "idle";   }
            case MonsterAnim::Walk:   { return "walk";   }
            case MonsterAnim::Count:  [[fallthrough]];
            default:  { return "error_MonsterAnin_not_found"; }
        }
        // clang-format on
    }

    inline constexpr bool doesAnimLoop(const MonsterAnim t_anim) noexcept
    {
        return ((t_anim == MonsterAnim::Walk) || (t_anim == MonsterAnim::Idle));
    }

    //

    struct AttackInfo
    {
        explicit AttackInfo(const Health_t t_damage = 0, const sf::FloatRect & t_rect = {})
            : damage(t_damage)
            , rect(t_rect)
        {}

        Health_t damage;
        sf::FloatRect rect;
    };

    //

    struct IMonster
    {
        virtual ~IMonster() = default;

        virtual void update(Context & t_context, const float t_frameTimeSec)                 = 0;
        virtual void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const = 0;
        virtual void move(const float t_amount)                                              = 0;
        virtual Harm avatarCollide(const sf::FloatRect & t_avatarRect)                       = 0;
        virtual sf::FloatRect collisionRect() const                                          = 0;
        virtual sf::FloatRect attackCollisionRect() const                                    = 0;
        virtual bool avatarAttack(Context & t_context, const AttackInfo & t_attackInfo)      = 0;
    };

} // namespace bramblefore

#endif // IMONSTER_HPP_INCLUDED
