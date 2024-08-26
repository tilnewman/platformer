#ifndef MONSTER_YETI_HPP_INCLUDED
#define MONSTER_YETI_HPP_INCLUDED
//
// monster-yeti.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class Yeti : public Monster
    {
      public:
        Yeti(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Yeti() override = default;

        // IMonster functions
        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void playAttackSfx(Context & t_context) const final;
        void playHurtSfx(Context & t_context) const final;
        void playDeathSfx(Context & t_context) const final;
    };

} // namespace platformer

#endif // MONSTER_YETI_HPP_INCLUDED
