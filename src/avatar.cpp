// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// avatar.hpp
//
#include "avatar.hpp"

#include "accents.hpp"
#include "background-images.hpp"
#include "context.hpp"
#include "harm-collision-manager.hpp"
#include "level-info.hpp"
#include "level.hpp"
#include "monster-manager.hpp"
#include "pickups.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "sound-player.hpp"
#include "spells-anim.hpp"
#include "state-manager.hpp"

#include <SFML/Window/Keyboard.hpp>

namespace platformer
{

    Avatar::Avatar()
        : m_sprite()
        , m_type(AvatarType::Count) // anything works here
        , m_anim(AvatarAnim::Walk)
        , m_state(AvatarState::Still)
        , m_elapsedTimeSec(0.0f)
        , m_deathDelayElapsedTimeSec(0.0f)
        , m_animIndex(0)
        , m_velocity()
        , m_hasLanded(false)
        , m_isFacingRight(true)
        , m_avatarImageWidthRatio(0.25f)
        , m_isAnimating(false)
        , m_hasHitEnemy(false)
        , m_spellAnim()
    {}

    Avatar::~Avatar() { AvatarTextureManager::instance().release(m_type); }

    void Avatar::setup(const Context & context)
    {
        m_spellAnim.setup(context.settings);

        m_type  = context.player.avatarType();
        m_anim  = AvatarAnim::Walk;
        m_state = AvatarState::Still;

        AvatarTextureManager & textureManager{ AvatarTextureManager::instance() };
        textureManager.acquire(context, m_type);
        textureManager.set(m_sprite, m_type, m_anim, 0);
        m_sprite.setScale(context.settings.avatar_scale, context.settings.avatar_scale);
    }

    void Avatar::update(Context & context, const float frameTimeSec)
    {
        m_spellAnim.update(frameTimeSec);

        // this handleDeath() call must happen first
        if (handleDeath(context, frameTimeSec))
        {
            return;
        }

        // handleAttacking() should be called BEFORE these three
        handleAttackState(context);
        handleAttackingEnemies(context);
        sideToSideMotion(context, frameTimeSec);
        jumping(context, frameTimeSec);

        m_velocity += (context.settings.gravity_acc * frameTimeSec);
        m_sprite.move(m_velocity);

        // moveMap() and collisions should be called AFTER m_sprite.move() above
        moveMap(context);
        preventBacktracking(context);
        collisions(context);
        exitCollisions(context);
        hurtCollisions(context);
        // context.managers.collideAllWithAvatar(context, collisionRect());
        context.pickup.processCollisionWithAvatar(context, collisionRect());

        // these two must happen last
        killIfOutOfBounds(context);
        animate(context, frameTimeSec);
    }

    void Avatar::draw(sf::RenderTarget & target, sf::RenderStates states)
    {
        target.draw(m_sprite, states);
        m_spellAnim.draw(target, states);
    }

    void Avatar::setPosition(const sf::FloatRect & rect)
    {
        m_sprite.setPosition(util::center(rect).x, util::bottom(rect));
        m_sprite.move(0.0f, (-110.0f * m_sprite.getScale().y));
    }

    const sf::FloatRect Avatar::collisionRect() const
    {
        const sf::FloatRect bounds{ m_sprite.getGlobalBounds() };
        sf::FloatRect rect{ bounds };
        util::scaleRectInPlace(rect, { m_avatarImageWidthRatio, 0.35f });
        rect.top += (bounds.width * 0.175f);

        if (m_isFacingRight)
        {
            rect.left -= (bounds.width * 0.15f);
        }
        else
        {
            rect.left += (bounds.width * 0.15f);
        }

        return rect;
    }

    void Avatar::changeType(Context & context)
    {
        AvatarTextureManager::instance().release(m_type);
        m_type = context.player.avatarType();
        AvatarTextureManager::instance().acquire(context, m_type);
    }

