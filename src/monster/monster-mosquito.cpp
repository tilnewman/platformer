//
// monster-mosquito.cpp
//
#include "monster-mosquito.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "map/level.hpp"
#include "monster/monster-manager.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/filesystem-util.hpp"
#include "util/random.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

#include <filesystem>

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    Mosquito::Mosquito(const Context & t_context, const sf::FloatRect & t_region)
        : m_anim{ MosquitoAnim::Idle }
        , m_task{ MosquitoTask::Idle }
        , m_animTextures{}
        , m_sprite{ util::SfmlDefaults::instance().texture() }
        , m_elapsedAnimTimeSec{ 0.0f }
        , m_taskTimeRemainingSec{ randomIdleDurationSec(t_context) }
        , m_flyBounds{ t_region }
        , m_isFacingRight{ true }
        , m_resetPosition{}
        , m_frameIndex{ 0 }
    {}

    Mosquito::~Mosquito() {}

    void Mosquito::setup(const Context & t_context)
    {
        // load all textures
        // exact size to prevent re-allocations
        m_animTextures.reserve(static_cast<std::size_t>(MosquitoAnim::Count));

        for (std::size_t i{ 0 }; i < static_cast<std::size_t>(MosquitoAnim::Count); ++i)
        {
            const MosquitoAnim anim{ static_cast<MosquitoAnim>(i) };

            std::vector<sf::Texture> & textures = m_animTextures.emplace_back();

            const std::filesystem::path imageDirPath{ t_context.settings.media_path / "image" /
                                                      "monster" / "mosquito" / toString(anim) };

            const std::vector<std::filesystem::path> files{ util::findFilesInDirectory(
                imageDirPath, ".png") };

            M_CHECK(
                !files.empty(),
                "Failed to find any MosquitoAnim::" << toString(anim) << " images!");

            textures.reserve(files.size()); // exact size to prevent re-allocations

            for (const std::filesystem::path & path : files)
            {
                sf::Texture & texture{ textures.emplace_back() };
                util::TextureLoader::load(texture, path, true);
            }
        }

        // setup the sprite
        m_sprite.setTexture(
            m_animTextures.at(static_cast<std::size_t>(MosquitoAnim::Idle)).front(), true);

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

    void Mosquito::update(const Context & t_context, const float t_elapsedTimeSec)
    {
        // animate frame by frame
        m_elapsedAnimTimeSec += t_elapsedTimeSec;
        const float timeBetweenFrames{ toTimeBetweenFrames(m_anim) };
        if (m_elapsedAnimTimeSec > timeBetweenFrames)
        {
            m_elapsedAnimTimeSec -= timeBetweenFrames;

            auto & textures{ m_animTextures.at(static_cast<std::size_t>(m_anim)) };

            if (++m_frameIndex >= textures.size())
            {
                m_frameIndex = 0;
            }

            m_sprite.setTexture(textures.at(m_frameIndex));
        }

        /*
        // spot the player if not already
        if (!m_hasSpottedPlayer)
        {
            if (t_context.avatar.collisionRect().findIntersection(m_flyBounds))
            {
                m_hasSpottedPlayer     = true;
                m_frameIndex           = 0;
                m_elapsedAnimTimeSec   = 0.0f;
                m_taskTimeRemainingSec = randomIdleDurationSec(t_context);
                m_anim                 = MosquitoAnim::Flying;
                m_resetPosition        = randomResetPosition(t_context);
                return;
            }
        }
        */

        // change tasks if time to do so
        m_taskTimeRemainingSec -= t_elapsedTimeSec;
        if (m_taskTimeRemainingSec < 0.0f)
        {
            m_taskTimeRemainingSec = 0.0f;

            if (MosquitoTask::Idle == m_task)
            {
                if (t_context.random.fromTo(1, 10) <= 3)
                {
                    m_task                 = MosquitoTask::Idle;
                    m_anim                 = MosquitoAnim::Idle;
                    m_frameIndex           = 0;
                    m_taskTimeRemainingSec = randomIdleDurationSec(t_context);
                    turnAround();
                    return;
                }
                else
                {
                    m_task          = MosquitoTask::Wander;
                    m_anim          = MosquitoAnim::Flying;
                    m_frameIndex    = 0;
                    m_resetPosition = randomResetPosition(t_context);
                    turnToFacePosition(m_resetPosition);
                    return;
                }
            }
        }

        // wander
        if (MosquitoTask::Wander == m_task)
        {
            if (util::distance(m_resetPosition, util::center(m_sprite.getGlobalBounds())) <
                collisionRect().size.x)
            {
                m_task                 = MosquitoTask::Idle;
                m_anim                 = MosquitoAnim::Idle;
                m_frameIndex           = 0;
                m_taskTimeRemainingSec = randomIdleDurationSec(t_context);
            }
            else
            {
                const sf::Vector2f diffVec{ util::normalize(
                    m_resetPosition - util::center(m_sprite.getGlobalBounds())) };

                m_sprite.move(diffVec * (100.0f * t_elapsedTimeSec));
            }
        }
    }

    void Mosquito::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeRect{ t_context.layout.wholeRect() };
        if (wholeRect.findIntersection(m_sprite.getGlobalBounds()))
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

    bool Mosquito::avatarAttack(const Context &, const AttackInfo &)
    {
        // TODO
        return false;
    }

    const Harm Mosquito::avatarCollide(const sf::FloatRect &)
    {
        // TODO
        return Harm();
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
        rect.position -= sf::Vector2f(2.0f, 2.0f);
        rect.size += sf::Vector2f(4.0f, 4.0f);
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

} // namespace bramblefore
