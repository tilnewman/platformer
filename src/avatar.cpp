// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// avatar.hpp
//
#include "avatar.hpp"

#include "accents.hpp"
#include "context.hpp"
#include "level.hpp"
#include "pickups.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "sound-player.hpp"
#include "spells.hpp"

#include <filesystem>

#include <SFML/Window/Keyboard.hpp>

namespace platformer
{

    Avatar::Avatar()
        : m_sprite()
        , m_type(AvatarType::Assassin) // anything works here
        , m_anim(AvatarAnim::Idle)
        , m_elapsedTimeSec(0.0f)
        , m_animIndex(0)
        , m_velocity()
        , m_hasLanded(false)
        , m_isFacingRight(true)
    {}

    void Avatar::setup(const Context & context, const AvatarType & type)
    {
        m_type = type;
        m_anim = AvatarAnim::Idle;
        m_sprite.setTexture(context.avatar_textures.get(m_type, m_anim).textures.at(0), true);
        m_sprite.setScale(context.settings.avatar_scale, context.settings.avatar_scale);
    }

    void Avatar::update(Context & context, const float frameTimeSec)
    {
        // if (handleDeath(context, frameTimeSec))
        //{
        //    return;
        //}
        //
        // const bool isAttacking = handleAttacking(context, frameTimeSec);
        // const bool isThrowing  = handleThrowing(context, frameTimeSec);
        // const bool isGliding   = handleGliding(context, frameTimeSec);

        // if (!isAttacking && !isThrowing && !isGliding)
        //{
        sideToSideMotion(context, frameTimeSec);
        jumping(context, frameTimeSec);
        //}

        m_velocity += (context.settings.gravity_acc * frameTimeSec);
        m_sprite.move(m_velocity);
        moveMap(context);

        preventBacktracking(context);
        collisions(context);
        // killCollisions(context);
        // acidCollisions(context);
        // waterCollisions(context);
        // exitCollisions(context);
        // handleAttackingEnemies(context);

        // context.managers.collideAllWithAvatar(context, collisionRect());

        // if (context.managers.doesAvatarCollideWithAnyAndDie(collisionRect()))
        //{
        //    triggerDeath(context);
        //}

        killIfOutOfBounds(context);

        // this one must come last, after all possible sets to m_action
        // handleIdle(context, frameTimeSec);

        animate(context, frameTimeSec);
    }

    void Avatar::draw(sf::RenderTarget & target, sf::RenderStates states)
    {
        target.draw(m_sprite, states);
        util::drawRectangleShape(target, collisionRect(), false, sf::Color::Red);
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
        util::scaleRectInPlace(rect, { 0.25f, 0.35f });
        rect.left -= (bounds.width * 0.15f);
        rect.top += (bounds.width * 0.175f);
        return rect;
    }

