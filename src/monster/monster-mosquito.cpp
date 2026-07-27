//
// monster-mosquito.cpp
//
#include "monster-mosquito.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "map/level.hpp"
#include "monster/monster-manager.hpp"
#include "monster/mosquito-textures.hpp"
#include "subsystem/context.hpp"
#include "subsystem/floating-text.hpp"
#include "subsystem/font.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/filesystem-util.hpp"
#include "util/random.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <filesystem>

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    Mosquito::Mosquito(const Context & t_context, const sf::FloatRect & t_region)
        : m_health{ startingHealth(type()) }
        , m_anim{ MosquitoAnim::Idle }
        , m_task{ MosquitoTask::Idle }
        , m_sprite{ util::SfmlDefaults::instance().texture() }
        , m_elapsedAnimTimeSec{ 0.0f }
        , m_taskTimeRemainingSec{ randomIdleDurationSec(t_context) }
        , m_flyBounds{ t_region }
        , m_isFacingRight{ true }
        , m_resetPosition{}
        , m_frameIndex{ 0 }
        , m_speedMult{ t_context.random.fromTo(0.8f, 1.2f) }
        , m_isAlive{ true }
        , m_resetDistance{ 0.0f }
    {
        MosquitoTextureManager::instance().acquire(t_context);
    }

    Mosquito::~Mosquito() { MosquitoTextureManager::instance().release(); }

    void Mosquito::setup(const Context & t_context)
    {
        // setup the sprite
        const auto & texturesVec{ MosquitoTextureManager::instance().textures() };

        m_sprite.setTexture(
            texturesVec.at(static_cast<std::size_t>(MosquitoAnim::Idle)).front(), true);

        const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.0f) *
                           t_context.settings.monster_scale };

        m_sprite.setScale({ scale, scale });

        m_sprite.setPosition(randomResetPosition(t_context));

        // randomize facing direction
        if (t_context.random.boolean())
        {
            turnAround();
        }
    }

    void Mosquito::setupTask(
        const Context & t_context, const MosquitoTask t_task, const MosquitoAnim t_anim)
    {
        m_task                 = t_task;
        m_anim                 = t_anim;
        m_frameIndex           = 0;
        m_elapsedAnimTimeSec   = 0.0f;
        m_resetPosition        = randomResetPosition(t_context);
        m_taskTimeRemainingSec = randomIdleDurationSec(t_context);

        m_resetDistance =
            (t_context.layout.calScaleBasedOnResolution(
                 t_context, t_context.random.fromTo(150.0f, 300.0f)) *
             t_context.settings.map_scale);

        m_hasSpottedPlayer = ((MosquitoTask::Attack == t_task) || (MosquitoTask::Reset == t_task));

        if ((MosquitoTask::Reset == t_task) || (MosquitoTask::Wander == t_task))
        {
            turnToFacePosition(m_resetPosition);
        }
    }

    void Mosquito::update(const Context & t_context, const float t_elapsedTimeSec)
    {
        if (!m_isAlive)
        {
            return;
        }

        // animate frame by frame
        m_elapsedAnimTimeSec += t_elapsedTimeSec;
        const float timeBetweenFrames{ toTimeBetweenFrames(m_anim) };
        if (m_elapsedAnimTimeSec > timeBetweenFrames)
        {
            m_elapsedAnimTimeSec -= timeBetweenFrames;

            const auto & texturesVec{ MosquitoTextureManager::instance().textures() };
            auto & textures{ texturesVec.at(static_cast<std::size_t>(m_anim)) };

            if (++m_frameIndex >= textures.size())
            {
                m_frameIndex = 0;

                if (MosquitoAnim::AttackPrep == m_anim)
                {
                    m_anim = MosquitoAnim::AttackCycle;
                }
                else if (MosquitoAnim::Death == m_anim)
                {
                    handleDying(t_context);
                    return;
                }
                else if (MosquitoAnim::Hurt == m_anim)
                {
                    setupTask(t_context, MosquitoTask::Reset, MosquitoAnim::Flying);
                    return;
                }
            }

            m_sprite.setTexture(textures.at(m_frameIndex));
        }

        // spot the player if not already
        if ((MosquitoTask::Hurt != m_task) && (MosquitoTask::Death != m_task) &&
            !m_hasSpottedPlayer &&
            t_context.avatar.collisionRect().findIntersection(spottedRect(t_context)))
        {
            if (t_context.random.boolean())
            {
                setupTask(t_context, MosquitoTask::Reset, MosquitoAnim::Flying);
            }
            else
            {
                setupTask(t_context, MosquitoTask::Attack, MosquitoAnim::AttackPrep);
            }

            // TODO play a buzzing "i see you" sfx
            return;
        }

        // change tasks if time to do so
        m_taskTimeRemainingSec -= t_elapsedTimeSec;
        if (m_taskTimeRemainingSec < 0.0f)
        {
            m_taskTimeRemainingSec = 0.0f;

            if (MosquitoTask::Idle == m_task)
            {
                if (t_context.random.fromTo(1, 10) <= 3)
                {
                    setupTask(t_context, MosquitoTask::Idle, MosquitoAnim::Idle);
                    turnAround();
                    return;
                }
                else
                {
                    setupTask(t_context, MosquitoTask::Wander, MosquitoAnim::Flying);
                    return;
                }
            }
        }

        const float distanceCloseEnough{ 5.0f };

        if (MosquitoTask::Wander == m_task)
        {
            const sf::Vector2f mosquitoPos{ util::center(m_sprite.getGlobalBounds()) };

            if (util::distance(m_resetPosition, mosquitoPos) < distanceCloseEnough)
            {
                setupTask(t_context, MosquitoTask::Idle, MosquitoAnim::Idle);
            }
            else
            {
                const sf::Vector2f diffVec{ util::normalize(m_resetPosition - mosquitoPos) };

                m_sprite.move(
                    diffVec * (flyingSpeed(MosquitoTask::Wander) * m_speedMult * t_elapsedTimeSec));
            }
        }
        else if (MosquitoTask::Reset == m_task)
        {
            const sf::Vector2f playerPos{ util::center(t_context.avatar.collisionRect()) };
            const sf::Vector2f mosquitoPos{ util::center(m_sprite.getGlobalBounds()) };

            if ((util::distance(playerPos, mosquitoPos) > m_resetDistance) ||
                (util::distance(m_resetPosition, mosquitoPos) < distanceCloseEnough))
            {
                setupTask(t_context, MosquitoTask::Attack, MosquitoAnim::AttackPrep);
            }
            else
            {
                const sf::Vector2f diffVec{ util::normalize(m_resetPosition - mosquitoPos) };

                m_sprite.move(
                    diffVec * (flyingSpeed(MosquitoTask::Reset) * m_speedMult * t_elapsedTimeSec));
            }
        }
        else if (MosquitoTask::Attack == m_task)
        {
            turnToFacePlayer(t_context);

            const sf::FloatRect playerRect{ t_context.avatar.collisionRect() };
            const sf::Vector2f playerPos{ util::center(playerRect) };
            const sf::Vector2f mosquitoPos{ util::center(m_sprite.getGlobalBounds()) };
            const sf::Vector2f diffVec{ util::normalize(playerPos - mosquitoPos) };
            const sf::FloatRect monsterRect{ collisionRect() };
            const sf::FloatRect attackRect{ attackCollisionRect() };

            const sf::Vector2f move{ diffVec * (flyingSpeed(MosquitoTask::Attack) * m_speedMult *
                                                t_elapsedTimeSec) };

            m_sprite.move(move);

            if (attackRect.findIntersection(playerRect))
            {
                setupTask(t_context, MosquitoTask::Reset, MosquitoAnim::Flying);

                Harm harm;
                harm.damage = attackDamage(type());
                harm.rect   = attackRect;
                harm.sfx    = hitSfx(type());
                t_context.avatar.harm(t_context, harm);
            }
            else if (util::bottom(monsterRect) > util::center(playerRect).y)
            {
                setupTask(t_context, MosquitoTask::Reset, MosquitoAnim::Flying);
            }
            else if (!monsterRect.findIntersection(m_flyBounds))
            {
                setupTask(t_context, MosquitoTask::Wander, MosquitoAnim::Flying);
            }
        }
    }

    const sf::FloatRect Mosquito::spottedRect(const Context & t_context) const
    {
        sf::FloatRect rect{ m_sprite.getGlobalBounds() };

        const float horizOffset{ rect.size.x * 1.5f };
        rect.position.x -= horizOffset;
        rect.size.x += (horizOffset * 2.0f);

        rect.size.y += t_context.layout.wholeRect().size.y;

        return rect;
    }

    float Mosquito::flyingSpeed(const MosquitoTask t_task) const
    {
        switch (t_task)
        {
            case MosquitoTask::Attack: return 200.0f;
            case MosquitoTask::Reset: return 150.0f;
            case MosquitoTask::Wander: return 100.0f;
            case MosquitoTask::Death:
            case MosquitoTask::Hurt:
            case MosquitoTask::Idle:
            default: return 0.0f;
        }
    }

    void Mosquito::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        if (t_context.layout.wholeRect().findIntersection(m_sprite.getGlobalBounds()))
        {
            t_target.draw(m_sprite, t_states);
        }
    }

    void Mosquito::move(const sf::Vector2f & t_move)
    {
        m_sprite.move(t_move);
        m_flyBounds.position += t_move;
        m_resetPosition += t_move;
    }

    bool Mosquito::avatarAttack(const Context & t_context, const AttackInfo & t_attackInfo)
    {
        if (!m_isAlive || (MosquitoAnim::Death == m_anim) || (MosquitoAnim::Hurt == m_anim))
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
            setupTask(t_context, MosquitoTask::Hurt, MosquitoAnim::Hurt);

            const std::string hurtSfxName{ hurtSfx(type()) };
            if (!hurtSfxName.empty())
            {
                t_context.sfx.play(hurtSfxName);
            }
        }
        else
        {
            setupTask(t_context, MosquitoTask::Death, MosquitoAnim::Death);

            const std::string deathSfxName{ deathSfx(type()) };
            if (!deathSfxName.empty())
            {
                t_context.sfx.play(deathSfxName);
            }
        }

        return true;
    }

    const Harm Mosquito::avatarCollide(const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        const sf::FloatRect attackRect{ attackCollisionRect() };

        if ((MosquitoTask::Attack == m_task) && t_avatarRect.findIntersection(attackRect))
        {
            harm.damage = attackDamage(type());
            harm.rect   = attackRect;
            harm.sfx    = hitSfx(type());
        }

        return harm;
    }

    const sf::FloatRect Mosquito::collisionRect() const
    {
        sf::FloatRect rect{ m_sprite.getGlobalBounds() };
        util::scaleRectInPlace(rect, { 0.4f, 0.5f });

        const float vertOffset{ rect.size.y * 0.5f };
        rect.position.y += vertOffset;
        rect.size.y -= vertOffset;

        rect.size.y -= (rect.size.y * 0.25f);
        return rect;
    }

    const sf::FloatRect Mosquito::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };
        rect.position += sf::Vector2f(2.0f, 2.0f);
        rect.size += sf::Vector2f(2.0f, 2.0f);
        return rect;
    }

    const sf::Vector2f Mosquito::randomResetPosition(const Context & t_context) const
    {
        sf::Vector2f pos{};

        pos.x = t_context.random.fromTo(m_flyBounds.position.x, util::right(m_flyBounds));

        pos.y = t_context.random.fromTo(
            m_flyBounds.position.y, (m_flyBounds.position.y + (m_flyBounds.size.y * 0.5f)));

        return pos;
    }

    void Mosquito::turnToFacePlayer(const Context & t_context)
    {
        turnToFacePosition(util::center(t_context.avatar.collisionRect()));
    }

    void Mosquito::turnAround()
    {
        m_sprite.scale({ -1.0f, 1.0f });

        if (m_isFacingRight)
        {
            m_sprite.move({ m_sprite.getGlobalBounds().size.x, 0.0f });
        }
        else
        {
            m_sprite.move({ -m_sprite.getGlobalBounds().size.x, 0.0f });
        }

        m_isFacingRight = !m_isFacingRight;
    }

    void Mosquito::turnToFacePosition(const sf::Vector2f & t_position)
    {
        const bool isPositionToTheRight{ t_position.x > util::center(collisionRect()).x };
        if (isPositionToTheRight != m_isFacingRight)
        {
            turnAround();
        }
    }

    float Mosquito::randomIdleDurationSec(const Context & t_context) const
    {
        return t_context.random.fromTo(1.5f, 4.0f);
    }

    void Mosquito::handleDying(const Context & t_context)
    {
        m_isAlive = false;

        const Experience_t xpBonus{ startingHealth(type()) };

        std::string message{ "+" };
        message += std::to_string(xpBonus);
        message += "xp";

        const sf::Vector2f messagePos{ util::center(m_sprite).x, m_sprite.getPosition().y };

        t_context.float_text.add(
            t_context, message, t_context.settings.off_white_color, messagePos);

        t_context.player.experienceAdjust(xpBonus);

        const auto & texturesVec{ MosquitoTextureManager::instance().textures() };
        auto & textures{ texturesVec.at(static_cast<std::size_t>(m_anim)) };
        m_sprite.setTexture(textures.at(textures.size() - 1));
    }

} // namespace bramblefore
