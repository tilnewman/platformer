#ifndef AVATAR_HPP_INCLUDED
#define AVATAR_HPP_INCLUDED
//
// avatar.hpp
//
#include "avatar/avatar-textures.hpp"
#include "player/player-info.hpp"
#include "subsystem/harm.hpp"
#include "subsystem/run-particle-effect.hpp"

#include <string_view>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
    class RenderTarget;
} // namespace sf

namespace bramblefore
{
    struct Context;
    struct Settings;

    //

    enum class AvatarState
    {
        Attack,
        AttackExtra,
        Climb,
        Death,
        Hurt,
        Idle,
        Jump,
        JumpHigh,
        Push,
        Run,
        Walk,
        Still
    };

    inline constexpr std::string_view toString(const AvatarState t_state) noexcept
    {
        // clang-format off
        switch (t_state)
        {
            case AvatarState::Attack:       { return "Attack";      }
            case AvatarState::AttackExtra:  { return "AttackExtra"; }
            case AvatarState::Climb:        { return "Climb";       }
            case AvatarState::Death:        { return "Death";       }
            case AvatarState::Hurt:         { return "Hurt";        }
            case AvatarState::Idle:         { return "Idle";        }
            case AvatarState::Jump:         { return "Jump";        }
            case AvatarState::JumpHigh:     { return "JumpHigh";    }
            case AvatarState::Push:         { return "Push";        }
            case AvatarState::Run:          { return "Run";         }
            case AvatarState::Walk:         { return "Walk";        }
            case AvatarState::Still:        { return "Still";       }
            default: { return "Error_toString_unknown_AvatarState"; }
        }
        // clang-format on
    }

    //

    struct MovementDetails
    {
        float walk_speed_limit{ 0.0f };
        float run_speed_limit{ 0.0f };
        float walk_acc{ 0.0f };
        float run_acc{ 0.0f };
        float ladder_speed{ 0.0f };
        float jump_acc{ 0.0f };
        float high_jump_acc{ 0.0f };
    };

    //

    class Avatar
    {
      public:
        Avatar();
        ~Avatar();

        void setup(const Context & t_context);
        void update(const Context & t_context, const float t_frameTimeSec);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states);
        void setPosition(const sf::FloatRect & t_rect);
        [[nodiscard]] const sf::FloatRect collisionRect() const;
        [[nodiscard]] const sf::FloatRect attackRect() const;
        [[nodiscard]] const sf::FloatRect footCollisionRect() const;
        void triggerDeath(const Context & t_context);
        void triggerIdle();

        // TODO remove after testing
        void changeType(const Context & t_context);

      private:
        void animate(const Context & t_context, const float t_frameTimeSec);
        void handleClimbing(const Context & t_context, const float t_frameTimeSec);
        void handleAttackState(const Context & t_context);
        void handleAttackingEnemies(const Context & t_context);
        void moveMap(const Context & t_context);
        void killIfOutOfBounds(const Context & t_context);
        void preventBacktracking(const Context & t_context);
        void collisions(const Context & t_context);
        void sideToSideMotion(const Context & t_context, const float t_frameTimeSec);
        void restartAnim();
        void jumping(const Context & t_context, const float t_frameTimeSec);
        bool handleDeath(const Context & t_context, const float t_frameTimeSec);
        void respawn(const Context & t_context);
        void turnRight();
        void turnLeft();
        void exitCollisions(const Context & t_context) const;
        void hurtCollisions(const Context & t_context);
        void killCollisions(const Context & t_context);
        void harm(const Context & t_context, const Harm & t_harm);
        [[nodiscard]] MovementDetails calculateMovementDetails(const Context & t_context) const;

        void collide(
            const Context & t_context,
            const sf::FloatRect & t_avatarRect,
            const sf::FloatRect & t_avatarFootRect,
            const sf::FloatRect & t_collRect,
            const sf::FloatRect & t_intersectionRect,
            const float t_tolerance,
            bool & t_hasHitSomething);

      private:
        sf::Sprite m_sprite;
        AvatarType m_type;
        AvatarAnim m_anim;
        AvatarState m_state;
        float m_elapsedTimeSec;
        float m_deathDelayElapsedTimeSec;
        std::size_t m_animIndex;
        sf::Vector2f m_velocity;
        bool m_hasLanded;
        bool m_isFacingRight;
        sf::Vector2f m_avatarSizeRatio;
        bool m_isAnimating;
        bool m_hasHitEnemy;
        MovementDetails m_movement;
        RunParticleEffect m_runParticleEffect;
        std::vector<sf::FloatRect> collisionRectCache;
        float m_facingDirectionOffsetRatio;
        bool m_willDisplayCollisionRect;
    };

} // namespace bramblefore

#endif // AVATAR_HPP_INCLUDED
