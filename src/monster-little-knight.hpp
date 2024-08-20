#ifndef MONSTER_LITTLE_KNIGHT_HPP_INCLUDED
#define MONSTER_LITTLE_KNIGHT_HPP_INCLUDED
//
// monster-little-knight.hpp
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

    class LittleKnight : public Monster
    {
      public:
        LittleKnight(Context & context, const sf::FloatRect & region);
        virtual ~LittleKnight() override = default;

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

#endif // MONSTER_LITTLE_KNIGHT_HPP_INCLUDED
