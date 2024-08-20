#ifndef MONSTER_SKELETON_HPP_INCLUDED
#define MONSTER_SKELETON_HPP_INCLUDED
//
// monster-skeleton.hpp
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

    class Skeleton : public Monster
    {
      public:
        Skeleton(Context & context, const sf::FloatRect & region);
        virtual ~Skeleton() override = default;

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

#endif // MONSTER_SKELETON_HPP_INCLUDED
