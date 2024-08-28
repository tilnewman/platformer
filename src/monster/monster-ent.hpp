#ifndef MONSTER_ENT_HPP_INCLUDED
#define MONSTER_ENT_HPP_INCLUDED
//
// monster-ent.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Ent : public Monster
    {
      public:
        Ent(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Ent() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace bramblefore

#endif // MONSTER_ENT_HPP_INCLUDED
