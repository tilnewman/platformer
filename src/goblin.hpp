#ifndef GOBLIN_HPP_INCLUDED
#define GOBLIN_HPP_INCLUDED
//
// goblin.hpp
//
#include "harm.hpp"
#include "monster.hpp"

#include <filesystem>
#include <string>

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Context;
    struct Settings;

    //

    class Goblin : public Monster
    {
      public:
        Goblin(Context & context, const sf::FloatRect & region);
        virtual ~Goblin() override = default;

        // IMonster functions
        const Harm avatarCollide(const sf::FloatRect & avatarRect) override;
        const sf::FloatRect collisionRect() const override;
        const sf::FloatRect attackCollisionRect() const override;

      protected:
        inline float walkSpeed() const override { return 40.0f; }
        void playAttackSfx(Context & context) const override;
        void playHurtSfx(Context & context) const override;
        void playDeathSfx(Context & context) const override;
    };

} // namespace platformer

#endif // GOBLIN_HPP_INCLUDED
