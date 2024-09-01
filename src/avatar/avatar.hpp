#ifndef AVATAR_HPP_INCLUDED
#define AVATAR_HPP_INCLUDED
//
// avatar.hpp
//
#include "avatar/avatar-spell-anims.hpp"
#include "avatar/avatar-textures.hpp"
#include "player/player-info.hpp"
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

    inline constexpr std::string_view toString(const AvatarState t_state)
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
        float walk_acceleration{0.0f};
        float run_acceleration{ 0.0f };
        float ladder_speed{ 0.0f };
        float jump_acceleration{ 0.0f };
        float high_jump_acceleration{0.0f};
    };

    //

    class Avatar
    {
      public:
        Avatar();
        ~Avatar();

        void setup(const Context & t_context);
        void update(Context & t_context, const float t_frameTimeSec);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states);
        void setPosition(const sf::FloatRect & t_rect);
        [[nodiscard]] sf::FloatRect collisionRect() const;
        [[nodiscard]] sf::FloatRect attackRect() const;
        void triggerDeath(Context & t_context);

        // TODO remove after testing
        void changeType(Context & t_context);

      private:
        void animate(Context & t_context, const float t_frameTimeSec);
        void handleClimbing(Context & t_context, const float t_frameTimeSec);
        void handleAttackState(Context & t_context);
        void handleAttackingEnemies(Context & t_context);
        void moveMap(Context & t_context);
        void killIfOutOfBounds(Context & t_context);
        void preventBacktracking(const Context & t_context);
        void collisions(Context & t_context);
        void sideToSideMotion(Context & t_context, const float t_frameTimeSec);
        void restartAnim();
        void jumping(Context & t_context, const float t_frameTimeSec);
        bool handleDeath(Context & t_context, const float t_frameTimeSec);
        void respawn(Context & t_context);
        void turnRight();
        void turnLeft();
        void exitCollisions(Context & t_context) const;
        void hurtCollisions(Context & t_context);
        void harm(Context & t_context, const Harm & t_harm);
        MovementDetails calculateMovementDetails(const Context & t_context) const;

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
        float m_avatarImageWidthRatio;
        bool m_isAnimating;
        bool m_hasHitEnemy;
        AvatarSpellAnimations m_spellAnim;
        MovementDetails m_movement;
    };

} // namespace bramblefore

#endif // AVATAR_HPP_INCLUDED
