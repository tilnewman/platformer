#ifndef IMONSTER_HPP_INCLUDED
#define IMONSTER_HPP_INCLUDED
//
// imonster.hpp
//
#include "player/custom-types.hpp"
#include "subsystem/harm.hpp"

#include <string>

#include <SFML/Graphics/Rect.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
} // namespace sf

namespace platformer
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
            case MonsterType::Count:            //intentional fallthrough      
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
            case MonsterType::Count://intentional fallthrough      
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
            case MonsterType::Dragon:        { return 50.0f; }
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
            case MonsterType::Count: //intentional fallthrough      
            default:                         { return 0.0f;  }
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
            case MonsterAnim::Attack:      { return "attack"; }
            case MonsterAnim::Death:       { return "death";  }
            case MonsterAnim::Hurt:        { return "hurt";   }
            case MonsterAnim::Idle:        { return "idle";   }
            case MonsterAnim::Walk:        { return "walk";   }
            case MonsterAnim::Count: // intentional fallthrough
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
        virtual const Harm avatarCollide(const sf::FloatRect & t_avatarRect)                 = 0;
        virtual const sf::FloatRect collisionRect() const                                    = 0;
        virtual const sf::FloatRect attackCollisionRect() const                              = 0;
        virtual bool avatarAttack(Context & t_context, const AttackInfo & t_attackInfo)      = 0;
    };

} // namespace platformer

#endif // IMONSTER_HPP_INCLUDED
