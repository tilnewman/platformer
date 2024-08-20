#ifndef MONSTER_FIRE_KNIGHT_HPP_INCLUDED
#define MONSTER_FIRE_KNIGHT_HPP_INCLUDED
//
// monster-fire-knight.hpp
//
#include "harm.hpp"
#include "monster.hpp"

#include <filesystem>
#include <string>

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;

    //

    class FireKnight : public Monster
    {
      public:
        FireKnight(Context & context, const sf::FloatRect & region);
        virtual ~FireKnight() override = default;

        // IMonster functions
        const Harm avatarCollide(const sf::FloatRect & avatarRect) final;
        const sf::FloatRect collisionRect() const final;
        const sf::FloatRect attackCollisionRect() const final;

      protected:
        inline float walkSpeed() const final { return 40.0f; }
        void playAttackSfx(Context & context) const final;
        void playHurtSfx(Context & context) const final;
        void playDeathSfx(Context & context) const final;
        void turnAround() final;
    };

} // namespace platformer

#endif // MONSTER_FIRE_KNIGHT_HPP_INCLUDED
