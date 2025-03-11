#ifndef MONSTER_LIZARD_HPP_INCLUDED
#define MONSTER_LIZARD_HPP_INCLUDED
//
// monster-lizard.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Lizard : public Monster
    {
      public:
        Lizard(const Context & t_context, const sf::FloatRect & t_region);
        virtual ~Lizard() override = default;

        // IMonster functions
        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace bramblefore

#endif // MONSTER_LIZARD_HPP_INCLUDED
