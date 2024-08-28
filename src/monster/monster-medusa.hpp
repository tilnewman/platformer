#ifndef MONSTER_MEDUSA_HPP_INCLUDED
#define MONSTER_MEDUSA_HPP_INCLUDED
//
// monster-medusa.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Medusa : public Monster
    {
      public:
        Medusa(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Medusa() override = default;

        // IMonster functions
        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace bramblefore

#endif // MONSTER_MEDUSA_HPP_INCLUDED
