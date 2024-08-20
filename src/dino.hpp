#ifndef DINO_HPP_INCLUDED
#define DINO_HPP_INCLUDED
//
// dino.hpp
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

    class Dino : public Monster
    {
      public:
        explicit Dino(Context & context, const sf::FloatRect & region);
        virtual ~Dino() override = default;

        // IMonster functions
        const Harm avatarCollide(const sf::FloatRect & avatarRect) override;
        const sf::FloatRect collisionRect() const override;
        const sf::FloatRect attackCollisionRect() const override;

      protected:
        inline float walkSpeed() const override { return 30.0f; }
        void playAttackSfx(Context & context) const override;
        void playHurtSfx(Context & context) const override;
        void playDeathSfx(Context & context) const override;
    };

} // namespace platformer

#endif // DINO_HPP_INCLUDED
