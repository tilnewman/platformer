#ifndef MONSTER_SALAMANDER_HPP_INCLUDED
#define MONSTER_SALAMANDER_HPP_INCLUDED
//
// monster-salamander.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class Salamander : public Monster
    {
      public:
        Salamander(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Salamander() override = default;

        // IMonster functions
        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void playAttackSfx(Context & t_context) const final;
        void playHurtSfx(Context & t_context) const final;
        void playDeathSfx(Context & t_context) const final;
    };

} // namespace platformer

#endif // MONSTER_SALAMANDER_HPP_INCLUDED
