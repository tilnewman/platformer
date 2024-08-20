#ifndef MONSTER_FIRE_IMP_HPP_INCLUDED
#define MONSTER_FIRE_IMP_HPP_INCLUDED
//
// monster-fire-imp.hpp
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

    class FireImp : public Monster
    {
      public:
        FireImp(Context & context, const sf::FloatRect & region);
        virtual ~FireImp() override = default;

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

#endif // MONSTER_FIRE_IMP_HPP_INCLUDED
