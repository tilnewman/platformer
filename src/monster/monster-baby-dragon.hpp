#ifndef MONSTER_BABY_DRAGON_HPP_INCLUDED
#define MONSTER_BABY_DRAGON_HPP_INCLUDED
//
// monster-baby-dragon.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class BabyDragon : public Monster
    {
      public:
        BabyDragon(Context & t_context, const sf::FloatRect & t_region);
        virtual ~BabyDragon() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace platformer

#endif // MONSTER_SALAMANDER_HPP_INCLUDED
