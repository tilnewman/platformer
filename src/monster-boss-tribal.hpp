#ifndef MONSTER_BOSS_TRIBAL_HPP_INCLUDED
#define MONSTER_BOSS_TRIBAL_HPP_INCLUDED
//
// monster-boss-tribal.hpp
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

    class BossTribal : public Monster
    {
      public:
        BossTribal(Context & context, const sf::FloatRect & region);
        virtual ~BossTribal() override = default;

        // IMonster functions
        const Harm avatarCollide(const sf::FloatRect & avatarRect) final;
        const sf::FloatRect collisionRect() const final;
        const sf::FloatRect attackCollisionRect() const final;

      protected:
        inline float walkSpeed() const final { return 55.0f; }
        void playAttackSfx(Context & context) const final;
        void playHurtSfx(Context & context) const final;
        void playDeathSfx(Context & context) const final;
        void turnAround() final;
    };

} // namespace platformer

#endif // MONSTER_BIG_KNIGHT_HPP_INCLUDED
