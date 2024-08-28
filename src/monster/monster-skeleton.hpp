#ifndef MONSTER_SKELETON_HPP_INCLUDED
#define MONSTER_SKELETON_HPP_INCLUDED
//
// monster-skeleton.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Skeleton : public Monster
    {
      public:
        Skeleton(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Skeleton() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void turnAround() final;
    };

} // namespace bramblefore

#endif // MONSTER_SKELETON_HPP_INCLUDED
