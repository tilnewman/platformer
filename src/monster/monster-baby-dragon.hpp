#ifndef MONSTER_BABY_DRAGON_HPP_INCLUDED
#define MONSTER_BABY_DRAGON_HPP_INCLUDED
//
// monster-baby-dragon.hpp
//
#include "monster/monster.hpp"
#include "subsystem/harm.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class BabyDragon : public Monster
    {
      public:
        BabyDragon(Context & t_context, const sf::FloatRect & t_region);
        virtual ~BabyDragon() override = default;

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

#endif // MONSTER_SALAMANDER_HPP_INCLUDED
