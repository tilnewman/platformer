#ifndef MONSTER_SALAMANDER_HPP_INCLUDED
#define MONSTER_SALAMANDER_HPP_INCLUDED
//
// monster-salamander.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Salamander : public Monster
    {
      public:
        Salamander(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Salamander() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace bramblefore

#endif // MONSTER_SALAMANDER_HPP_INCLUDED
