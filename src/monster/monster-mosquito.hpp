#ifndef MONSTER_MOSQUITO_HPP_INCLUDED
#define MONSTER_MOSQUITO_HPP_INCLUDED
//
// monster-mosquito.hpp
//
#include "imonster.hpp"
#include "subsystem/harm.hpp"

#include <vector>
#include <string_view>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
    class RenderTarget;
} // namespace sf

namespace bramblefore
{

    struct Context;

    //

    enum class MosquitoAnim : unsigned char
    {
        Idle = 0,
        Flying,
        AttackPrep,
        AttackCycle,
        AttackReset,
        Hurt,
        Death,
        Count
    };

    [[nodiscard]] constexpr std::string_view toString(const MosquitoAnim t_anim) noexcept
    {
        // clang-format off
        switch (t_anim)
        {
            case MosquitoAnim::Idle:         { return "idle";         }
            case MosquitoAnim::Flying:       { return "flying";       }
            case MosquitoAnim::AttackPrep:   { return "attack-prep";  }
            case MosquitoAnim::AttackCycle:  { return "attack-cycle"; }
            case MosquitoAnim::AttackReset:  { return "attack-reset"; }
            case MosquitoAnim::Hurt:         { return "hurt";         }
            case MosquitoAnim::Death:        { return "death";        }
            case MosquitoAnim::Count:        [[fallthrough]];
            default:           { return "Error_MosquitoAnim_Unknown"; }
        }
        // clang-format on
    }

    //

    class Mosquito : public IMonster
    {
      public:
        Mosquito(const Context & t_context, const sf::FloatRect & t_region);
        virtual ~Mosquito() override;

        // IMonster functions
        [[nodiscard]] inline MonsterType type() const final { return MonsterType::Mosquito; }
        void setup(const Context & t_context) final;
        void update(const Context & t_context, const float t_frameTimeSec) override;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const override;

        void move(const sf::Vector2f & t_move) override;
        bool avatarAttack(const Context & t_context, const AttackInfo & t_attackInfo) override;

        const Harm avatarCollide(const sf::FloatRect & t_avatarRect) override;

        const sf::FloatRect collisionRect() const final;
        const sf::FloatRect attackCollisionRect() const final;

      private:

      private:
        MosquitoAnim m_anim;
        std::vector<std::vector<sf::Texture>> m_animTextures;
        sf::Sprite m_sprite;
        float m_elapsedTimeSec;
        sf::FloatRect m_flyBounds;
        sf::Vector2f m_spawnPosition;
    };

} // namespace bramblefore

#endif // MONSTER_MOSQUITO_HPP_INCLUDED
