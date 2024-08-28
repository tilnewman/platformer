#ifndef MONSTER_FIRE_IMP_HPP_INCLUDED
#define MONSTER_FIRE_IMP_HPP_INCLUDED
//
// monster-fire-imp.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class FireImp : public Monster
    {
      public:
        FireImp(Context & t_context, const sf::FloatRect & t_region);
        virtual ~FireImp() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace bramblefore

#endif // MONSTER_FIRE_IMP_HPP_INCLUDED
