#ifndef MONSTER_SNAKE_HPP_INCLUDED
#define MONSTER_SNAKE_HPP_INCLUDED
//
// monster-snake.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Snake : public Monster
    {
      public:
        Snake(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Snake() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void turnAround() final;
    };

} // namespace bramblefore

#endif // MONSTER_DINO_HPP_INCLUDED
