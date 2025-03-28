#ifndef MONSTER_DEMON_HPP_INCLUDED
#define MONSTER_DEMON_HPP_INCLUDED
//
// monster-demon.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Demon : public Monster
    {
      public:
        Demon(const Context & t_context, const sf::FloatRect & t_region);
        virtual ~Demon() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void turnAround() final;
    };

} // namespace bramblefore

#endif // MONSTER_BIG_KNIGHT_HPP_INCLUDED
