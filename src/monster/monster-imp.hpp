#ifndef MONSTER_IMP_HPP_INCLUDED
#define MONSTER_IMP_HPP_INCLUDED
//
// monster-imp.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Imp : public Monster
    {
      public:
        Imp(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Imp() override = default;

        // IMonster functions
        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace bramblefore

#endif // MONSTER_IMP_HPP_INCLUDED
