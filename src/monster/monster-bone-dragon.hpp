#ifndef MONSTER_BONE_DRAGON_HPP_INCLUDED
#define MONSTER_BONE_DRAGON_HPP_INCLUDED
//
// monster-bone-dragon.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class BoneDragon : public Monster
    {
      public:
        BoneDragon(Context & t_context, const sf::FloatRect & t_region);
        virtual ~BoneDragon() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace platformer

#endif // MONSTER_DRAGON_HPP_INCLUDED
