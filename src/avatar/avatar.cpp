// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// avatar.hpp
//
#include "avatar/avatar.hpp"

#include "avatar/spells-anim.hpp"
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
#include "subsystem/screen-layout.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

#include <SFML/Window/Keyboard.hpp>

namespace bramblefore
{

    Avatar::Avatar()
        : m_sprite{}
        , m_type{ AvatarType::Count } // anything works here
        , m_anim{ AvatarAnim::Walk }
        , m_state{ AvatarState::Still }
        , m_elapsedTimeSec{ 0.0f }
        , m_deathDelayElapsedTimeSec{ 0.0f }
        , m_animIndex{ 0 }
        , m_velocity{}
        , m_hasLanded{ false }
        , m_isFacingRight{ true }
        , m_avatarImageWidthRatio{ 0.25f }
        , m_isAnimating{ false }
        , m_hasHitEnemy{ false }
        , m_spellAnim{}
    {}

    Avatar::~Avatar() { AvatarTextureManager::instance().release(m_type); }

    void Avatar::setup(const Context & t_context)
    {
        m_spellAnim.setup(t_context);

        m_type  = t_context.player.avatarType();
        m_anim  = AvatarAnim::Walk;
        m_state = AvatarState::Still;

        AvatarTextureManager & textureManager{ AvatarTextureManager::instance() };
        textureManager.acquire(t_context, m_type);
        textureManager.set(m_sprite, m_type, m_anim, 0);

        const float scale{ t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.avatar_scale) };

