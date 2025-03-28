#ifndef MONSTER_DJINN_HPP_INCLUDED
#define MONSTER_DJINN_HPP_INCLUDED
//
// monster-djinn.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Djinn : public Monster
    {
      public:
        Djinn(const Context & t_context, const sf::FloatRect & t_region);
        virtual ~Djinn() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void turnAround() final;
    };

} // namespace bramblefore

#endif // MONSTER_DJINN_HPP_INCLUDED
