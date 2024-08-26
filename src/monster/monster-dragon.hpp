#ifndef MONSTER_DRAGON_HPP_INCLUDED
#define MONSTER_DRAGON_HPP_INCLUDED
//
// monster-dragon.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class Dragon : public Monster
    {
      public:
        Dragon(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Dragon() override = default;

        // IMonster functions
        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void playAttackSfx(Context & t_context) const final;
        void playHurtSfx(Context & t_context) const final;
        void playDeathSfx(Context & t_context) const final;
        void startAttackAnimation(Context & t_context) final;
    };

} // namespace platformer

#endif // MONSTER_DRAGON_HPP_INCLUDED
