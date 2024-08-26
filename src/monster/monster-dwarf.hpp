#ifndef MONSTER_DWARF_HPP_INCLUDED
#define MONSTER_DWARF_HPP_INCLUDED
//
// monster-dwarf.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class Dwarf : public Monster
    {
      public:
        Dwarf(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Dwarf() override = default;

        // IMonster functions
        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void playAttackSfx(Context & t_context) const final;
        void playHurtSfx(Context & t_context) const final;
        void playDeathSfx(Context & t_context) const final;
        void turnAround() final;
    };

} // namespace platformer

#endif // MONSTER_BEAR_HPP_INCLUDED
