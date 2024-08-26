#ifndef MONSTER_SKELETON_HPP_INCLUDED
#define MONSTER_SKELETON_HPP_INCLUDED
//
// monster-skeleton.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class Skeleton : public Monster
    {
      public:
        Skeleton(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Skeleton() override = default;

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

#endif // MONSTER_SKELETON_HPP_INCLUDED
