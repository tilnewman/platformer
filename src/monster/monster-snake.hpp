#ifndef MONSTER_SNAKE_HPP_INCLUDED
#define MONSTER_SNAKE_HPP_INCLUDED
//
// monster-snake.hpp
//
#include "monster/monster.hpp"
#include "subsystem/harm.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class Snake : public Monster
    {
      public:
        Snake(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Snake() override = default;

        // IMonster functions
        const Harm avatarCollide(const sf::FloatRect & t_avatarRect) final;
        const sf::FloatRect collisionRect() const final;
        const sf::FloatRect attackCollisionRect() const final;

      protected:
        void playAttackSfx(Context & t_context) const final;
        void playHurtSfx(Context & t_context) const final;
        void playDeathSfx(Context & t_context) const final;
        void turnAround() final;
    };

} // namespace platformer

#endif // MONSTER_DINO_HPP_INCLUDED
