#ifndef MONSTER_FIRE_KNIGHT_HPP_INCLUDED
#define MONSTER_FIRE_KNIGHT_HPP_INCLUDED
//
// monster-fire-knight.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class FireKnight : public Monster
    {
      public:
        FireKnight(Context & t_context, const sf::FloatRect & t_region);
        virtual ~FireKnight() override = default;

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

#endif // MONSTER_FIRE_KNIGHT_HPP_INCLUDED
