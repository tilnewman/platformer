#ifndef MONSTER_MOSQUITO_HPP_INCLUDED
#define MONSTER_MOSQUITO_HPP_INCLUDED
//
// monster-mosquito.hpp
//
#include "imonster.hpp"
#include "subsystem/harm.hpp"

#include <string_view>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

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

    [[nodiscard]] constexpr float toTimeBetweenFrames(const MosquitoAnim t_anim) noexcept
    {
        if (MosquitoAnim::Idle == t_anim)
        {
            return 0.15f;
        }
        else if (MosquitoAnim::Flying == t_anim)
        {
            return 0.05f;
        }
        else if (MosquitoAnim::AttackCycle == t_anim)
        {
            return 0.075f;
        }
        else
        {
            return 0.1f;
        }
    }

    enum class MosquitoTask
    {
        // before spotting the player
        Idle,
        Wander,

        // after spotting the player
        Attack,
        Reset
    };

    //

    class Mosquito : public IMonster
    {
      public:
        Mosquito(const Context & t_context, const sf::FloatRect & t_region);
        virtual ~Mosquito() override;

        // IMonster functions
        [[nodiscard]] inline MonsterType type() const final { return MonsterType::Mosquito; }
        void setup(const Context & t_context) final;
        void update(const Context & t_context, const float t_elapsedTimeSec) override;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const override;

        void move(const sf::Vector2f & t_move) override;
        bool avatarAttack(const Context & t_context, const AttackInfo & t_attackInfo) override;

        const Harm avatarCollide(const sf::FloatRect & t_avatarRect) override;

        const sf::FloatRect collisionRect() const final;
        const sf::FloatRect attackCollisionRect() const final;

      private:
        // pick a random postion in the top half of m_flyBounds
        [[nodiscard]] const sf::Vector2f randomResetPosition(const Context & t_context) const;

        void turnToFacePlayer(const Context & t_context);
        void turnToFacePosition(const sf::Vector2f & t_position);
        void turnAround();
        [[nodiscard]] float randomIdleDurationSec(const Context & t_context) const;

      private:
        MosquitoAnim m_anim;
        MosquitoTask m_task;
        std::vector<std::vector<sf::Texture>> m_animTextures;
        sf::Sprite m_sprite;
        float m_elapsedAnimTimeSec;
        float m_taskTimeRemainingSec;
        sf::FloatRect m_flyBounds;
        bool m_isFacingRight;
        bool m_hasSpottedPlayer;
        sf::Vector2f m_resetPosition;
        std::size_t m_frameIndex;
    };

} // namespace bramblefore

#endif // MONSTER_MOSQUITO_HPP_INCLUDED
