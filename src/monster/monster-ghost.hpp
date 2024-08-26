#ifndef MONSTER_GHOST_HPP_INCLUDED
#define MONSTER_GHOST_HPP_INCLUDED
//
// monster-ghost.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class Ghost : public Monster
    {
      public:
        Ghost(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Ghost() override = default;

        // IMonster functions
        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void playAttackSfx(Context & t_context) const final;
        void playHurtSfx(Context & t_context) const final;
        void playDeathSfx(Context & t_context) const final;
    };

} // namespace platformer

#endif // MONSTER_GHOST_HPP_INCLUDED
