// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster.hpp
//
#include "monster/monster.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "monster/monster-textures.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    Monster::Monster(Context & t_context, const MonsterSetupInfo & t_setupInfo)
        : m_type{ t_setupInfo.type }
        , m_region{ t_setupInfo.region }
        , m_anim{ MonsterAnim::Idle }
        , m_animFrame{ 0 }
        , m_sprite{}
        , m_elapsedTimeSec{ 0.0f }
        , m_isFacingRight{ true }
        , m_stateElapsedTimeSec{ 0.0f }
        , m_stateTimeUntilChangeSec{ 0.0f }
        , m_hasSpottedPlayer{ false }
        , m_health{ startingHealth(t_setupInfo.type) }
        , m_isAlive{ true }
        , m_animations{}
    {
        MonsterTextureManager::instance().acquire(t_context, m_type);
        m_animations.setup(t_context.settings);
        initialSpriteSetup(t_context, t_setupInfo.image_height_ratio, t_setupInfo.image_scale);
    }

    Monster::~Monster() { MonsterTextureManager::instance().release(m_type); }

    void Monster::update(Context & t_context, const float t_frameTimeSec)
    {
        m_animations.update(t_frameTimeSec);

        if (!m_isAlive)
        {
            return;
        }

        if (!m_hasSpottedPlayer && m_region.intersects(t_context.avatar.collisionRect()))
        {
            m_hasSpottedPlayer = true;
            turnToFacePlayer(t_context);
        }

        m_elapsedTimeSec += t_frameTimeSec;
        m_stateElapsedTimeSec += t_frameTimeSec;

        handleWalking(t_context, t_frameTimeSec);

        if (!animate())
        {
            return;
        }

        if (doesAnimLoop(m_anim))
        {
            if (m_stateElapsedTimeSec > m_stateTimeUntilChangeSec)
            {
                m_stateElapsedTimeSec = 0.0f;

                if (m_hasSpottedPlayer)
                {
                    changeStateAfterSeeingPlayer(t_context);
                }
                else
                {
                    changeStateBeforeSeeingPlayer(t_context);
                }
            }
        }
        else
        {
            if (MonsterAnim::Death == m_anim)
            {
                m_isAlive = false;

                auto & manager{ MonsterTextureManager::instance() };

                manager.setTexture(
                    m_sprite,
                    m_type,
                    MonsterAnim::Death,
                    (manager.frameCount(m_type, MonsterAnim::Death) - 1));
            }
            else
            {
                if (m_hasSpottedPlayer)
                {
                    changeStateAfterSeeingPlayer(t_context);
                }
                else
                {
                    changeStateBeforeSeeingPlayer(t_context);
                }
            }
        }
    }

    void Monster::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        if (t_context.layout.wholeRect().intersects(m_sprite.getGlobalBounds()))
        {
            t_target.draw(m_sprite, t_states);
            // util::drawRectangleShape(target, collisionRect(), false, sf::Color::Green);
            // util::drawRectangleShape(target, attackCollisionRect(), false, sf::Color::Red);

            m_animations.draw(t_target, t_states);
        }
    }

    void Monster::move(const float t_amount)
    {
        m_sprite.move(t_amount, 0.0f);
        m_region.left += t_amount;
        m_animations.move(t_amount);
    }

    bool Monster::avatarAttack(Context & t_context, const AttackInfo & t_attackInfo)
    {
        if ((MonsterAnim::Death == m_anim) || (MonsterAnim::Hurt == m_anim))
        {
            return false;
        }

        if (!t_attackInfo.rect.intersects(collisionRect()))
        {
            return false;
        }

        m_health -= t_attackInfo.damage;

        if (m_health > 0)
        {
            m_anim = MonsterAnim::Hurt;
            resetAnimation();

            const std::string hurtSfxName{ hurtSfx(m_type) };
            if (!hurtSfxName.empty())
            {
                t_context.sfx.play(hurtSfxName);
            }
        }
        else
        {
            m_anim = MonsterAnim::Death;
            resetAnimation();
            
            const std::string deathSfxName{ hurtSfx(m_type) };
            if (!deathSfxName.empty())
            {
                t_context.sfx.play(deathSfxName);
            }
        }

        return true;
    }

    Harm Monster::avatarCollide(const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        if ((MonsterAnim::Attack == m_anim) && t_avatarRect.intersects(attackCollisionRect()))
        {
            harm.damage = attackDamage(m_type);
            harm.rect   = collisionRect();
            harm.sfx    = hitSfx(m_type);
        }

        return harm;
    }

    bool Monster::animate()
    {
        bool isAnimationFinished{ false };

        const float timeBetweenFramesSec{ timePerFrameSec(m_anim) };

        if (m_elapsedTimeSec > timeBetweenFramesSec)
        {
            auto & manager{ MonsterTextureManager::instance() };

            m_elapsedTimeSec -= timeBetweenFramesSec;

            ++m_animFrame;
            if (m_animFrame >= manager.frameCount(m_type, m_anim))
            {
                m_animFrame         = 0;
                isAnimationFinished = true;
            }

            manager.setTexture(m_sprite, m_type, m_anim, m_animFrame);
        }

        return isAnimationFinished;
    }

    float Monster::timePerFrameSec(const MonsterAnim t_anim) const noexcept
    {
        // clang-format off
        switch (t_anim)
        {
            case MonsterAnim::Attack: { return 0.1f;   }
            case MonsterAnim::Death:  { return 0.175f; }
            case MonsterAnim::Hurt:   { return 0.15f;  }
            case MonsterAnim::Idle:   { return 0.15f;  }
            case MonsterAnim::Walk:   { return 0.1f;   }
            case MonsterAnim::Count:  [[fallthrough]];
            default:                  { return 0.0f;   }
        }
        // clang-format on
    }

    void Monster::changeStateBeforeSeeingPlayer(Context & t_context)
    {
        if (MonsterAnim::Death == m_anim)
        {
            return;
        }

        m_elapsedTimeSec          = 0.0f;
        m_stateTimeUntilChangeSec = t_context.random.fromTo(1.0f, 6.0f);

        const int isNextActionWalking{ t_context.random.boolean() };
        if (isNextActionWalking)
        {
            m_anim = MonsterAnim::Walk;
            resetAnimation();
            const bool willChangeDirection{ t_context.random.boolean() };
            if (willChangeDirection)
            {
                turnAround();
            }
        }
        else
        {
            // in all other cases just turn around
            m_anim = MonsterAnim::Idle;
            resetAnimation();
            turnAround();
        }
    }

    void Monster::changeStateAfterSeeingPlayer(Context & t_context)
    {
        if (!t_context.layout.wholeRect().intersects(collisionRect()))
        {
            return;
        }

        if (MonsterAnim::Death == m_anim)
        {
            return;
        }

        turnToFacePlayer(t_context);

        m_elapsedTimeSec      = 0.0f;
        m_stateElapsedTimeSec = 0.0f;

        const int isNextActionWalking{ t_context.random.boolean() };
        if (isNextActionWalking)
        {
            m_anim = MonsterAnim::Walk;
            resetAnimation();
            m_stateTimeUntilChangeSec = t_context.random.fromTo(1.0f, 2.0f);
        }
        else
        {
            // in all other cases just attack
            m_anim = MonsterAnim::Attack;
            resetAnimation();

            const std::string attackSfxName{ hurtSfx(m_type) };
            if (!attackSfxName.empty())
            {
                t_context.sfx.play(attackSfxName);
            }

            startAttackAnimation(t_context);
        }
    }

    void Monster::handleWalking(Context & t_context, const float t_frameTimeSec)
    {
        if (MonsterAnim::Walk != m_anim)
        {
            return;
        }

        const float speed{ walkSpeed(m_type) };
        if (m_isFacingRight)
        {
            m_sprite.move((speed * t_frameTimeSec), 0.0f);
        }
        else
        {
            m_sprite.move(-(speed * t_frameTimeSec), 0.0f);
        }

        const sf::FloatRect avatarRect{ t_context.avatar.collisionRect() };
        const sf::FloatRect monsterRect{ collisionRect() };

        // backoff if walking into the player
        sf::FloatRect intersection;
        if (monsterRect.intersects(avatarRect, intersection))
        {
            if (m_isFacingRight)
            {
                m_sprite.move(-intersection.width, 0.0f);
            }
            else
            {
                m_sprite.move(intersection.width, 0.0f);
            }
        }

        // if walking out of bounds simply turn around and keep walking
        if (util::right(monsterRect) > util::right(m_region))
        {
            turnAround();
            m_sprite.move((util::right(m_region) - util::right(monsterRect)), 0.0f);
        }
        else if (monsterRect.left < m_region.left)
        {
            turnAround();
            m_sprite.move((m_region.left - monsterRect.left), 0.0f);
        }
    }

    void Monster::resetAnimation()
    {
        m_elapsedTimeSec = 0.0f;
        m_animFrame      = 0;
    }

    void Monster::initialSpriteSetup(
        Context & t_context, const float t_imageHeightOffsetRatio, const float t_imageScale)
    {
        MonsterTextureManager::instance().setTexture(m_sprite, m_type, m_anim, m_animFrame);
        m_sprite.setScale(t_context.settings.monster_scale, t_context.settings.monster_scale);
        m_sprite.scale(t_imageScale, t_imageScale);
        util::setOriginToCenter(m_sprite);

        m_sprite.setPosition(
            t_context.random.fromTo(m_region.left, util::right(m_region)),
            (util::bottom(m_region) - m_sprite.getGlobalBounds().height));

        m_sprite.move(0.0f, (t_imageHeightOffsetRatio * m_sprite.getGlobalBounds().height));
    }

    void Monster::turnToFacePlayer(Context & t_context)
    {
        const bool isPlayerToTheRight{ util::center(t_context.avatar.collisionRect()).x >
                                       util::center(collisionRect()).x };

        if (isPlayerToTheRight != m_isFacingRight)
        {
            turnAround();
        }
    }

    void Monster::turnAround()
    {
        m_sprite.scale(-1.0f, 1.0f);
        m_isFacingRight = !m_isFacingRight;
    }

} // namespace platformer