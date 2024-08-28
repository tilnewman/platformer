#ifndef MONSTER_BOSS_TRIBAL_HPP_INCLUDED
#define MONSTER_BOSS_TRIBAL_HPP_INCLUDED
//
// monster-boss-tribal.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class BossTribal : public Monster
    {
      public:
        BossTribal(Context & t_context, const sf::FloatRect & t_region);
        virtual ~BossTribal() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void turnAround() final;
    };

} // namespace bramblefore

#endif // MONSTER_BOSS_TRIBAL_HPP_INCLUDED
