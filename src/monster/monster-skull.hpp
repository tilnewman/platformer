#ifndef MONSTER_SKULL_HPP_INCLUDED
#define MONSTER_SKULL_HPP_INCLUDED
//
// monster-skull.hpp
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

    class Skull : public Monster
    {
      public:
        Skull(Context & context, const sf::FloatRect & region);
        virtual ~Skull() override = default;

        // IMonster functions
        const Harm avatarCollide(const sf::FloatRect & avatarRect) final;
        const sf::FloatRect collisionRect() const final;
        const sf::FloatRect attackCollisionRect() const final;

      protected:
        inline float walkSpeed() const final { return 30.0f; }
        void playAttackSfx(Context & context) const final;
        void playHurtSfx(Context & context) const final;
        void playDeathSfx(Context & context) const final;
    };

} // namespace platformer

#endif // MONSTER_SKULL_HPP_INCLUDED
