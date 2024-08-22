#ifndef IMONSTER_HPP_INCLUDED
#define IMONSTER_HPP_INCLUDED
//
// imonster.hpp
//
#include "custom-types.hpp"
#include "harm.hpp"

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
        Count
    };

    inline constexpr std::string_view toString(const MonsterType type)
    {
        // clang-format off
        switch (type)
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
            case MonsterType::Count:            //intentional fallthrough      
            default:              { return "Error_unknown_MonsterType"; }
        }
        // clang-format on
    }

    inline constexpr Health_t startingHealth(const MonsterType type)
    {
        // clang-format off
        switch (type)
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
            case MonsterType::Count://intentional fallthrough      
            default:                         { return 0;   }
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

    inline constexpr std::string_view toString(const MonsterAnim anim)
    {
        // clang-format off
        switch (anim)
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

    inline constexpr bool doesAnimLoop(const MonsterAnim anim)
    {
        return ((anim == MonsterAnim::Walk) || (anim == MonsterAnim::Idle));
    }

    //

    struct AttackInfo
    {
        explicit AttackInfo(const Health_t attackDamage = 0, const sf::FloatRect & avatarAttackRect = {})
            : damage(attackDamage)
            , rect(avatarAttackRect)
        {}

        Health_t damage;
        sf::FloatRect rect;
    };

    //

    struct IMonster
    {
        virtual ~IMonster() = default;

        virtual void update(Context & context, const float frameTimeSec)                     = 0;
        virtual void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const = 0;
        virtual void move(const float amount)                                                = 0;
        virtual const Harm avatarCollide(const sf::FloatRect & avatarRect)                   = 0;
        virtual const sf::FloatRect collisionRect() const                                    = 0;
        virtual const sf::FloatRect attackCollisionRect() const                              = 0;
        virtual bool avatarAttack(Context & context, const AttackInfo & attackInfo)          = 0;
    };

} // namespace platformer

#endif // IMONSTER_HPP_INCLUDED
