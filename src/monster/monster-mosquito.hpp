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
#include <SFML/Graphics/Text.hpp>
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
        else if ((MosquitoAnim::Hurt == t_anim) || (MosquitoAnim::Death == t_anim))
        {
            return 0.25f;
        }
        else if (MosquitoAnim::AttackPrep == t_anim)
        {
            return 0.2f;
        }
        else
        {
            return 0.1f;
        }
    }

    enum class MosquitoTask
    {
        Idle,
        Wander,
        Attack,
        Reset,
        Death,
        Hurt
    };

    [[nodiscard]] constexpr std::string_view toString(const MosquitoTask t_task) noexcept
    {
        // clang-format off
        switch (t_task)
        {
            case MosquitoTask::Idle:    { return "idle";    }
            case MosquitoTask::Wander:  { return "wander";  }
            case MosquitoTask::Attack:  { return "attack";  }
            case MosquitoTask::Reset:   { return "reset";   }
            case MosquitoTask::Death:   { return "death";   }
            case MosquitoTask::Hurt:    { return "hurt";   }
            default: { return "Error_MosquitoTask_Unknown"; }
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
        [[nodiscard]] bool isAlive() const final { return m_isAlive; }
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

        void setupTask(
            const Context & t_context, const MosquitoTask t_task, const MosquitoAnim t_anim);

        [[nodiscard]] float flyingSpeed(const MosquitoTask t_task) const;
        [[nodiscard]] const sf::FloatRect spottedRect(const Context & t_context) const;
        void handleDying(const Context & t_context);

      private:
        Health_t m_health;
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
        float m_speedMult;
        bool m_isAlive;
        float m_resetDistance;
        mutable sf::Text m_debugText;
    };

} // namespace bramblefore

#endif // MONSTER_MOSQUITO_HPP_INCLUDED
