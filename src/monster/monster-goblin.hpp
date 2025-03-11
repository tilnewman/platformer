#ifndef MONSTER_GOBLIN_HPP_INCLUDED
#define MONSTER_GOBLIN_HPP_INCLUDED
//
// monster-goblin.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Goblin : public Monster
    {
      public:
        Goblin(const Context & t_context, const sf::FloatRect & t_region);
        virtual ~Goblin() override = default;

        // IMonster functions
        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace bramblefore

#endif // MONSTER_GOBLIN_HPP_INCLUDED
