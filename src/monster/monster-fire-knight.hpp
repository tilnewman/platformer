#ifndef MONSTER_FIRE_KNIGHT_HPP_INCLUDED
#define MONSTER_FIRE_KNIGHT_HPP_INCLUDED
//
// monster-fire-knight.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class FireKnight : public Monster
    {
      public:
        FireKnight(const Context & t_context, const sf::FloatRect & t_region);
        virtual ~FireKnight() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void turnAround() final;
    };

} // namespace bramblefore

#endif // MONSTER_FIRE_KNIGHT_HPP_INCLUDED
