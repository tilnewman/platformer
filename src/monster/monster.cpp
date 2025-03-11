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
#include "subsystem/floating-text.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/random.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    Monster::Monster(const Context & t_context, const MonsterSetupInfo & t_setupInfo)
        : m_type{ t_setupInfo.type }
        , m_spell{ t_setupInfo.spell } // Count means this monster does not cast spells
        , m_region{ t_setupInfo.region }
        , m_anim{ MonsterAnim::Idle }
        , m_animFrame{ 0 }
        , m_sprite{ util::SfmlDefaults::instance().texture() }
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

        // Count means this monster does not cast spells
        if (MonsterSpell::Count != m_spell)
        {
            MonsterSpellTextureManager::instance().acquire(t_context, m_spell);
        }

        initialSpriteSetup(t_context, t_setupInfo.image_height_ratio, t_setupInfo.image_scale);
    }

    Monster::~Monster()
    {
        if (MonsterSpell::Count != m_spell)
        {
            MonsterSpellTextureManager::instance().release(m_spell);
        }

        MonsterTextureManager::instance().release(m_type);
    }

    void Monster::update(Context & t_context, const float t_frameTimeSec)
    {
        m_animations.update(t_frameTimeSec);

        if (!m_isAlive)
        {
            return;
        }

        if (!m_hasSpottedPlayer && m_region.findIntersection(t_context.avatar.collisionRect()))
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
                handleDying(t_context);
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

    void Monster::handleDying(Context & t_context)
    {
        m_isAlive = false;

        const Experience_t xpBonus{ startingHealth(m_type) };

        std::string message{ "+" };
        message += std::to_string(xpBonus);
        message += "xp";

        const sf::Vector2f messagePos{ util::center(m_sprite).x, m_sprite.getPosition().y };

        t_context.float_text.add(t_context, message, sf::Color(200, 200, 200), messagePos);

        t_context.player.experienceAdjust(xpBonus);

        auto & manager{ MonsterTextureManager::instance() };

        manager.setTexture(
            m_sprite,
            m_type,
            MonsterAnim::Death,
            (manager.frameCount(m_type, MonsterAnim::Death) - 1));
    }

    void Monster::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        if (t_context.layout.wholeRect().findIntersection(m_sprite.getGlobalBounds()))
        {
            t_target.draw(m_sprite, t_states);
            m_animations.draw(t_target, t_states);
        }
    }

    void Monster::move(const float t_amount)
    {
        m_sprite.move({ t_amount, 0.0f });
        m_region.position.x += t_amount;
        m_animations.move(t_amount);
    }

    bool Monster::avatarAttack(Context & t_context, const AttackInfo & t_attackInfo)
    {
        if ((MonsterAnim::Death == m_anim) || (MonsterAnim::Hurt == m_anim))
        {
            return false;
        }

        if (!t_attackInfo.rect.findIntersection(collisionRect()))
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

            const std::string deathSfxName{ deathSfx(m_type) };
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

        if ((MonsterAnim::Attack == m_anim) && t_avatarRect.findIntersection(attackCollisionRect()))
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
        if (!t_context.layout.wholeRect().findIntersection(collisionRect()))
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

            if (MonsterSpell::Count != m_spell)
            {
                startAttackAnimation(t_context);
            }
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
            m_sprite.move({ (speed * t_frameTimeSec), 0.0f });
        }
        else
        {
            m_sprite.move({ -(speed * t_frameTimeSec), 0.0f });
        }

        const sf::FloatRect avatarRect{ t_context.avatar.collisionRect() };
        const sf::FloatRect monsterRect{ collisionRect() };

        // backoff if walking into the player
        const auto intersectOpt{ monsterRect.findIntersection(avatarRect) };
        if (intersectOpt)
        {
            if (m_isFacingRight)
            {
                m_sprite.move({ -intersectOpt.value().size.x, 0.0f });
            }
            else
            {
                m_sprite.move({ intersectOpt.value().size.x, 0.0f });
            }
        }

        // if walking out of bounds simply turn around and keep walking
        if (util::right(monsterRect) > util::right(m_region))
        {
            turnAround();
            m_sprite.move({ (util::right(m_region) - util::right(monsterRect)), 0.0f });
        }
        else if (monsterRect.position.x < m_region.position.x)
        {
            turnAround();
            m_sprite.move({ (m_region.position.x - monsterRect.position.x), 0.0f });
        }
    }

    void Monster::resetAnimation()
    {
        m_elapsedTimeSec = 0.0f;
        m_animFrame      = 0;
    }

    void Monster::initialSpriteSetup(
        const Context & t_context, const float t_imageHeightOffsetRatio, const float t_imageScale)
    {
        MonsterTextureManager::instance().setTexture(m_sprite, m_type, m_anim, m_animFrame);

        const float scale{ t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.monster_scale) };

        m_sprite.setScale({ scale, scale });

        m_sprite.scale({ t_imageScale, t_imageScale });
        util::setOriginToCenter(m_sprite);

        m_sprite.setPosition({ t_context.random.fromTo(m_region.position.x, util::right(m_region)),
                               (util::bottom(m_region) - m_sprite.getGlobalBounds().size.y) });

        m_sprite.move({ 0.0f, (t_imageHeightOffsetRatio * m_sprite.getGlobalBounds().size.y) });
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
        m_sprite.scale({ -1.0f, 1.0f });
        m_isFacingRight = !m_isFacingRight;
    }

} // namespace bramblefore