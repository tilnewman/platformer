#ifndef MONSTER_ENT_HPP_INCLUDED
#define MONSTER_ENT_HPP_INCLUDED
//
// monster-ent.hpp
//
#include "monster/monster.hpp"
#include "subsystem/harm.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class Ent : public Monster
    {
      public:
        Ent(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Ent() override = default;

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

#endif // MONSTER_ENT_HPP_INCLUDED
