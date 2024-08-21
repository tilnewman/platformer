// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster.hpp
//
#include "monster.hpp"

#include "avatar.hpp"
#include "context.hpp"
#include "random.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "sound-player.hpp"
#include "texture-stats.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    Monster::Monster(Context & context, const MonsterSetupInfo & setupInfo)
        : m_imageDirName(setupInfo.image_dir)
        , m_region(setupInfo.region)
        , m_anim(MonsterAnim::Idle)
        , m_animFrame(0)
        , m_sprite()
        , m_elapsedTimeSec(0.0f)
        , m_isFacingRight(true)
        , m_stateElapsedTimeSec(0.0f)
        , m_stateTimeUntilChangeSec(0.0f)
        , m_hasSpottedPlayer(false)
        , m_health(setupInfo.health)
        , m_isAlive(true)
        , m_textures()
    {
        loadTextures(context.settings);
        initialSpriteSetup(context, setupInfo.image_height_ratio, setupInfo.image_scale);
    }

    void Monster::update(Context & context, const float frameTimeSec)
    {
        if (!m_isAlive)
        {
            return;
        }

        if (!m_hasSpottedPlayer && m_region.intersects(context.avatar.collisionRect()))
        {
            m_hasSpottedPlayer = true;
            turnToFacePlayer(context);
        }

        m_elapsedTimeSec += frameTimeSec;
        m_stateElapsedTimeSec += frameTimeSec;

        handleWalking(context, frameTimeSec);

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
                    changeStateAfterSeeingPlayer(context);
                }
                else
                {
                    changeStateBeforeSeeingPlayer(context);
                }
            }
        }
        else
        {
            if (MonsterAnim::Death == m_anim)
            {
                m_isAlive = false;
                setTexture(m_sprite, MonsterAnim::Death, (frameCount(MonsterAnim::Death) - 1));
            }
            else
            {
                if (m_hasSpottedPlayer)
                {
                    changeStateAfterSeeingPlayer(context);
                }
                else
                {
                    changeStateBeforeSeeingPlayer(context);
                }
            }
        }
    }

    void Monster::draw(
        const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        if (context.layout.wholeRect().intersects(m_sprite.getGlobalBounds()))
        {
            target.draw(m_sprite, states);
            util::drawRectangleShape(target, collisionRect(), false, sf::Color::Green);
            util::drawRectangleShape(target, attackCollisionRect(), false, sf::Color::Red);
        }
    }

    void Monster::move(const float amount)
    {
        m_sprite.move(amount, 0.0f);
        m_region.left += amount;
    }

    bool Monster::avatarAttack(Context & context, const AttackInfo & attackInfo)
    {
        if ((MonsterAnim::Death == m_anim) || (MonsterAnim::Hurt == m_anim))
        {
            return false;
        }

        if (!attackInfo.rect.intersects(collisionRect()))
        {
            return false;
        }

        m_health -= attackInfo.damage;

        if (m_health > 0)
        {
            m_anim = MonsterAnim::Hurt;
            resetAnimation();
            playHurtSfx(context);
        }
        else
        {
            m_anim = MonsterAnim::Death;
            resetAnimation();
            playDeathSfx(context);
        }

        return true;
    }

    bool Monster::animate()
    {
        bool isAnimationFinished{ false };

        const float timeBetweenFramesSec{ timePerFrameSec(m_anim) };

        if (m_elapsedTimeSec > timeBetweenFramesSec)
        {
            m_elapsedTimeSec -= timeBetweenFramesSec;

            ++m_animFrame;
            if (m_animFrame >= frameCount(m_anim))
            {
                m_animFrame         = 0;
                isAnimationFinished = true;
            }

            setTexture(m_sprite, m_anim, m_animFrame);
        }

        return isAnimationFinished;
    }

    float Monster::timePerFrameSec(const MonsterAnim anim) const
    {
        // clang-format off
        switch (anim)
        {
            case MonsterAnim::Attack:        { return 0.1f;   }
            case MonsterAnim::Death:         { return 0.175f; }
            case MonsterAnim::Hurt:          { return 0.15f;   }
            case MonsterAnim::Idle:          { return 0.15f;  }
            case MonsterAnim::Walk:          { return 0.1f;   }
            case MonsterAnim::Count: // intentional fallthrough
            default:                         { return 0.0f;   }
        }
        // clang-format on
    }

    void Monster::changeStateBeforeSeeingPlayer(Context & context)
    {
        if (MonsterAnim::Death == m_anim)
        {
            return;
        }

        m_elapsedTimeSec          = 0.0f;
        m_stateTimeUntilChangeSec = context.random.fromTo(1.0f, 6.0f);

        const int isNextActionWalking{ context.random.boolean() };
        if (isNextActionWalking)
        {
            m_anim = MonsterAnim::Walk;
            resetAnimation();
            const bool willChangeDirection{ context.random.boolean() };
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

    void Monster::changeStateAfterSeeingPlayer(Context & context)
    {
        if (!context.layout.wholeRect().intersects(collisionRect()))
        {
            return;
        }

        if (MonsterAnim::Death == m_anim)
        {
            return;
        }

        turnToFacePlayer(context);

        m_elapsedTimeSec      = 0.0f;
        m_stateElapsedTimeSec = 0.0f;

        const int isNextActionWalking{ context.random.boolean() };
        if (isNextActionWalking)
        {
            m_anim = MonsterAnim::Walk;
            resetAnimation();
            m_stateTimeUntilChangeSec = context.random.fromTo(1.0f, 2.0f);
        }
        else
        {
            // in all other cases just attack
            m_anim = MonsterAnim::Attack;
            resetAnimation();
            playAttackSfx(context);
        }
    }

    void Monster::handleWalking(Context & context, const float frameTimeSec)
    {
        if (MonsterAnim::Walk != m_anim)
        {
            return;
        }

        const float speed{ walkSpeed() };
        if (m_isFacingRight)
        {
            m_sprite.move((speed * frameTimeSec), 0.0f);
        }
        else
        {
            m_sprite.move(-(speed * frameTimeSec), 0.0f);
        }

        const sf::FloatRect avatarRect{ context.avatar.collisionRect() };
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

    void Monster::loadTextures(const Settings & settings)
    {
        m_textures.reserve(static_cast<std::size_t>(MonsterAnim::Count));

        for (std::size_t animIndex(0); animIndex < static_cast<std::size_t>(MonsterAnim::Count);
             ++animIndex)
        {
            const MonsterAnim anim{ static_cast<MonsterAnim>(animIndex) };

            const std::filesystem::path path{ settings.media_path / "image/monster" /
                                              m_imageDirName /
                                              std::string(toString(anim)).append(".png") };

            sf::Texture & texture{ m_textures.emplace_back() };
            texture.loadFromFile(path.string());
            texture.setSmooth(true);
            TextureStats::instance().process(texture);
        }
    }

    void Monster::initialSpriteSetup(
        Context & context, const float imageHeightOffsetRatio, const float imageScale)
    {
        setTexture(m_sprite, m_anim, m_animFrame);
        m_sprite.setScale(context.settings.monster_scale, context.settings.monster_scale);
        m_sprite.scale(imageScale, imageScale);
        util::setOriginToCenter(m_sprite);

        m_sprite.setPosition(
            context.random.fromTo(m_region.left, util::right(m_region)),
            (util::bottom(m_region) - m_sprite.getGlobalBounds().height));

        m_sprite.move(0.0f, (imageHeightOffsetRatio * m_sprite.getGlobalBounds().height));
    }

    void Monster::setTexture(
        sf::Sprite & sprite, const MonsterAnim anim, const std::size_t frame) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(anim)) };
        sprite.setTexture(texture);
        sprite.setTextureRect(textureRect(anim, frame));
    }

    std::size_t Monster::frameCount(const MonsterAnim anim) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(anim)) };
        return static_cast<std::size_t>(texture.getSize().x / texture.getSize().y);
    }

    const sf::IntRect Monster::textureRect(const MonsterAnim anim, const std::size_t frame) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(anim)) };

        sf::IntRect rect;

        if (frame >= frameCount(anim))
        {
            rect.left = 0;
        }
        else
        {
            rect.left = static_cast<int>(static_cast<std::size_t>(texture.getSize().y) * frame);
        }

        rect.top    = 0;
        rect.width  = static_cast<int>(texture.getSize().y);
        rect.height = rect.width;

        return rect;
    }

    void Monster::turnToFacePlayer(Context & context)
    {
        const bool isPlayerToTheRight{ util::center(context.avatar.collisionRect()).x >
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