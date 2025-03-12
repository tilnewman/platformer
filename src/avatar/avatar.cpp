// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// avatar.hpp
//
#include "avatar/avatar.hpp"

#include "bramblefore/settings.hpp"
#include "map/accents.hpp"
#include "map/level-info.hpp"
#include "map/level.hpp"
#include "map/pickups.hpp"
#include "monster/monster-manager.hpp"
#include "player/player-info-display.hpp"
#include "state/state-manager.hpp"
#include "subsystem/background-images.hpp"
#include "subsystem/context.hpp"
#include "subsystem/floating-text.hpp"
#include "subsystem/harm-collision-manager.hpp"
#include "subsystem/map-coordinator.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace bramblefore
{

    Avatar::Avatar()
        : m_sprite{ util::SfmlDefaults::instance().texture() }
        , m_type{ AvatarType::Count } // anything works here
        , m_anim{ AvatarAnim::Walk }
        , m_state{ AvatarState::Still }
        , m_elapsedTimeSec{ 0.0f }
        , m_deathDelayElapsedTimeSec{ 0.0f }
        , m_animIndex{ 0 }
        , m_velocity{}
        , m_hasLanded{ false }
        , m_isFacingRight{ true }
        , m_avatarSizeRatio{ 0.25f, 0.35f }
        , m_isAnimating{ false }
        , m_hasHitEnemy{ false }
        , m_spellAnim{}
        , m_movement{}
        , m_runParticleEffect{}
    {}

    Avatar::~Avatar() { AvatarTextureManager::instance().release(m_type); }

    void Avatar::setup(const Context & t_context)
    {
        m_runParticleEffect.setup(t_context);
        m_spellAnim.setup(t_context);

        m_type  = t_context.player.avatarType();
        m_anim  = AvatarAnim::Walk;
        m_state = AvatarState::Still;

        AvatarTextureManager & textureManager{ AvatarTextureManager::instance() };
        textureManager.acquire(t_context, m_type);
        textureManager.set(m_sprite, m_type, m_anim, 0);

        const float scale{ t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.avatar_scale) };

        m_sprite.setScale({ scale, scale });

        m_movement = calculateMovementDetails(t_context);
    }

    void Avatar::update(Context & t_context, const float t_frameTimeSec)
    {
        m_runParticleEffect.update(t_context, t_frameTimeSec);

        m_spellAnim.update(t_frameTimeSec);

        // this handleDeath() call must happen first
        if (handleDeath(t_context, t_frameTimeSec))
        {
            return;
        }

        // handleAttacking() should be called BEFORE these
        handleAttackState(t_context);
        handleAttackingEnemies(t_context);
        sideToSideMotion(t_context, t_frameTimeSec);
        jumping(t_context, t_frameTimeSec);
        handleClimbing(t_context, t_frameTimeSec);

        if (AvatarState::Climb != m_state)
        {
            m_velocity.y += (t_context.settings.gravity_acc * t_frameTimeSec);
            m_sprite.move(m_velocity);
        }

        // moveMap() and collision handlers should be called AFTER m_sprite.move() above
        moveMap(t_context);
        preventBacktracking(t_context);
        collisions(t_context);
        exitCollisions(t_context);
        hurtCollisions(t_context);
        killCollisions(t_context);

        t_context.pickup.processCollisionWithAvatar(t_context, collisionRect());

        // these two must happen last
        killIfOutOfBounds(t_context);
        animate(t_context, t_frameTimeSec);

        if ((AvatarState::Run != m_state) && (AvatarState::Jump != m_state) &&
            (AvatarState::JumpHigh != m_state))
        {
            m_runParticleEffect.stop();
        }
    }

    void Avatar::draw(sf::RenderTarget & t_target, sf::RenderStates t_states)
    {
        m_runParticleEffect.draw(t_target, t_states);
        t_target.draw(m_sprite, t_states);
        // util::drawRectangleShape(t_target, collisionRect(), false, sf::Color::Red);
        m_spellAnim.draw(t_target, t_states);
    }

    void Avatar::setPosition(const sf::FloatRect & t_rect)
    {
        m_sprite.setPosition({ util::center(t_rect).x, util::bottom(t_rect) });
        m_sprite.move({ 0.0f, (-110.0f * m_sprite.getScale().y) });
    }

    sf::FloatRect Avatar::collisionRect() const
    {
        const sf::FloatRect bounds{ m_sprite.getGlobalBounds() };
        sf::FloatRect rect{ bounds };
        util::scaleRectInPlace(rect, m_avatarSizeRatio);
        rect.position.y += (bounds.size.x * 0.175f);

        if (m_isFacingRight)
        {
            rect.position.x -= (bounds.size.x * 0.15f);
        }
        else
        {
            rect.position.x += (bounds.size.x * 0.15f);
        }

        return rect;
    }

    void Avatar::changeType(Context & t_context)
    {
        AvatarTextureManager::instance().release(m_type);
        m_type = t_context.player.avatarType();
        AvatarTextureManager::instance().acquire(t_context, m_type);
        m_movement = calculateMovementDetails(t_context);
    }

    sf::FloatRect Avatar::attackRect() const
    {
        if ((AvatarState::Attack != m_state) && (AvatarState::AttackExtra != m_state))
        {
            return {};
        }

        sf::FloatRect rect{ collisionRect() };

        // make the attack rect 2 pixels bigger vertically so players can attack up and down
        rect.size.y += 4.0f;
        rect.position.y -= 2.0f;

        if ((AvatarType::BlueKnight == m_type) || (AvatarType::RedKnight == m_type) ||
            (AvatarType::Viking == m_type))
        {
            rect.size.x *= 1.2f;
        }

        if (m_isFacingRight)
        {
            rect.position.x += rect.size.x;
        }
        else
        {
            rect.position.x -= rect.size.x;
        }

        return rect;
    }

    void Avatar::animate(Context &, const float t_frameTimeSec)
    {
        AvatarTextureManager & textureManager{ AvatarTextureManager::instance() };
        const std::size_t frameCount{ textureManager.frameCount(m_type, m_anim) };

        const float timePerFrameSec{ avatarTimePerFrameSec(m_anim) };

        if (AvatarState::Still == m_state)
        {
            textureManager.set(m_sprite, m_type, m_anim, 0);
            return;
        }

        m_elapsedTimeSec += t_frameTimeSec;
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

    void Avatar::handleClimbing(Context & t_context, const float t_frameTimeSec)
    {
        // first frame
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) &&
            (AvatarState::Attack != m_state) && (AvatarState::AttackExtra != m_state) &&
            (AvatarState::Climb != m_state) && (AvatarState::Hurt != m_state) &&
            (AvatarState::Death != m_state))
        {
            const std::optional<sf::FloatRect> ladderRectOpt{ t_context.level.ladderCollisionRect(
                collisionRect()) };

            if (ladderRectOpt.has_value())
            {
                m_state = AvatarState::Climb;
                m_anim  = AvatarAnim::Climb;
                restartAnim();

                m_velocity.x = 0.0f;
                m_velocity.y = 0.0f;

                return;
            }
        }

        // all other frames
        if ((AvatarState::Climb == m_state))
        {
            const std::optional<sf::FloatRect> ladderRectOpt{ t_context.level.ladderCollisionRect(
                collisionRect()) };

            if (ladderRectOpt.has_value())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
                {
                    m_sprite.move({ 0.0f, -(m_movement.ladder_speed * t_frameTimeSec) });
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
                {
                    m_sprite.move({ 0.0f, (m_movement.ladder_speed * t_frameTimeSec) });
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
                {
                    m_sprite.move({ -(m_movement.ladder_speed * t_frameTimeSec), 0.0f });
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
                {
                    m_sprite.move({ (m_movement.ladder_speed * t_frameTimeSec), 0.0f });
                }
            }
        }
    }

    void Avatar::handleAttackState(Context & t_context)
    {
        // first frame
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::F) &&
            (AvatarState::Attack != m_state) && (AvatarState::AttackExtra != m_state) &&
            (AvatarState::Climb != m_state) && (AvatarState::Hurt != m_state) &&
            (AvatarState::Death != m_state))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift))
            {
                m_state = AvatarState::AttackExtra;
                m_anim  = AvatarAnim::AttackExtra;

                if (isWizard(m_type))
                {
                    const sf::Vector2f spellAnimPos = [&]() {
                        const sf::FloatRect collRect{ collisionRect() };
                        sf::Vector2f pos{ util::center(collRect) };

                        if (m_isFacingRight)
                        {
                            pos.x += (collRect.size.x * 1.5f);
                        }
                        else
                        {
                            pos.x -= (collRect.size.x * 1.5f);
                        }

                        return pos;
                    }();

                    t_context.player.castCurrentSpell(t_context, spellAnimPos);
                }
                else
                {
                    t_context.sfx.play("swipe", 0.5f);
                }
            }
            else
            {
                t_context.sfx.play("swipe");
                m_state = AvatarState::Attack;
                m_anim  = AvatarAnim::Attack;
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

    void Avatar::handleAttackingEnemies(Context & t_context)
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

        const AttackInfo attackInfo(
            startingAttackDamage(m_type, (AvatarState::AttackExtra == m_state)), attackRect());

        if (t_context.level.monsters.avatarAttack(t_context, attackInfo))
        {
            m_hasHitEnemy = true;
            if (isWizard(m_type))
            {
                t_context.sfx.play("hit-staff");
            }
            else
            {
                t_context.sfx.play("hit-metal");
            }
        }
    }

    void Avatar::moveMap(Context & t_context)
    {
        const float posXAfter    = util::center(m_sprite.getGlobalBounds()).x;
        const float screenMiddle = (t_context.layout.wholeRect().size.x * 0.5f);

        if ((m_velocity.x < 0.0f) || (posXAfter < screenMiddle))
        {
            return;
        }

        const float moveX = (screenMiddle - posXAfter);
        if (t_context.level.move(t_context, moveX))
        {
            m_sprite.move({ moveX, 0.0f });
            m_runParticleEffect.move(moveX);
        }
    }

    void Avatar::killIfOutOfBounds(Context & t_context)
    {
        if (!t_context.layout.wholeRect().findIntersection(collisionRect()))
        {
            triggerDeath(t_context);
        }
    }

    void Avatar::preventBacktracking(const Context & t_context)
    {
        const sf::FloatRect backtrackRect{ { -100.0f, 0.0f },
                                           { 100.0f, t_context.layout.wholeSize().y } };

        const auto intersectionOpt{ collisionRect().findIntersection(backtrackRect) };
        if (intersectionOpt.has_value())
        {
            m_velocity.x = 0.0f;
            m_sprite.move({ intersectionOpt.value().size.x, 0.0f });
        }
    }

    void Avatar::collisions(Context & t_context)
    {
        const float tolerance = 25.0f; // this magic number brought to you by zTn 2021-8-2

        const sf::FloatRect avatarRect  = collisionRect();
        const sf::Vector2f avatarCenter = util::center(avatarRect);

        const float footRectHeightAdj{ avatarRect.size.y * 0.8f };
        sf::FloatRect footRect = avatarRect;
        footRect.position.y += footRectHeightAdj;
        footRect.size.y -= footRectHeightAdj;

        std::vector<sf::FloatRect> rects{ t_context.level.collisions };
        t_context.level.monsters.appendCollisionRects(rects);

        for (const sf::FloatRect & rect : t_context.level.layer_collisions)
        {
            rects.push_back(rect);
        }

        bool hasHitSomething{ false };
        for (const sf::FloatRect & collRect : rects)
        {
            const auto intersectOpt{ avatarRect.findIntersection(collRect) };
            if (!intersectOpt)
            {
                continue;
            }

            const sf::FloatRect intersection{ intersectOpt.value() };

            if ((m_velocity.y < 0.0f) && (util::center(collRect).y < avatarCenter.y))
            {
                // rising and hit something above

                m_velocity.y = 0.0f;
                m_sprite.move({ 0.0f, intersection.size.y });
                hasHitSomething = true;
                continue;
            }

            if ((m_velocity.y >= 0.0f) && (intersection.size.y < tolerance) &&
                collRect.findIntersection(footRect))
            {
                // falling and hit something below

                if (!m_hasLanded)
                {
                    t_context.sfx.play("land");
                    m_state = AvatarState::Still;
                    m_anim  = AvatarAnim::Walk;
                    restartAnim();
                }

                m_hasLanded  = true;
                m_velocity.y = 0.0f;
                m_sprite.move({ 0.0f, -intersection.size.y });
                hasHitSomething = true;
                continue;
            }

            // at this point we hit something from the side

            if (intersection.size.x < tolerance)
            {
                if (m_velocity.x > 0.0f)
                {
                    m_velocity.x = 0.0f;
                    m_sprite.move({ -intersection.size.x, 0.0f });
                    hasHitSomething = true;
                    continue;
                }
                else if (m_velocity.x < 0.0f)
                {
                    m_velocity.x = 0.0f;
                    m_sprite.move({ intersection.size.x, 0.0f });
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

    MovementDetails Avatar::calculateMovementDetails(const Context & t_context) const
    {
        MovementDetails details;

        details.walk_speed_limit = t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.walk_speed_limit);

        details.walk_acc =
            t_context.layout.calScaleBasedOnResolution(t_context, t_context.settings.walk_acc);

        details.run_speed_limit = t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.run_speed_limit);

        details.run_acc =
            t_context.layout.calScaleBasedOnResolution(t_context, t_context.settings.run_acc);

        details.ladder_speed =
            t_context.layout.calScaleBasedOnResolution(t_context, t_context.settings.ladder_speed);

        details.jump_acc = t_context.settings.jump_acc;

        details.high_jump_acc = t_context.settings.high_jump_acc;

        if (isWizard(m_type))
        {
            const float wizardSpeedRatio{ 1.35f };
            details.walk_speed_limit *= wizardSpeedRatio;
            details.walk_acc *= wizardSpeedRatio;
            details.run_speed_limit *= wizardSpeedRatio;
            details.run_acc *= wizardSpeedRatio;

            details.jump_acc += 50.0f;
            details.high_jump_acc += 50.0f;
        }
        else if (isRaider(m_type))
        {
            const float raiderSpeedRatio{ 2.0f };
            details.walk_speed_limit *= raiderSpeedRatio;
            details.walk_acc *= raiderSpeedRatio;
            details.run_speed_limit *= raiderSpeedRatio;
            details.run_acc *= raiderSpeedRatio;

            details.jump_acc += 100.0f;
            details.high_jump_acc += 100.0f;
        }

        return details;
    }

    void Avatar::sideToSideMotion(Context & t_context, const float t_frameTimeSec)
    {
        if ((AvatarState::Hurt == m_state) || (AvatarState::Attack == m_state) ||
            (AvatarState::AttackExtra == m_state) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
        {
            return;
        }

        if ((AvatarState::Jump == m_state) || (AvatarState::JumpHigh == m_state))
        {
            // Allow moving side-to-side at a reduced rate while in the air.
            // It sounds wrong but feels so right.
            // What the hell, mario did it.
            const float jumpMoveDivisor = 3.0f;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
            {
                m_velocity.x += ((m_movement.walk_acc / jumpMoveDivisor) * t_frameTimeSec);

                if (m_velocity.x > m_movement.walk_speed_limit)
                {
                    m_velocity.x = m_movement.walk_speed_limit;
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
            {
                m_velocity.x -= ((m_movement.walk_acc / jumpMoveDivisor) * t_frameTimeSec);

                if (m_velocity.x < -m_movement.walk_speed_limit)
                {
                    m_velocity.x = -m_movement.walk_speed_limit;
                }
            }

            return;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift))
            {
                m_velocity.x += (m_movement.run_acc * t_frameTimeSec);
                if (m_velocity.x > m_movement.run_speed_limit)
                {
                    m_velocity.x = m_movement.run_speed_limit;
                }

                if (AvatarState::Run != m_state)
                {
                    restartAnim();
                    t_context.sfx.play("walk");
                }

                m_state = AvatarState::Run;
                m_anim  = AvatarAnim::Run;

                m_runParticleEffect.start();
            }
            else
            {
                m_velocity.x += (m_movement.walk_acc * t_frameTimeSec);
                if (m_velocity.x > m_movement.walk_speed_limit)
                {
                    m_velocity.x = m_movement.walk_speed_limit;
                }

                if (AvatarState::Walk != m_state)
                {
                    restartAnim();
                    t_context.sfx.play("walk");
                }

                m_state = AvatarState::Walk;
                m_anim  = AvatarAnim::Walk;
            }

            if (!m_isFacingRight)
            {
                turnRight();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift))
            {
                m_velocity.x -= (m_movement.run_acc * t_frameTimeSec);
                if (m_velocity.x < -m_movement.run_speed_limit)
                {
                    m_velocity.x = -m_movement.run_speed_limit;
                }

                if (AvatarState::Run != m_state)
                {
                    restartAnim();
                    t_context.sfx.play("walk");
                }

                m_state = AvatarState::Run;
                m_anim  = AvatarAnim::Run;

                m_runParticleEffect.start();
            }
            else
            {
                m_velocity.x -= (m_movement.walk_acc * t_frameTimeSec);
                if (m_velocity.x < -m_movement.walk_speed_limit)
                {
                    m_velocity.x = -m_movement.walk_speed_limit;
                }

                if (AvatarState::Walk != m_state)
                {
                    restartAnim();
                    t_context.sfx.play("walk");
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
            if (AvatarState::Idle != m_state)
            {
                m_velocity.x = 0.0f;
                m_state      = AvatarState::Still;
                m_anim       = AvatarAnim::Walk;
                restartAnim();
                t_context.sfx.stop("walk");
            }
        }
    }

    void Avatar::restartAnim()
    {
        m_animIndex      = 0;
        m_isAnimating    = true;
        m_elapsedTimeSec = 0.0f;
    }

    void Avatar::jumping(Context & t_context, const float t_frameTimeSec)
    {
        if ((AvatarState::Attack == m_state) || (AvatarState::AttackExtra == m_state) ||
            (AvatarState::Climb == m_state) || (AvatarState::Death == m_state) ||
            (AvatarState::Hurt == m_state))
        {
            return;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up) && m_hasLanded)
        {
            m_hasLanded = false;
            t_context.sfx.stop("walk");

            if (isMale(m_type))
            {
                t_context.sfx.play("jump-male");
            }
            else
            {
                t_context.sfx.play("jump-female");
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift))
            {
                m_state = AvatarState::JumpHigh;
                m_anim  = AvatarAnim::JumpHigh;
                m_velocity.y -= (m_movement.high_jump_acc * t_frameTimeSec);
            }
            else
            {
                m_state = AvatarState::Jump;
                m_anim  = AvatarAnim::Jump;
                m_velocity.y -= (m_movement.jump_acc * t_frameTimeSec);
            }

            restartAnim();
        }
    }

    void Avatar::triggerDeath(Context & t_context)
    {
        if (AvatarState::Death == m_state)
        {
            return;
        }

        m_state = AvatarState::Death;
        m_anim  = AvatarAnim::Death;
        restartAnim();

        m_velocity = { 0.0f, 0.0f };

        t_context.sfx.stop("walk");
        t_context.map_coord.deathBeforeDelay(t_context);
    }

    void Avatar::triggerIdle()
    {
        if (AvatarState::Still != m_state)
        {
            return;
        }

        m_state = AvatarState::Idle;
        m_anim  = AvatarAnim::Idle;
        restartAnim();
    }

    bool Avatar::handleDeath(Context & t_context, const float t_frameTimeSec)
    {
        if (AvatarState::Death != m_state)
        {
            return false;
        }

        // Delay a few seconds after death before changing states.
        // This allows the player to see how they died, and for all
        // the various sound effects to finish playing.
        m_deathDelayElapsedTimeSec += t_frameTimeSec;
        if (m_deathDelayElapsedTimeSec > t_context.settings.death_delay_sec)
        {
            m_deathDelayElapsedTimeSec = 0.0f;

            if (t_context.level_info.playerLives() > 1)
            {
                t_context.level_info.playerLivesAdjust(-1);
                respawn(t_context);
            }
            else
            {
                t_context.map_coord.deathAfterDelay(t_context);
            }
        }

        animate(t_context, t_frameTimeSec);
        return true;
    }

    void Avatar::respawn(Context & t_context)
    {
        m_state = AvatarState::Still;
        m_anim  = AvatarAnim::Walk;
        restartAnim();
        m_hasLanded = false;
        m_velocity  = { 0.0f, 0.0f };

        if (!m_isFacingRight)
        {
            turnRight();
        }

        t_context.map_coord.respawn(t_context);
    }

    void Avatar::turnRight()
    {
        m_isFacingRight = true;
        m_sprite.scale({ -1.0f, 1.0f });

        m_sprite.move(
            { -(m_sprite.getGlobalBounds().size.x * (1.0f - m_avatarSizeRatio.x)), 0.0f });
    }

    void Avatar::turnLeft()
    {
        m_isFacingRight = false;
        m_sprite.scale({ -1.0f, 1.0f });
        m_sprite.move({ (m_sprite.getGlobalBounds().size.x * (1.0f - m_avatarSizeRatio.x)), 0.0f });
    }

    void Avatar::exitCollisions(Context & t_context) const
    {
        if (collisionRect().findIntersection(t_context.level.exit_rect))
        {
            t_context.sfx.stopAllLooped();
            t_context.state.setChangePending(State::LevelComplete);
        }
    }

    void Avatar::hurtCollisions(Context & t_context)
    {
        if (AvatarState::Hurt == m_state)
        {
            return;
        }

        const sf::FloatRect avatarRect{ collisionRect() };
        harm(t_context, HarmCollisionManager::instance().avatarCollide(t_context, avatarRect));
        harm(t_context, t_context.level.monsters.avatarCollide(avatarRect));
    }

    void Avatar::killCollisions(Context & t_context)
    {
        const sf::FloatRect collRect{ collisionRect() };

        for (const sf::FloatRect & killRect : t_context.level.kill_collisions)
        {
            if (collRect.findIntersection(killRect))
            {
                t_context.player.healthAdjust(t_context, -t_context.player.health());
                triggerDeath(t_context);
                break;
            }
        }
    }

    void Avatar::harm(Context & t_context, const Harm & t_harm)
    {
        if (!t_harm.isAnyHarmDone())
        {
            return;
        }

        if (!t_harm.sfx.empty())
        {
            t_context.sfx.play(t_harm.sfx);
        }

        // some harm has only the sfx so bail here if only an sfx
        if (t_harm.damage <= 0)
        {
            return;
        }

        m_state = AvatarState::Hurt;
        m_anim  = AvatarAnim::Hurt;
        restartAnim();

        const float recoilSpeed{ 3.5f };
        m_velocity.y = -recoilSpeed;

        const float collisionRectCenterHoriz{ util::center(t_harm.rect).x };
        if (collisionRectCenterHoriz < util::center(collisionRect()).x)
        {
            m_velocity.x = recoilSpeed;
        }
        else
        {
            m_velocity.x = -recoilSpeed;
        }

        t_context.player.healthAdjust(t_context, -t_harm.damage);

        if (t_context.player.health() <= 0)
        {
            triggerDeath(t_context);
        }
        else
        {
            t_context.sfx.play("hurt-avatar");
        }
    }

} // namespace bramblefore
