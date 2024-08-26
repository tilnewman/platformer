#ifndef MONSTER_SNAKE_HPP_INCLUDED
#define MONSTER_SNAKE_HPP_INCLUDED
//
// monster-snake.hpp
//
#include "subsystem/harm.hpp"
#include "monster/monster.hpp"

#include <filesystem>
#include <string>

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class Snake : public Monster
    {
      public:
        Snake(Context & context, const sf::FloatRect & region);
        virtual ~Snake() override = default;

        // IMonster functions
        const Harm avatarCollide(const sf::FloatRect & avatarRect) final;
        const sf::FloatRect collisionRect() const final;
        const sf::FloatRect attackCollisionRect() const final;

      protected:
        inline float walkSpeed() const final { return 30.0f; }
        void playAttackSfx(Context & context) const final;
        void playHurtSfx(Context & context) const final;
        void playDeathSfx(Context & context) const final;
        void turnAround() final;
    };

} // namespace platformer

#endif // MONSTER_DINO_HPP_INCLUDED
