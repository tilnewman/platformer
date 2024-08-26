#ifndef MONSTER_ORC_HPP_INCLUDED
#define MONSTER_ORC_HPP_INCLUDED
//
// monster-orc.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class Orc : public Monster
    {
      public:
        Orc(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Orc() override = default;

        // IMonster functions
        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void playAttackSfx(Context & t_context) const final;
        void playHurtSfx(Context & t_context) const final;
        void playDeathSfx(Context & t_context) const final;
        void turnAround() final;
    };

} // namespace platformer

#endif // MONSTER_BIG_KNIGHT_HPP_INCLUDED