    void Avatar::animate(Context & context, const float frameTimeSec)
    {
        const AnimTextures & textureSet{ context.avatar_textures.get(m_type, m_anim) };

        m_elapsedTimeSec += frameTimeSec;
        if (m_elapsedTimeSec > textureSet.time_per_frame_sec)
        {
            m_elapsedTimeSec -= textureSet.time_per_frame_sec;

            ++m_animIndex;
            if (m_animIndex >= textureSet.textures.size())
            {
                m_animIndex = 0;
            }

            if (textureSet.textures.empty())
            {
                m_sprite.setTexture(context.avatar_textures.getDefault(m_type), true);
            }
            else
            {
                m_sprite.setTexture(textureSet.textures.at(m_animIndex), true);
            }
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
        context.accents.move(moveX);
        context.level.move(context, moveX);
        context.pickups.move(moveX);
        // context.spells.move(moveX); //TODO
    }

    void Avatar::killIfOutOfBounds(Context & context)
    {
        if (!context.layout.wholeRect().intersects(collisionRect()))
        {
            // triggerDeath(context); //TODO
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

        std::vector<sf::FloatRect> rects = context.level.collisions;
        // context.managers.appendAllCollisions(rects); TODO

        bool hasHitSomething{ false };
        sf::FloatRect intersection;
        for (const sf::FloatRect & collRect : rects)
        {
            if (!avatarRect.intersects(collRect, intersection))
            {
                continue;
            }

            hasHitSomething               = true;
            const sf::Vector2f collCenter = util::center(collRect);

            if ((m_velocity.y < 0.0f) && (collCenter.y < avatarCenter.y))
            {
                // rising and hit something above

                m_velocity.y = 0.0f;
                m_sprite.move(0.0f, intersection.height);
                continue;
            }

            const bool doesIntersectFeet = collRect.intersects(footRect);

            if ((m_velocity.y > 0.0f) && (intersection.height < tolerance) && doesIntersectFeet)
            {
                // falling and hit something below

                if (!m_hasLanded)
                {
                    context.sfx.play("land");
                    m_anim = AvatarAnim::Idle;
                    restartAnim();
                }

                m_hasLanded  = true;
                m_velocity.y = 0.0f;
                m_sprite.move(0.0f, -intersection.height);
                continue;
            }

            // at this point we hit something from the side

            if (intersection.width < tolerance)
            {
                if (m_velocity.x > 0.0f)
                {
                    m_velocity.x = 0.0f;
                    m_sprite.move(-intersection.width, 0.0f);
                    continue;
                }
                else if (m_velocity.x < 0.0f)
                {
                    m_velocity.x = 0.0f;
                    m_sprite.move(intersection.width, 0.0f);
                    continue;
                }
            }

            hasHitSomething = false;
        }

        if (!hasHitSomething)
        {
            m_hasLanded = false;
        }
    }

    void Avatar::sideToSideMotion(Context & context, const float frameTimeSec)
    {
        if (AvatarAnim::Jump == m_anim)
        {
            // Allow moving side-to-side at a reduced rate while in the air.
            // It sounds wrong but feels so right.
            // What the hell, mario did it.
            const float jumpMoveDivisor = 3.0f;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                m_velocity.x += ((context.settings.walk_acc / jumpMoveDivisor) * frameTimeSec);
                if (m_velocity.x > context.settings.walk_speed_limit)
                {
                    m_velocity.x = context.settings.walk_speed_limit;
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                m_velocity.x -= ((context.settings.walk_acc / jumpMoveDivisor) * frameTimeSec);
                if (m_velocity.x < -context.settings.walk_speed_limit)
                {
                    m_velocity.x = -context.settings.walk_speed_limit;
                }
            }
        }
        else
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                m_velocity.x += (context.settings.walk_acc * frameTimeSec);
                if (m_velocity.x > context.settings.walk_speed_limit)
                {
                    m_velocity.x = context.settings.walk_speed_limit;
                }

                if (AvatarAnim::Run != m_anim)
                {
                    restartAnim();
                }

                m_anim = AvatarAnim::Run;
                context.sfx.play("walk");

                if (!m_isFacingRight)
                {
                    m_isFacingRight = true;
                    m_sprite.scale(-1.0f, 1.0f); // sfml trick to horiz flip image
                    m_sprite.move(-m_sprite.getGlobalBounds().width, 0.0f);
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                m_velocity.x -= (context.settings.walk_acc * frameTimeSec);
                if (m_velocity.x < -context.settings.walk_speed_limit)
                {
                    m_velocity.x = -context.settings.walk_speed_limit;
                }

                if (AvatarAnim::Run != m_anim)
                {
                    restartAnim();
                }

                m_anim = AvatarAnim::Run;
                context.sfx.play("walk");

                if (m_isFacingRight)
                {
                    m_isFacingRight = false;
                    m_sprite.scale(-1.0f, 1.0f); // sfml trick to horiz flip image
                    m_sprite.move(m_sprite.getGlobalBounds().width, 0.0f);
                }
            }
            else
            {
                m_velocity.x = 0.0f;
                m_anim       = AvatarAnim::Idle;
                restartAnim();
                context.sfx.stop("walk");
            }
        }
    }

    void Avatar::restartAnim()
    {
        m_animIndex      = 0;
        m_elapsedTimeSec = 0.0f;
    }

    void Avatar::jumping(Context & context, const float frameTimeSec)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_hasLanded)
        {
            m_hasLanded = false;
            m_velocity.y -= (context.settings.jump_acc * frameTimeSec);
            context.sfx.play("jump");
            context.sfx.stop("walk");
            m_anim = AvatarAnim::Jump;
            restartAnim();
        }
    }

} // namespace platformer
