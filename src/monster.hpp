#ifndef MONSTER_HPP_INCLUDED
#define MONSTER_HPP_INCLUDED
//
// monster.hpp
//
#include "harm.hpp"

#include <vector>

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

    struct IMonster
    {
        virtual ~IMonster() = default;

        virtual void update(Context & context, const float frameTimeSec)                     = 0;
        virtual void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const = 0;
        virtual void move(const float amount)                                                = 0;
        virtual const Harm avatarCollide(const sf::FloatRect & avatarRect)                   = 0;
        virtual const sf::FloatRect collisionRect() const                                    = 0;
    };


} // namespace platformer

#endif // MONSTER_HPP_INCLUDED
