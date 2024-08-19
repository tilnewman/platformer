#ifndef AVATAR_HPP_INCLUDED
#define AVATAR_HPP_INCLUDED
//
// avatar.hpp
//
#include "harm.hpp"
#include "avatar-textures.hpp"

#include <string_view>
#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace platformer
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

    inline constexpr std::string_view toString(const AvatarState state)
    {
        // clang-format off
        switch (state)
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

    class Avatar
    {
      public:
        Avatar();

        void setup(const Context & context, const AvatarType & type);
        void update(Context & context, const float frameTimeSec);
        void draw(sf::RenderTarget & target, sf::RenderStates states);
        void setPosition(const sf::FloatRect & rect);
        AvatarType type() const { return m_type; }
        const sf::FloatRect collisionRect() const;
        const sf::FloatRect attackRect() const;
        void triggerDeath(Context & context);

        // TODO remove after testing
        void changeType();
        AvatarAnim anim() const { return m_anim; }
        std::size_t animIndex() const { return m_animIndex; }

      private:
        void animate(Context & context, const float frameTimeSec);
        void handleAttackState(Context & context);
        void handleAttackingEnemies(Context & context);
        void moveMap(Context & context);
        void killIfOutOfBounds(Context & context);
        void preventBacktracking(const Context & context);
        void collisions(Context & context);
        void sideToSideMotion(Context & context, const float frameTimeSec);
        void restartAnim();
        void jumping(Context & context, const float frameTimeSec);
        bool handleDeath(Context & context, const float frameTimeSec);
        void respawn(Context & context);
        void turnRight();
        void turnLeft();
        void exitCollisions(Context & context) const;
        void hurtCollisions(Context & context);
        void harm(Context & context, const Harm & harm);

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
    };

} // namespace platformer

#endif // AVATAR_HPP_INCLUDED
