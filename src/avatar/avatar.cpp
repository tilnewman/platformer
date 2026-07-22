// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// avatar.hpp
//
#include "avatar/avatar.hpp"

#include "bramblefore/settings.hpp"
#include "map/level-info.hpp"
#include "map/level.hpp"
#include "map/pickups.hpp"
#include "monster/monster-manager.hpp"
#include "state/state-manager.hpp"
#include "subsystem/context.hpp"
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
        , m_avatarSizeRatio{}
        , m_isAnimating{ false }
        , m_hasHitEnemy{ false }
        , m_movement{}
        , m_runParticleEffect{}
        , collisionRectCache{}
        , m_facingDirectionOffsetRatio{ 0.0f }
        , m_willDisplayCollisionRect{ false }
    {
        // just a guess
        collisionRectCache.reserve(1024);
    }

    Avatar::~Avatar() { AvatarTextureManager::instance().release(m_type); }

    void Avatar::setup(const Context & t_context)
    {
        m_runParticleEffect.setup(t_context);

        m_avatarSizeRatio            = t_context.settings.avatar_collision_scale;
        m_facingDirectionOffsetRatio = t_context.settings.avatar_collision_facing_offset_ratio;
        m_willDisplayCollisionRect   = t_context.settings.avatar_collision_display;
        m_type                       = t_context.player.avatarType();
        m_anim                       = AvatarAnim::Walk;
        m_state                      = AvatarState::Still;

        AvatarTextureManager & textureManager{ AvatarTextureManager::instance() };
        textureManager.acquire(t_context, m_type);
        textureManager.set(m_sprite, m_type, m_anim, 0);

        const float scale{ t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.avatar_scale) };

        m_sprite.setScale({ scale, scale });

        calculateMovementDetails(t_context);
    }

    void Avatar::update(const Context & t_context, const float t_frameTimeSec)
    {
        m_runParticleEffect.update(t_context, t_frameTimeSec);

        // this handleDeath() call must happen before all other handle functions below
        if (handleDeath(t_context, t_frameTimeSec))
        {
            return;
        }

        handleAttackState(t_context);
        handleAttackingEnemies(t_context);

        sideToSideMotion(t_context, t_frameTimeSec);
        jumpingMotion(t_context, t_frameTimeSec);
        climbingMotion(t_context, t_frameTimeSec);
        gravityMotion(t_frameTimeSec);

        // prevent backtracking only after all the motion funcions above
        preventBacktracking(t_context);

        // moveMap() should be called after the movement functions above
        moveMap(t_context);

        // all collision handling should occur after the moveMap() function above
        collisions(t_context);
        exitCollisions(t_context);
        hurtCollisions(t_context);
        killCollisions(t_context);

        t_context.pickup.processCollisionWithAvatar(t_context, collisionRect());

        // these two should be called in this order after all other functions above
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

        if (m_willDisplayCollisionRect)
        {
            util::drawRectangleShape(t_target, collisionRect(), false, sf::Color::Red);
        }
    }

    void Avatar::resetBeforePlacingInNewLevel()
    {
        m_anim                     = AvatarAnim::Walk;
        m_state                    = AvatarState::Still;
        m_elapsedTimeSec           = 0.0f;
        m_deathDelayElapsedTimeSec = 0.0f;
        m_velocity                 = { 0.0f, 0.0f };
        m_hasLanded                = false;
        m_isFacingRight            = true;
        m_hasHitEnemy              = false;
        m_runParticleEffect.stop();
    }

    void Avatar::setToSpawnPosition(const sf::FloatRect & t_rect)
    {
        // place just above the spawn point so the player falls a little when entering a new map
        m_sprite.setPosition(
            { util::center(t_rect).x, (t_rect.position.y - m_sprite.getGlobalBounds().size.y) });
    }

    const sf::FloatRect Avatar::collisionRect() const
    {
        const sf::FloatRect bounds{ m_sprite.getGlobalBounds() };
        sf::FloatRect rect{ bounds };
        util::scaleRectInPlace(rect, m_avatarSizeRatio);
        rect.position.y += (bounds.size.x * 0.175f);

        if (m_isFacingRight)
        {
            rect.position.x -= (bounds.size.x * m_facingDirectionOffsetRatio);
        }
        else
        {
            rect.position.x += (bounds.size.x * m_facingDirectionOffsetRatio);
        }

        return rect;
    }

    void Avatar::changeType(const Context & t_context)
    {
        AvatarTextureManager::instance().release(m_type);
        m_type = t_context.player.avatarType();
        AvatarTextureManager::instance().acquire(t_context, m_type);
        calculateMovementDetails(t_context);
    }

    const sf::FloatRect Avatar::attackRect() const
    {
        if ((AvatarState::Attack != m_state) && (AvatarState::AttackExtra != m_state))
        {
            return {};
        }

        sf::FloatRect rect{ collisionRect() };

        // make the attack rect 2 pixels bigger vertically so players can attack up and down
        rect.size.y += 4.0f;
        rect.position.y -= 2.0f;

        // these avatars have long swords that reach farther
        if ((AvatarType::BlueKnight == m_type) || (AvatarType::RedKnight == m_type) ||
            (AvatarType::Viking == m_type))
        {
            rect.size.x *= 1.2f; // TODO move this to settings
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

    void Avatar::animate(const Context &, const float t_frameTimeSec)
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

    void Avatar::gravityMotion(const float t_frameTimeSec)
    {
        if (AvatarState::Climb != m_state)
        {
            m_velocity.y += (m_movement.gravity * t_frameTimeSec);
            m_sprite.move(m_velocity);
        }
    }

    void Avatar::climbingMotion(const Context & t_context, const float t_frameTimeSec)
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

    void Avatar::handleAttackState(const Context & t_context)
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

                        const float facingDirectionOffsetRatio{ 1.5f };
                        if (m_isFacingRight)
                        {
                            pos.x += (collRect.size.x * facingDirectionOffsetRatio);
                        }
                        else
                        {
                            pos.x -= (collRect.size.x * facingDirectionOffsetRatio);
                        }

                        return pos;
                    }();

                    t_context.player.castCurrentSpell(t_context, spellAnimPos, m_isFacingRight);
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

    void Avatar::handleAttackingEnemies(const Context & t_context)
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

        if (t_context.level.monsters().avatarAttack(t_context, attackInfo))
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

    void Avatar::moveMap(const Context & t_context)
    {
        const sf::FloatRect wholeRect{ t_context.layout.wholeRect() };
        const sf::Vector2f avatarMiddle{ util::center(m_sprite.getGlobalBounds()) };

        // moving right creates a negative move.x (can't move left)
        if (const sf::Vector2f screenMiddle{ util::center(wholeRect) };
            (m_velocity.x > 0.0f) && (avatarMiddle.x > screenMiddle.x))
        {
            const sf::Vector2f move{ (screenMiddle.x - avatarMiddle.x), 0.0f };
            if (t_context.level.move(t_context, move))
            {
                m_sprite.move(move);
                m_runParticleEffect.move(move);
            }
        }

        // moving up creates a positive move.y
        if (const float riseMovePoint{ wholeRect.size.y * 0.12f };
            (m_velocity.y < 0.0f) && (avatarMiddle.y < riseMovePoint))
        {
            const sf::Vector2f move{ 0.0f, (riseMovePoint - avatarMiddle.y) };
            if (t_context.level.move(t_context, move))
            {
                m_sprite.move(move);
                m_runParticleEffect.move(move);
            }
        }

        // moving down creates a negative move.y
        if (const float fallMovePoint{ wholeRect.size.y * 0.5f };
            (m_velocity.y > 0.0f) && (avatarMiddle.y > fallMovePoint))
        {
            const sf::Vector2f move{ 0.0f, (fallMovePoint - avatarMiddle.y) };
            if (t_context.level.move(t_context, move))
            {
                m_sprite.move(move);
                m_runParticleEffect.move(move);
            }
        }
    }

    void Avatar::killIfOutOfBounds(const Context & t_context)
    {
        if (!t_context.layout.wholeRect().findIntersection(collisionRect()))
        {
            triggerDeath(t_context);
        }
    }

    void Avatar::preventBacktracking(const Context & t_context)
    {
        // anything about the size of the avatar or bigger will work here
        const float collisionWidth{ m_sprite.getGlobalBounds().size.x };

        // double tall to prevent jumping backwards offscreen
        const float collisionHeight{ t_context.layout.wholeSize().y * 2.0f };

        const sf::FloatRect backtrackRect{ { -collisionWidth, -collisionHeight },
                                           { collisionWidth, (collisionHeight * 2.0f) } };

        const auto intersectionOpt{ collisionRect().findIntersection(backtrackRect) };
        if (intersectionOpt.has_value())
        {
            m_velocity.x = 0.0f;
            m_sprite.move({ intersectionOpt.value().size.x, 0.0f }); // only allow pushing right
        }
    }

    void Avatar::collisions(const Context & t_context)
    {
        bool detectLanding{ false };
        const sf::FloatRect avatarRect{ collisionRect() };

        for (const sf::FloatRect & collRect : t_context.level.collisions())
        {
            const auto intersectionOpt{ avatarRect.findIntersection(collRect) };
            if (intersectionOpt)
            {
                collide(t_context, avatarRect, intersectionOpt.value(), detectLanding);
            }
        }

        for (const sf::FloatRect & collRect : t_context.level.layerCollisions())
        {
            const auto intersectionOpt{ avatarRect.findIntersection(collRect) };
            if (intersectionOpt)
            {
                collide(t_context, avatarRect, intersectionOpt.value(), detectLanding);
            }
        }

        collisionRectCache.clear();
        t_context.level.monsters().appendCollisionRects(collisionRectCache);
        for (const sf::FloatRect & collRect : collisionRectCache)
        {
            const auto intersectionOpt{ avatarRect.findIntersection(collRect) };
            if (intersectionOpt)
            {
                collide(t_context, avatarRect, intersectionOpt.value(), detectLanding);
            }
        }

        if (!detectLanding)
        {
            m_hasLanded = false;
        }
    }

    void Avatar::collide(
        const Context & t_context,
        const sf::FloatRect & t_avatarRect,
        const sf::FloatRect & t_intersectionRect,
        bool & t_detectLanding)
    {
        const sf::Vector2f avatarCenter{ util::center(t_avatarRect) };
        const sf::Vector2f intersectionCenter{ util::center(t_intersectionRect) };

        if ((m_velocity.y < 0.0f) && (intersectionCenter.y < avatarCenter.y))
        {
            // rising and hit something above
            m_velocity.y = 0.0f;
            m_sprite.move({ 0.0f, t_intersectionRect.size.y });
        }
        else if (
            (m_velocity.y > 0.0f) && (t_intersectionRect.size.y < 25.0f) &&
            (intersectionCenter.y > avatarCenter.y))
        {
            // falling and hit something below
            if (!m_hasLanded)
            {
                t_context.sfx.play("land");
                m_state = AvatarState::Still;
                m_anim  = AvatarAnim::Walk;
                restartAnim();
            }

            m_hasLanded     = true;
            t_detectLanding = true;
            m_velocity.y    = 0.0f;
            m_sprite.move({ 0.0f, -t_intersectionRect.size.y });
        }
        else
        {
            // at this point we hit something from the side
            m_velocity.x = 0.0f;

            if (intersectionCenter.x > avatarCenter.x)
            {
                m_sprite.move({ -t_intersectionRect.size.x, 0.0f });
            }
            else
            {
                m_sprite.move({ t_intersectionRect.size.x, 0.0f });
            }
        }
    }

    void Avatar::calculateMovementDetails(const Context & t_context)
    {
        m_movement.gravity = t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.gravity_acc, CalcOrder::Inverse);

        m_movement.walk_speed_limit = t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.walk_speed_limit);

        m_movement.walk_acc =
            t_context.layout.calScaleBasedOnResolution(t_context, t_context.settings.walk_acc);

        m_movement.run_speed_limit = t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.run_speed_limit);

        m_movement.run_acc =
            t_context.layout.calScaleBasedOnResolution(t_context, t_context.settings.run_acc);

        m_movement.ladder_speed =
            t_context.layout.calScaleBasedOnResolution(t_context, t_context.settings.ladder_speed);

        m_movement.jump_acc = t_context.settings.jump_acc;

        m_movement.high_jump_acc = t_context.settings.high_jump_acc;

        if (isWizard(m_type))
        {
            const float wizardSpeedRatio{ t_context.settings.wizard_walk_run_speed_adj_ratio };
            m_movement.walk_speed_limit *= wizardSpeedRatio;
            m_movement.walk_acc *= wizardSpeedRatio;
            m_movement.run_speed_limit *= wizardSpeedRatio;
            m_movement.run_acc *= wizardSpeedRatio;

            const float wizardJumpAccOffset{ t_context.settings.wizard_jump_acc_offset };
            m_movement.jump_acc += wizardJumpAccOffset;
            m_movement.high_jump_acc += wizardJumpAccOffset;
        }
        else if (isRaider(m_type))
        {
            const float raiderSpeedRatio{ t_context.settings.raider_walk_run_speed_adj_ratio };
            m_movement.walk_speed_limit *= raiderSpeedRatio;
            m_movement.walk_acc *= raiderSpeedRatio;
            m_movement.run_speed_limit *= raiderSpeedRatio;
            m_movement.run_acc *= raiderSpeedRatio;

            const float raiderJumpAccOffset{ t_context.settings.raider_jump_acc_offset };
            m_movement.jump_acc += raiderJumpAccOffset;
            m_movement.high_jump_acc += raiderJumpAccOffset;
        }
    }

    void Avatar::sideToSideMotion(const Context & t_context, const float t_frameTimeSec)
    {
        if ((AvatarState::Hurt == m_state) || (AvatarState::Attack == m_state) ||
            (AvatarState::AttackExtra == m_state) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
        {
            return;
        }

        const auto enforceSpeedLimitWalk = [&]() {
            if (m_velocity.x > m_movement.walk_speed_limit)
            {
                m_velocity.x = m_movement.walk_speed_limit;
            }
            else if (m_velocity.x < -m_movement.walk_speed_limit)
            {
                m_velocity.x = -m_movement.walk_speed_limit;
            }
        };

        const auto enforceSpeedLimitRun = [&]() {
            if (m_velocity.x > m_movement.run_speed_limit)
            {
                m_velocity.x = m_movement.run_speed_limit;
            }
            else if (m_velocity.x < -m_movement.run_speed_limit)
            {
                m_velocity.x = -m_movement.run_speed_limit;
            }
        };

        if ((AvatarState::Jump == m_state) || (AvatarState::JumpHigh == m_state))
        {
            // Allow moving side-to-side at a reduced rate while in the air.
            // It sounds wrong but feels so right. What the hell, mario did it.
            const float jumpMoveDivisor{ t_context.settings.avatar_jump_horiz_move_divisor };

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
            {
                m_velocity.x += ((m_movement.walk_acc / jumpMoveDivisor) * t_frameTimeSec);
                enforceSpeedLimitWalk();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
            {
                m_velocity.x -= ((m_movement.walk_acc / jumpMoveDivisor) * t_frameTimeSec);
                enforceSpeedLimitWalk();
            }

            return;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift))
            {
                m_velocity.x += (m_movement.run_acc * t_frameTimeSec);
                enforceSpeedLimitRun();

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
                enforceSpeedLimitWalk();

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
                enforceSpeedLimitRun();

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
                enforceSpeedLimitWalk();

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

    void Avatar::jumpingMotion(const Context & t_context, const float t_frameTimeSec)
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

    void Avatar::triggerDeath(const Context & t_context)
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

    bool Avatar::handleDeath(const Context & t_context, const float t_frameTimeSec)
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

    void Avatar::respawn(const Context & t_context)
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

    void Avatar::exitCollisions(const Context & t_context) const
    {
        if (collisionRect().findIntersection(t_context.level.exitRect()))
        {
            t_context.sfx.stopAllLooped();
            t_context.state.setChangePending(State::LevelComplete);
        }
    }

    void Avatar::hurtCollisions(const Context & t_context)
    {
        if (AvatarState::Hurt == m_state)
        {
            return;
        }

        const sf::FloatRect avatarRect{ collisionRect() };
        harm(t_context, HarmCollisionManager::instance().avatarCollide(t_context, avatarRect));
        harm(t_context, t_context.level.monsters().avatarCollide(avatarRect));
    }

    void Avatar::killCollisions(const Context & t_context)
    {
        const sf::FloatRect collRect{ collisionRect() };

        for (const sf::FloatRect & killRect : t_context.level.killCollisions())
        {
            if (collRect.findIntersection(killRect))
            {
                t_context.player.healthAdjust(t_context, -t_context.player.health());
                triggerDeath(t_context);
                break;
            }
        }
    }

    void Avatar::harm(const Context & t_context, const Harm & t_harm)
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

        const sf::Vector2f recoilSpeed{ t_context.settings.avatar_hurt_recoil_speed };
        m_velocity.y = -recoilSpeed.y;

        const float collisionRectCenterHoriz{ util::center(t_harm.rect).x };
        if (collisionRectCenterHoriz < util::center(collisionRect()).x)
        {
            m_velocity.x = recoilSpeed.x;
        }
        else
        {
            m_velocity.x = -recoilSpeed.x;
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
