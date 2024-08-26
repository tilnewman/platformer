#ifndef MONSTER_DEMON_HPP_INCLUDED
#define MONSTER_DEMON_HPP_INCLUDED
//
// monster-demon.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class Demon : public Monster
    {
      public:
        Demon(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Demon() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void turnAround() final;
    };

} // namespace platformer

#endif // MONSTER_BIG_KNIGHT_HPP_INCLUDED
