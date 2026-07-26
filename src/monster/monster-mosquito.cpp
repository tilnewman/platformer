//
// monster-mosquito.cpp
//
#include "monster-mosquito.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "map/level.hpp"
#include "monster/monster-manager.hpp"
#include "subsystem/context.hpp"
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
        , m_speedMult{ t_context.random.fromTo(0.8f, 1.2f) }
        , m_isAlive{ true }
        , m_resetDistance{ 0.0f }
        , m_debugText{ util::SfmlDefaults::instance().font() }
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

        //
        m_debugText = t_context.font.makeText(Font::General, FontSize::Small, "", sf::Color::Red);
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

        if ((MosquitoTask::Attack == t_task) || (MosquitoTask::Reset == t_task))
        {
            m_hasSpottedPlayer = true;
        }

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

            auto & textures{ m_animTextures.at(static_cast<std::size_t>(m_anim)) };

            if (++m_frameIndex >= textures.size())
            {
                m_frameIndex = 0;

                if (MosquitoAnim::AttackPrep == m_anim)
                {
                    m_anim = MosquitoAnim::AttackCycle;
                }
                else if (MosquitoAnim::Death == m_anim)
                {
                    m_isAlive = false;
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
        if (!m_hasSpottedPlayer && t_context.avatar.collisionRect().findIntersection(m_flyBounds))
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
            t_context.sfx.play("pickup");
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

        if (MosquitoTask::Wander == m_task)
        {
            const sf::Vector2f mosquitoPos{ util::center(m_sprite.getGlobalBounds()) };

            if (util::distance(m_resetPosition, mosquitoPos) < collisionRect().size.x)
            {
                setupTask(t_context, MosquitoTask::Idle, MosquitoAnim::Idle);
            }
            else
            {
                const sf::Vector2f diffVec{ util::normalize(m_resetPosition - mosquitoPos) };
                const float wanderSpeed{ 100.0f };
                m_sprite.move(diffVec * (wanderSpeed * m_speedMult * t_elapsedTimeSec));
            }
        }
        else if (MosquitoTask::Reset == m_task)
        {
            const sf::Vector2f playerPos{ util::center(t_context.avatar.collisionRect()) };
            const sf::Vector2f mosquitoPos{ util::center(m_sprite.getGlobalBounds()) };

            if ((util::distance(playerPos, mosquitoPos) > m_resetDistance) ||
                (util::distance(m_resetPosition, mosquitoPos) < 5.0f))
            {
                setupTask(t_context, MosquitoTask::Attack, MosquitoAnim::AttackPrep);
            }
            else
            {
                const sf::Vector2f diffVec{ util::normalize(m_resetPosition - mosquitoPos) };
                const float resetSpeed{ 150.0f };
                m_sprite.move(diffVec * (resetSpeed * m_speedMult * t_elapsedTimeSec));
            }
        }
        else if (MosquitoTask::Attack == m_task)
        {
            turnToFacePlayer(t_context);

            const sf::FloatRect playerRect{ t_context.avatar.collisionRect() };
            const sf::Vector2f playerPos{ util::center(playerRect) };
            const sf::Vector2f mosquitoPos{ util::center(m_sprite.getGlobalBounds()) };
            const sf::Vector2f diffVec{ util::normalize(playerPos - mosquitoPos) };
            const float attackSpeed{ 200.0f };
            m_sprite.move(diffVec * (attackSpeed * m_speedMult * t_elapsedTimeSec));

            if (collisionRect().findIntersection(playerRect))
            {
                setupTask(t_context, MosquitoTask::Reset, MosquitoAnim::Flying);
                t_context.sfx.play("ui-select-thock-slide");
            }
        }
    }

    void Mosquito::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        if (!m_isAlive)
        {
            return;
        }

        const sf::FloatRect wholeRect{ t_context.layout.wholeRect() };
        if (wholeRect.findIntersection(m_sprite.getGlobalBounds()))
        {
            t_target.draw(m_sprite, t_states);

            std::string str{ toString(m_task) };
            str += ", ";
            str += toString(m_anim);
            str += ", ";
            str += std::to_string(m_frameIndex);
            //
            m_debugText.setString(str);
            util::setOriginToPosition(m_debugText);
            m_debugText.setPosition({ util::right(collisionRect()), collisionRect().position.y });
            t_target.draw(m_debugText, t_states);
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
