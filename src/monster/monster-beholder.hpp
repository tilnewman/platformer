#ifndef MONSTER_BEHOLDER_HPP_INCLUDED
#define MONSTER_BEHOLDER_HPP_INCLUDED
//
// monster-beholder.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Beholder : public Monster
    {
      public:
        Beholder(const Context & t_context, const sf::FloatRect & t_region);
        virtual ~Beholder() override = default;

        const sf::FloatRect collisionRect() const final;
        const sf::FloatRect attackCollisionRect() const final;

      protected:
        void turnAround() final;
    };

} // namespace bramblefore

#endif // MONSTER_BEHOLDER_HPP_INCLUDED
