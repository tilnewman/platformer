#ifndef MONSTER_BIG_KNIGHT_HPP_INCLUDED
#define MONSTER_BIG_KNIGHT_HPP_INCLUDED
//
// monster-big-knight.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class BigKnight : public Monster
    {
      public:
        BigKnight(const Context & t_context, const sf::FloatRect & t_region);
        virtual ~BigKnight() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace bramblefore

#endif // MONSTER_BIG_KNIGHT_HPP_INCLUDED
