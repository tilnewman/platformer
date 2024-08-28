#ifndef MONSTER_GHOST_HPP_INCLUDED
#define MONSTER_GHOST_HPP_INCLUDED
//
// monster-ghost.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Ghost : public Monster
    {
      public:
        Ghost(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Ghost() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace bramblefore

#endif // MONSTER_GHOST_HPP_INCLUDED