        m_sprite.setScale(scale, scale);
    }

    void Avatar::update(Context & t_context, const float t_frameTimeSec)
    {
        m_spellAnim.update(t_frameTimeSec);

        // this handleDeath() call must happen first
        if (handleDeath(t_context, t_frameTimeSec))
        {
            return;
        }

        // handleAttacking() should be called BEFORE these three
        handleAttackState(t_context);
        handleAttackingEnemies(t_context);
        sideToSideMotion(t_context, t_frameTimeSec);
        jumping(t_context, t_frameTimeSec);

        m_velocity.y += (t_context.settings.gravity_acc * t_frameTimeSec);
        m_sprite.move(m_velocity);

        // moveMap() and collisions should be called AFTER m_sprite.move() above
        moveMap(t_context);
        preventBacktracking(t_context);
        collisions(t_context);
        exitCollisions(t_context);
        hurtCollisions(t_context);
        // t_context.managers.collideAllWithAvatar(t_context, collisionRect());
        t_context.pickup.processCollisionWithAvatar(t_context, collisionRect());

        // these two must happen last
        killIfOutOfBounds(t_context);
        animate(t_context, t_frameTimeSec);
    }

    void Avatar::draw(sf::RenderTarget & t_target, sf::RenderStates t_states)
    {
        t_target.draw(m_sprite, t_states);
        m_spellAnim.draw(t_target, t_states);
    }

    void Avatar::setPosition(const sf::FloatRect & t_rect)
    {
        m_sprite.setPosition(util::center(t_rect).x, util::bottom(t_rect));
        m_sprite.move(0.0f, (-110.0f * m_sprite.getScale().y));
    }

    sf::FloatRect Avatar::collisionRect() const
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

    void Avatar::changeType(Context & t_context)
    {
        AvatarTextureManager::instance().release(m_type);
        m_type = t_context.player.avatarType();
        AvatarTextureManager::instance().acquire(t_context, m_type);
    }

    sf::FloatRect Avatar::attackRect() const
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

    void Avatar::handleAttackState(Context & t_context)
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
                t_context.sfx.play("swipe", 0.6f);
                m_state = AvatarState::AttackExtra;
                m_anim  = AvatarAnim::AttackExtra;
                m_spellAnim.add(spellAnimPos, m_type, false, m_isFacingRight);
            }
            else
            {
                t_context.sfx.play("swipe");
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

        AttackInfo attackInfo;
        attackInfo.rect = attackRect();

        // TODO this needs to be done differently
        if ((AvatarType::Assassin == m_type) || (AvatarType::Ninja == m_type) ||
            (AvatarType::Rogue == m_type))
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

        if (t_context.level.monsters.avatarAttack(t_context, attackInfo))
        {
            m_hasHitEnemy = true;
            if (isSpellCaster(m_type))
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
        const float screenMiddle = (t_context.layout.wholeRect().width * 0.5f);

        if ((m_velocity.x < 0.0f) || (posXAfter < screenMiddle))
        {
            return;
        }

        const float moveX = (screenMiddle - posXAfter);
        if (!t_context.level.move(t_context, moveX))
        {
            return;
        }

        m_sprite.move(moveX, 0.0f);
        t_context.accent.move(moveX);
        t_context.pickup.move(moveX);
        t_context.level.move(t_context, moveX);
        t_context.pickup.move(moveX);
        t_context.bg_image.move(moveX);
        t_context.spell.move(moveX);
        t_context.float_text.move(moveX);
    }

    void Avatar::killIfOutOfBounds(Context & t_context)
    {
        if (!t_context.layout.wholeRect().intersects(collisionRect()))
        {
            triggerDeath(t_context);
        }
    }

    void Avatar::preventBacktracking(const Context & t_context)
    {
        const sf::FloatRect backtrackRect{ -100.0f, 0.0f, 100.0f, t_context.layout.wholeSize().y };

        sf::FloatRect intersection;
        if (collisionRect().intersects(backtrackRect, intersection))
        {
            m_velocity.x = 0.0f;
            m_sprite.move(intersection.width, 0.0f);
        }
    }

    void Avatar::collisions(Context & t_context)
    {
        const float tolerance = 25.0f; // this magic number brought to you by zTn 2021-8-2

        const sf::FloatRect avatarRect  = collisionRect();
        const sf::Vector2f avatarCenter = util::center(avatarRect);

        const float footRectHeightAdj{ avatarRect.height * 0.8f };
        sf::FloatRect footRect = avatarRect;
        footRect.top += footRectHeightAdj;
        footRect.height -= footRectHeightAdj;

        std::vector<sf::FloatRect> rects{ t_context.level.collisions };
        t_context.level.monsters.appendCollisionRects(rects);

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
                    t_context.sfx.play("land");
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

    void Avatar::sideToSideMotion(Context & t_context, const float t_frameTimeSec)
    {
        if ((AvatarState::Hurt == m_state) || (AvatarState::Attack == m_state) ||
            (AvatarState::AttackExtra == m_state))
        {
            return;
        }

        const float walkSpeedLimit{ t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.walk_speed_limit) };

        const float walkAcceleration{ t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.walk_acceleration) };

        const float runSpeedLimit{ t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.run_speed_limit) };

        const float runAcceleration{ t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.run_acceleration) };


        if ((AvatarState::Jump == m_state) || (AvatarState::JumpHigh == m_state))
        {
            // Allow moving side-to-side at a reduced rate while in the air.
            // It sounds wrong but feels so right.
            // What the hell, mario did it.
            const float jumpMoveDivisor = 3.0f;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                m_velocity.x += ((walkAcceleration / jumpMoveDivisor) * t_frameTimeSec);

                if (m_velocity.x > walkSpeedLimit)
                {
                    m_velocity.x = walkSpeedLimit;
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                m_velocity.x -= ((walkAcceleration / jumpMoveDivisor) * t_frameTimeSec);

                if (m_velocity.x < -walkSpeedLimit)
                {
                    m_velocity.x = -walkSpeedLimit;
                }
            }

            return;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                m_velocity.x += (runAcceleration * t_frameTimeSec);
                if (m_velocity.x > runSpeedLimit)
                {
                    m_velocity.x = runSpeedLimit;
                }

                if (AvatarState::Run != m_state)
                {
                    restartAnim();
                    t_context.sfx.play("walk");
                }

                m_state = AvatarState::Run;
                m_anim  = AvatarAnim::Run;
            }
            else
            {
                m_velocity.x += (walkAcceleration * t_frameTimeSec);
                if (m_velocity.x > walkSpeedLimit)
                {
                    m_velocity.x = walkSpeedLimit;
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
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                m_velocity.x -= (runAcceleration * t_frameTimeSec);
                if (m_velocity.x < -runSpeedLimit)
                {
                    m_velocity.x = -runSpeedLimit;
                }

                if (AvatarState::Run != m_state)
                {
                    restartAnim();
                    t_context.sfx.play("walk");
                }

                m_state = AvatarState::Run;
                m_anim  = AvatarAnim::Run;
            }
            else
            {
                m_velocity.x -= (walkAcceleration * t_frameTimeSec);
                if (m_velocity.x < -walkSpeedLimit)
                {
                    m_velocity.x = -walkSpeedLimit;
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
            m_velocity.x = 0.0f;
            m_state      = AvatarState::Still;
            m_anim       = AvatarAnim::Walk;
            restartAnim();
            t_context.sfx.stop("walk");
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_hasLanded)
        {
            m_hasLanded = false;
            t_context.sfx.stop("walk");
            // t_context.sfx.play("jump"); //TODO

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                m_state = AvatarState::JumpHigh;
                m_anim  = AvatarAnim::JumpHigh;
                m_velocity.y -= (t_context.settings.high_jump_acc * t_frameTimeSec);
            }
            else
            {
                m_state = AvatarState::Jump;
                m_anim  = AvatarAnim::Jump;
                m_velocity.y -= (t_context.settings.jump_acc * t_frameTimeSec);
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
        t_context.sfx.stop("walk");
        t_context.sfx.play("death-avatar");
        m_velocity = { 0.0f, 0.0f };

        // t_context.stats.has_player_died = true;
        // t_context.stats.enemy_killed    = 0;
        // t_context.stats.coin_collected  = 0;
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
                t_context.state.setChangePending(State::LevelDeath);
            }
        }

        animate(t_context, t_frameTimeSec);
        return true;
    }

    void Avatar::respawn(Context & t_context)
    {
        t_context.accent.clear();
        t_context.pickup.clear();
        t_context.spell.clear();
        t_context.float_text.clear();

        t_context.player.healthReset(t_context);
        t_context.player.manaReset(t_context);
        // TODO reset stars as well?

        t_context.level.load(t_context);

        m_state = AvatarState::Still;
        m_anim  = AvatarAnim::Walk;
        restartAnim();
        m_hasLanded = false;
        m_velocity  = { 0.0f, 0.0f };

        if (!m_isFacingRight)
        {
            turnRight();
        }

        t_context.sfx.play("spawn");
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

    void Avatar::exitCollisions(Context & t_context) const
    {
        if (collisionRect().intersects(t_context.level.exit_rect))
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

        if (t_context.player.health() == 0)
        {
            triggerDeath(t_context);
        }
        else
        {
            t_context.sfx.play("hurt-avatar");
        }
    }

} // namespace bramblefore
