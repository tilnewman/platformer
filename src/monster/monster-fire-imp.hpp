#ifndef MONSTER_FIRE_IMP_HPP_INCLUDED
#define MONSTER_FIRE_IMP_HPP_INCLUDED
//
// monster-fire-imp.hpp
//
#include "monster/monster.hpp"
#include "subsystem/harm.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class FireImp : public Monster
    {
      public:
        FireImp(Context & t_context, const sf::FloatRect & t_region);
        virtual ~FireImp() override = default;

        // IMonster functions
        const Harm avatarCollide(const sf::FloatRect & t_avatarRect) final;
        const sf::FloatRect collisionRect() const final;
        const sf::FloatRect attackCollisionRect() const final;

      protected:
        void playAttackSfx(Context & t_context) const final;
        void playHurtSfx(Context & t_context) const final;
        void playDeathSfx(Context & t_context) const final;
    };

} // namespace platformer

#endif // MONSTER_FIRE_IMP_HPP_INCLUDED
