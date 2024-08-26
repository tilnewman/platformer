#ifndef MONSTER_BOSS_WIZARD_HPP_INCLUDED
#define MONSTER_BOSS_WIZARD_HPP_INCLUDED
//
// monster-boss-wizard.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class BossWizard : public Monster
    {
      public:
        BossWizard(Context & t_context, const sf::FloatRect & t_region);
        virtual ~BossWizard() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void turnAround() final;
    };

} // namespace platformer

#endif // MONSTER_BOSS_WIZARD_HPP_INCLUDED