    const sf::FloatRect Avatar::attackRect() const
    {
        if ((AvatarState::Attack != m_state) && (AvatarState::AttackExtra != m_state))
        {
            return {};
        }

        sf::FloatRect rect{ collisionRect() };

        // make the attack rect slightly bigger vertically so players can attack up and down
        rect.height += 4.0f;
        rect.top -= 2.0f;

        if ((AvatarType::BlueKnight == m_type) || (AvatarType::RedKnight == m_type) ||
            (AvatarType::Viking == m_type))
        {
            rect.width *= 1.2f;
        }

        if (m_isFacingRight)
        {
            rect.left += rect.width;
        }
        else
        {
            rect.left -= rect.width;
        }

        return rect;
    }

    void Avatar::animate(Context &, const float frameTimeSec)
    {
        AvatarTextureManager & textureManager{ AvatarTextureManager::instance() };
        const std::size_t frameCount{ textureManager.frameCount(m_type, m_anim) };

        const float timePerFrameSec{ avatarTimePerFrameSec(m_anim) };

        if (AvatarState::Still == m_state)
        {
            textureManager.set(m_sprite, m_type, m_anim, 0);
            return;
        }

        m_elapsedTimeSec += frameTimeSec;
        if (m_elapsedTimeSec > timePerFrameSec)
        {
            m_elapsedTimeSec -= timePerFrameSec;

            ++m_animIndex;
            m_isAnimating = true;
            if (m_animIndex >= frameCount)
            {
                if (doesAnimLoop(m_anim))
                {
                    m_animIndex = 0;
                }
                else
                {
                    m_isAnimating = false;
                    m_animIndex   = (frameCount - 1);
                }
            }

            textureManager.set(m_sprite, m_type, m_anim, m_animIndex);
        }
    }

    void Avatar::handleAttackState(Context & context)
    {
        // first frame
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && (AvatarState::Attack != m_state) &&
            (AvatarState::AttackExtra != m_state) && (AvatarState::Climb != m_state) &&
            (AvatarState::Hurt != m_state) && (AvatarState::Death != m_state))
        {

            const sf::Vector2f spellAnimPos = [&]() {
                const sf::FloatRect collRect{ collisionRect() };
                sf::Vector2f pos{ util::center(collRect) };

                if (m_isFacingRight)
                {
                    pos.x += (collRect.width * 1.5f);
                }
                else
                {
                    pos.x -= (collRect.width * 1.5f);
                }

                return pos;
            }();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                context.sfx.play("swipe", 0.6f);
                m_state = AvatarState::AttackExtra;
                m_anim  = AvatarAnim::AttackExtra;
                m_spellAnim.add(spellAnimPos, m_type, false, m_isFacingRight);
            }
            else
            {
                context.sfx.play("swipe");
                m_state = AvatarState::Attack;
                m_anim  = AvatarAnim::Attack;
                m_spellAnim.add(spellAnimPos, m_type, true, m_isFacingRight);
            }

