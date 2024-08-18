// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// goblin.hpp
//
#include "goblin.hpp"

#include "avatar.hpp"
#include "context.hpp"
#include "random.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

#include <filesystem>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    std::vector<sf::Texture> Goblin::m_textures;

    Goblin::Goblin(Context & context, const sf::FloatRect & region)
        : m_region(region)
        , m_anim(GoblinAnim::Idle)
        , m_animFrame(0)
        , m_sprite()
        , m_elapsedTimeSec(0.0f)
        , m_isFacingRight(context.random.boolean())
        , m_stateElapsedTimeSec(0.0f)
        , m_stateTimeUntilChangeSec(0.0f)
        , m_hasSpottedPlayer(false)
    {
        loadTextures(context.settings);

        setTexture(m_sprite, m_anim, m_animFrame);
        m_sprite.setScale(context.settings.monster_scale, context.settings.monster_scale);
        util::setOriginToCenter(m_sprite);

        m_sprite.setPosition(
            context.random.fromTo(region.left, util::right(region)),
            (util::bottom(region) - m_sprite.getGlobalBounds().height));

        m_sprite.move(0.0f, (0.8f * m_sprite.getGlobalBounds().height));

        if (!m_isFacingRight)
        {
            m_sprite.scale(-1.0f, 1.0f);
        }

        m_stateTimeUntilChangeSec = context.random.fromTo(1.0f, 6.0f);
    }

    void Goblin::update(Context & context, const float frameTimeSec)
    {
        if (animate(frameTimeSec))
        {
            if (!doesAnimLoop(m_anim))
            {
                if (m_hasSpottedPlayer)
                {
                    changeStateAfterSeeingPlayer(context);
                }
                else
                {
                    m_elapsedTimeSec = 0.0f;
                    m_anim           = GoblinAnim::Idle;
                }
            }
        }

        m_stateElapsedTimeSec += frameTimeSec;
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

        handleWalking(frameTimeSec);

        if (!m_hasSpottedPlayer && (m_region.intersects(context.avatar.collisionRect())))
        {
            m_hasSpottedPlayer    = true;
            m_elapsedTimeSec      = 0.0f;
            m_stateElapsedTimeSec = 0.0f;
            m_anim                = GoblinAnim::Idle;
            turnToFacePlayer(context);
        }
    }

    void Goblin::changeStateBeforeSeeingPlayer(Context & context)
    {
        m_elapsedTimeSec          = 0.0f;
        m_stateTimeUntilChangeSec = context.random.fromTo(1.0f, 6.0f);

        const int isNextActionWalking{ context.random.boolean() };
        if (isNextActionWalking)
        {
            m_anim = GoblinAnim::Walk;

            const bool willChangeDirection{ context.random.boolean() };
            if (willChangeDirection)
            {
                m_sprite.scale(-1.0f, 1.0f);
                m_isFacingRight = !m_isFacingRight;
            }
        }
        else
        {
            // in all other cases just turn around
            m_anim = GoblinAnim::Idle;
            m_sprite.scale(-1.0f, 1.0f);
            m_isFacingRight = !m_isFacingRight;
        }
    }

    void Goblin::changeStateAfterSeeingPlayer(Context & context)
    {
        turnToFacePlayer(context);

        m_elapsedTimeSec      = 0.0f;
        m_stateElapsedTimeSec = 0.0f;

        const int isNextActionWalking{ context.random.boolean() };
        if (isNextActionWalking)
        {
            m_anim                    = GoblinAnim::Walk;
            m_stateTimeUntilChangeSec = context.random.fromTo(1.0f, 2.0f);
        }
        else
        {
            // in all other cases just attack
            m_anim                    = GoblinAnim::Attack;
            m_stateTimeUntilChangeSec = 1.0f;
        }
    }

    bool Goblin::animate(const float frameTimeSec)
    {
        bool isAnimationFinished{ false };

        const float timePerSec{ timePerFrameSec(m_anim) };

        m_elapsedTimeSec += frameTimeSec;
        if (m_elapsedTimeSec > timePerSec)
        {
            m_elapsedTimeSec -= timePerSec;

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

    void Goblin::draw(
        const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        if (context.layout.wholeRect().intersects(m_sprite.getGlobalBounds()))
        {
            target.draw(m_sprite, states);
        }
    }

    void Goblin::move(const float amount)
    {
        m_sprite.move(amount, 0.0f);
        m_region.left += amount;
    }

    const sf::FloatRect Goblin::collisionRect() const
    {
        sf::FloatRect rect{ m_sprite.getGlobalBounds() };
        util::scaleRectInPlace(rect, 0.35f);
        return rect;
    }

    void Goblin::loadTextures(const Settings & settings)
    {
        if (!m_textures.empty())
        {
            return;
        }

        m_textures.reserve(static_cast<std::size_t>(GoblinAnim::Count));

        for (std::size_t animIndex(0); animIndex < static_cast<std::size_t>(GoblinAnim::Count);
             ++animIndex)
        {
            const GoblinAnim anim{ static_cast<GoblinAnim>(animIndex) };

            const std::filesystem::path path{ settings.media_path / "image/monster/goblin" /
                                              std::string(toString(anim)).append(".png") };

            sf::Texture & texture{ m_textures.emplace_back() };
            texture.loadFromFile(path.string());
            texture.setSmooth(true);
            TextureStats::instance().process(texture);
        }
    }

    std::size_t Goblin::frameCount(const GoblinAnim anim) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(anim)) };
        return static_cast<std::size_t>(texture.getSize().x / texture.getSize().y);
    }

    const sf::IntRect Goblin::textureRect(const GoblinAnim anim, const std::size_t frame) const
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

    void Goblin::setTexture(
        sf::Sprite & sprite, const GoblinAnim anim, const std::size_t frame) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(anim)) };
        sprite.setTexture(texture);
        sprite.setTextureRect(textureRect(anim, frame));
    }

    void Goblin::turnToFacePlayer(Context & context)
    {
        const bool isPlayerToTheRight{ util::center(context.avatar.collisionRect()).x >
                                       util::center(collisionRect()).x };

        if (isPlayerToTheRight != m_isFacingRight)
        {
            m_sprite.scale(-1.0f, 1.0f);
            m_isFacingRight = !m_isFacingRight;
        }
    }

    void Goblin::handleWalking(const float frameTimeSec)
    {
        if (GoblinAnim::Walk != m_anim)
        {
            return;
        }

        const float goblinWalkSpeed{ 40.0f };

        if (m_isFacingRight)
        {
            m_sprite.move((goblinWalkSpeed * frameTimeSec), 0.0f);
        }
        else
        {
            m_sprite.move(-(goblinWalkSpeed * frameTimeSec), 0.0f);
        }

        // if walked out of bounds simple turn around and keep walking
        const sf::FloatRect collRect{ collisionRect() };
        if (util::right(collRect) > util::right(m_region))
        {
            m_sprite.scale(-1.0f, 1.0f);
            m_isFacingRight = !m_isFacingRight;
            m_sprite.move((util::right(m_region) - util::right(collRect)), 0.0f);
        }
        else if (collRect.left < m_region.left)
        {
            m_sprite.scale(-1.0f, 1.0f);
            m_isFacingRight = !m_isFacingRight;
            m_sprite.move((m_region.left - collRect.left), 0.0f);
        }
    }

} // namespace platformer