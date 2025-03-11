#ifndef MONSTER_BEAR_HPP_INCLUDED
#define MONSTER_BEAR_HPP_INCLUDED
//
// monster-bear.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Bear : public Monster
    {
      public:
        Bear(const Context & t_context, const sf::FloatRect & t_region);
        virtual ~Bear() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void turnAround() final;
    };

} // namespace bramblefore

#endif // MONSTER_BEAR_HPP_INCLUDED
