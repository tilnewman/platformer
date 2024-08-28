#ifndef MONSTER_VAMPIRE_HPP_INCLUDED
#define MONSTER_VAMPIRE_HPP_INCLUDED
//
// monster-vampire.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Vampire : public Monster
    {
      public:
        Vampire(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Vampire() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void turnAround() final;
    };

} // namespace bramblefore

#endif // MONSTER_VAMPIRE_HPP_INCLUDED