            restartAnim();
            return;
        }

        // all other frames
        if ((AvatarState::Attack == m_state) || (AvatarState::AttackExtra == m_state))
        {
            if (!m_isAnimating)
            {
                m_state = AvatarState::Still;
                m_anim  = AvatarAnim::Walk;
                restartAnim();
            }
        }
    }

    void Avatar::handleAttackingEnemies(Context & context)
    {
        if ((AvatarState::Attack != m_state) && (AvatarState::AttackExtra != m_state))
        {
            m_hasHitEnemy = false;
            return;
        }

        if (m_hasHitEnemy)
        {
            return;
        }

        AttackInfo attackInfo;
        attackInfo.rect = attackRect();

        //TODO this needs to be done differently
        if ((AvatarType::Assassin == m_type) || (AvatarType::Ninja == m_type) ||
            (AvatarType::Assassin == m_type))
        {
            if (AvatarState::Attack == m_state)
            {
                attackInfo.damage = 8;
            }
            else
            {
                attackInfo.damage = 12;
            }
        }
        else if (
            (AvatarType::Druid == m_type) || (AvatarType::Enchantress == m_type) ||
            (AvatarType::Witch == m_type))
        {
            if (AvatarState::Attack == m_state)
            {
                attackInfo.damage = 6;
            }
            else
            {
                attackInfo.damage = 12;
            }
        }
        else // BlueKnight, RedKnight, Viking
        {
            if (AvatarState::Attack == m_state)
            {
                attackInfo.damage = 10;
            }
            else
            {
                attackInfo.damage = 16;
            }
        }

        if (context.level.monsters.avatarAttack(context, attackInfo))
        {
            m_hasHitEnemy = true;
            if ((AvatarType::Druid == m_type) || (AvatarType::Enchantress == m_type) ||
                (AvatarType::Witch == m_type))
            {
                context.sfx.play("hit-staff");
            }
            else
            {
                context.sfx.play("hit-metal");
            }
        }
        else
        {
            m_hasHitEnemy = false;
        }
    }

    void Avatar::moveMap(Context & context)
    {
        const float posXAfter    = util::center(m_sprite.getGlobalBounds()).x;
        const float screenMiddle = (context.layout.wholeRect().width * 0.5f);

        if ((m_velocity.x < 0.0f) || (posXAfter < screenMiddle))
        {
            return;
        }

        const float moveX = (screenMiddle - posXAfter);
        if (!context.level.move(context, moveX))
        {
            return;
        }

        const sf::Vector2f move{ moveX, 0.0f };
        m_sprite.move(move);

        context.accent.move(moveX);
        context.pickup.move(moveX);
        context.level.move(context, moveX);
        context.pickup.move(moveX);
        context.bg_image.move(moveX);
        context.spell.move(moveX);
    }

    void Avatar::killIfOutOfBounds(Context & context)
    {
        if (!context.layout.wholeRect().intersects(collisionRect()))
        {
            triggerDeath(context);
        }
    }

    void Avatar::preventBacktracking(const Context & context)
    {
        const sf::FloatRect backtrackRect{ -100.0f, 0.0f, 100.0f, context.layout.wholeSize().y };

        sf::FloatRect intersection;
        if (collisionRect().intersects(backtrackRect, intersection))
        {
            m_velocity.x = 0.0f;
            m_sprite.move(intersection.width, 0.0f);
        }
    }

    void Avatar::collisions(Context & context)
    {
        const float tolerance = 25.0f; // this magic number brought to you by zTn 2021-8-2

        const sf::FloatRect avatarRect  = collisionRect();
        const sf::Vector2f avatarCenter = util::center(avatarRect);

        const float footRectHeightAdj{ avatarRect.height * 0.8f };
        sf::FloatRect footRect = avatarRect;
        footRect.top += footRectHeightAdj;
        footRect.height -= footRectHeightAdj;

        std::vector<sf::FloatRect> rects{ context.level.collisions };
        context.level.monsters.appendCollisionRects(rects);

        bool hasHitSomething{ false };
        sf::FloatRect intersection;
        for (const sf::FloatRect & collRect : rects)
        {
            if (!avatarRect.intersects(collRect, intersection))
            {
                continue;
            }

            if ((m_velocity.y < 0.0f) && (util::center(collRect).y < avatarCenter.y))
            {
                // rising and hit something above

                m_velocity.y = 0.0f;
                m_sprite.move(0.0f, intersection.height);
                hasHitSomething = true;
                continue;
            }

            if ((m_velocity.y >= 0.0f) && (intersection.height < tolerance) &&
                collRect.intersects(footRect))
            {
                // falling and hit something below

                if (!m_hasLanded)
                {
                    context.sfx.play("land");
                    m_state = AvatarState::Still;
                    m_anim  = AvatarAnim::Walk;
                    restartAnim();
                }

                m_hasLanded  = true;
                m_velocity.y = 0.0f;
                m_sprite.move(0.0f, -intersection.height);
                hasHitSomething = true;
                continue;
            }

            // at this point we hit something from the side

            if (intersection.width < tolerance)
            {
                if (m_velocity.x > 0.0f)
                {
                    m_velocity.x = 0.0f;
                    m_sprite.move(-intersection.width, 0.0f);
                    hasHitSomething = true;
                    continue;
                }
                else if (m_velocity.x < 0.0f)
                {
                    m_velocity.x = 0.0f;
                    m_sprite.move(intersection.width, 0.0f);
                    hasHitSomething = true;
                    continue;
                }
            }
        }

        if (!hasHitSomething)
        {
            m_hasLanded = false;
        }
    }

    void Avatar::sideToSideMotion(Context & context, const float frameTimeSec)
    {
        if ((AvatarState::Hurt == m_state) || (AvatarState::Attack == m_state) ||
            (AvatarState::AttackExtra == m_state))
        {
            return;
        }

        if ((AvatarState::Jump == m_state) || (AvatarState::JumpHigh == m_state))
        {
            // Allow moving side-to-side at a reduced rate while in the air.
            // It sounds wrong but feels so right.
            // What the hell, mario did it.
            const float jumpMoveDivisor = 3.0f;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                m_velocity.x +=
                    ((context.settings.walk_acceleration / jumpMoveDivisor) * frameTimeSec);

                if (m_velocity.x > context.settings.walk_speed_limit)
                {
                    m_velocity.x = context.settings.walk_speed_limit;
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                m_velocity.x -=
                    ((context.settings.walk_acceleration / jumpMoveDivisor) * frameTimeSec);

                if (m_velocity.x < -context.settings.walk_speed_limit)
                {
                    m_velocity.x = -context.settings.walk_speed_limit;
                }
            }

            return;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                m_velocity.x += (context.settings.run_acceleration * frameTimeSec);
                if (m_velocity.x > context.settings.run_speed_limit)
                {
                    m_velocity.x = context.settings.run_speed_limit;
                }

                if (AvatarState::Run != m_state)
                {
                    restartAnim();
                    context.sfx.play("walk");
                }

                m_state = AvatarState::Run;
                m_anim  = AvatarAnim::Run;
            }
            else
            {
                m_velocity.x += (context.settings.walk_acceleration * frameTimeSec);
                if (m_velocity.x > context.settings.walk_speed_limit)
                {
                    m_velocity.x = context.settings.walk_speed_limit;
                }

                if (AvatarState::Walk != m_state)
                {
                    restartAnim();
                    context.sfx.play("walk");
                }

                m_state = AvatarState::Walk;
                m_anim  = AvatarAnim::Walk;
            }

            if (!m_isFacingRight)
            {
                turnRight();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                m_velocity.x -= (context.settings.run_acceleration * frameTimeSec);
                if (m_velocity.x < -context.settings.run_speed_limit)
                {
                    m_velocity.x = -context.settings.run_speed_limit;
                }

                if (AvatarState::Run != m_state)
                {
                    restartAnim();
                    context.sfx.play("walk");
                }

                m_state = AvatarState::Run;
                m_anim  = AvatarAnim::Run;
            }
            else
            {
                m_velocity.x -= (context.settings.walk_acceleration * frameTimeSec);
                if (m_velocity.x < -context.settings.walk_speed_limit)
                {
                    m_velocity.x = -context.settings.walk_speed_limit;
                }

                if (AvatarState::Walk != m_state)
                {
                    restartAnim();
                    context.sfx.play("walk");
                }

                m_state = AvatarState::Walk;
                m_anim  = AvatarAnim::Walk;
            }

            if (m_isFacingRight)
            {
                turnLeft();
            }
        }
        else
        {
            m_velocity.x = 0.0f;
            m_state      = AvatarState::Still;
            m_anim       = AvatarAnim::Walk;
            restartAnim();
            context.sfx.stop("walk");
        }
    }

    void Avatar::restartAnim()
    {
        m_animIndex      = 0;
        m_isAnimating    = true;
        m_elapsedTimeSec = 0.0f;
    }

    void Avatar::jumping(Context & context, const float frameTimeSec)
    {
        if ((AvatarState::Attack == m_state) || (AvatarState::AttackExtra == m_state) ||
            (AvatarState::Climb == m_state) || (AvatarState::Death == m_state) ||
            (AvatarState::Hurt == m_state))
        {
            return;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_hasLanded)
        {
            m_hasLanded = false;
            // context.sfx.play("jump"); //TODO
            context.sfx.stop("walk");

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                m_state = AvatarState::JumpHigh;
                m_anim  = AvatarAnim::JumpHigh;
                m_velocity.y -= (context.settings.high_jump_acc * frameTimeSec);
            }
            else
            {
                m_state = AvatarState::Jump;
                m_anim  = AvatarAnim::Jump;
                m_velocity.y -= (context.settings.jump_acc * frameTimeSec);
            }

            restartAnim();
        }
    }

    void Avatar::triggerDeath(Context & context)
    {
        if (AvatarState::Death == m_state)
        {
            return;
        }

        m_state = AvatarState::Death;
        m_anim  = AvatarAnim::Death;
        restartAnim();
        context.sfx.stop("walk");
        context.sfx.play("death-avatar");
        m_velocity = { 0.0f, 0.0f };

        // context.stats.has_player_died = true;
        // context.stats.enemy_killed    = 0;
        // context.stats.coin_collected  = 0;
    }

    bool Avatar::handleDeath(Context & context, const float frameTimeSec)
    {
        if (AvatarState::Death != m_state)
        {
            return false;
        }

        // Delay a few seconds after death before changing states.
        // This allows the player to see how they died, and for all
        // the various sound effects to finish playing.
        m_deathDelayElapsedTimeSec += frameTimeSec;
        if (m_deathDelayElapsedTimeSec > context.settings.death_delay_sec)
        {
            m_deathDelayElapsedTimeSec = 0.0f;

            if (context.level_info.playerLives() > 1)
            {
                context.level_info.playerLivesAdjust(-1);
                respawn(context);
            }
            else
            {
                context.state.setChangePending(State::LevelDeath);
            }
        }

        animate(context, frameTimeSec);
        return true;
    }

    void Avatar::respawn(Context & context)
    {
        context.accent.clear();
        context.pickup.clear();
        context.spell.clear();

        context.level.load(context);

        m_state = AvatarState::Still;
        m_anim  = AvatarAnim::Walk;
        restartAnim();
        m_hasLanded = false;
        m_velocity  = { 0.0f, 0.0f };

        if (!m_isFacingRight)
        {
            turnRight();
        }

        context.sfx.play("spawn");
    }

    void Avatar::turnRight()
    {
        m_isFacingRight = true;
        m_sprite.scale(-1.0f, 1.0f);
        m_sprite.move(-(m_sprite.getGlobalBounds().width * (1.0f - m_avatarImageWidthRatio)), 0.0f);
    }

    void Avatar::turnLeft()
    {
        m_isFacingRight = false;
        m_sprite.scale(-1.0f, 1.0f);
        m_sprite.move((m_sprite.getGlobalBounds().width * (1.0f - m_avatarImageWidthRatio)), 0.0f);
    }

    void Avatar::exitCollisions(Context & context) const
    {
        if (collisionRect().intersects(context.level.exit_rect))
        {
            context.sfx.stopAllLooped();
            context.state.setChangePending(State::LevelComplete);
        }
    }

    void Avatar::hurtCollisions(Context & context)
    {
        if (AvatarState::Hurt == m_state)
        {
            return;
        }

        const sf::FloatRect avatarRect{ collisionRect() };
        harm(context, HarmCollisionManager::instance().avatarCollide(context, avatarRect));
        harm(context, context.level.monsters.avatarCollide(avatarRect));
    }

    void Avatar::harm(Context & context, const Harm & harm)
    {
        if (!harm.isAnyHarmDone())
        {
            return;
        }

        if (!harm.sfx.empty())
        {
            context.sfx.play(harm.sfx);
        }

        if (harm.damage <= 0)
        {
            return;
        }

        m_state = AvatarState::Hurt;
        m_anim  = AvatarAnim::Hurt;
        restartAnim();

        const float recoilSpeed{ 3.5f };
        m_velocity.y = -recoilSpeed;

        const float collisionRectCenterHoriz{ util::center(harm.rect).x };
        if (collisionRectCenterHoriz < util::center(collisionRect()).x)
        {
            m_velocity.x = recoilSpeed;
        }
        else
        {
            m_velocity.x = -recoilSpeed;
        }

        // TODO subtract harm.damage from player health and check for death
        context.sfx.play("hurt-avatar");
    }

} // namespace platformer
