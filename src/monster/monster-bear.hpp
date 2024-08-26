#ifndef MONSTER_BEAR_HPP_INCLUDED
#define MONSTER_BEAR_HPP_INCLUDED
//
// monster-bear.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class Bear : public Monster
    {
      public:
        Bear(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Bear() override = default;

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

#endif // MONSTER_BEAR_HPP_INCLUDED
