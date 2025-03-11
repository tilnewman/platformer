#ifndef MONSTER_SKULL_HPP_INCLUDED
#define MONSTER_SKULL_HPP_INCLUDED
//
// monster-skull.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Skull : public Monster
    {
      public:
        Skull(const Context & t_context, const sf::FloatRect & t_region);
        virtual ~Skull() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace bramblefore

#endif // MONSTER_SKULL_HPP_INCLUDED
