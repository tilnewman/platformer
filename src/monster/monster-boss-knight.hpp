#ifndef MONSTER_BOSS_KNIGHT_HPP_INCLUDED
#define MONSTER_BOSS_KNIGHT_HPP_INCLUDED
//
// monster-boss-knight.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class BossKnight : public Monster
    {
      public:
        BossKnight(Context & t_context, const sf::FloatRect & t_region);
        virtual ~BossKnight() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void turnAround() final;
    };

} // namespace platformer

#endif // MONSTER_BOSS_KNIGHT_HPP_INCLUDED
