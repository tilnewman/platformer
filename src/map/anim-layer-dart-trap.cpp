// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-dart-trap.cpp
//
#include "anim-layer-dart-trap.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "map/level.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    DartRectDir::DartRectDir(const bool t_isLeft, const sf::FloatRect & t_rect)
        : is_left{ t_isLeft }
        , rect{ t_rect }
    {}

    //

    Shooter::Shooter(
        const bool t_isLeft,
        const sf::Texture & t_texture,
        const sf::FloatRect & t_rect,
        const float t_timeBetweenShotsSec,
        const float t_scale)
        : is_left{ t_isLeft }
        , sprite{ t_texture }
        , region{ t_rect }
        , time_between_shots_sec{ t_timeBetweenShotsSec }
        , elapsed_time_sec{ 0.0 }
    {
        sprite.setScale({ t_scale, t_scale });

        if (is_left)
        {
            sprite.scale({ -1.0f, 1.0f });

            sprite.setPosition(
                { (util::right(t_rect) + (sprite.getGlobalBounds().size.x * 0.5f)),
                  (util::center(t_rect).y - (sprite.getGlobalBounds().size.y * 0.5f)) });
        }
        else
        {
            sprite.setPosition(
                { (t_rect.position.x - (sprite.getGlobalBounds().size.x * 0.5f)),
                  (util::center(t_rect).y - (sprite.getGlobalBounds().size.y * 0.5f)) });
        }
    }

    //

    Dart::Dart(
        const bool t_isLeft,
        const sf::Texture & t_texture,
        const sf::FloatRect & t_rect,
        const float t_scale,
        const sf::FloatRect & t_shooterGlobalBounds)
        : is_alive{ true }
        , is_left{ t_isLeft }
        , sprite{ t_texture }
        , region{ t_rect }
    {
        sprite.setScale({ t_scale, t_scale });

        if (is_left)
        {
            sprite.scale({ -1.0f, 1.0f });

            sprite.setPosition(
                { (t_shooterGlobalBounds.position.x + sprite.getGlobalBounds().size.x),
                  (util::center(region).y - (sprite.getGlobalBounds().size.y * 0.5f)) });
        }
        else
        {
            sprite.setPosition(
                { (util::right(t_shooterGlobalBounds) - sprite.getGlobalBounds().size.x),
                  (util::center(region).y - (sprite.getGlobalBounds().size.y * 0.5f)) });
        }
    }

    //

    DartTrapAnimationLayer::DartTrapAnimationLayer(
        Context & t_context, const std::vector<DartRectDir> & t_rectDirs)
        : m_shooterTexture{}
        , m_dartTexture{}
        , m_shooters{}
        , m_darts{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        util::TextureLoader::load(
            m_shooterTexture, (t_context.settings.media_path / "image/anim/dart-shooter.png"));

        util::TextureLoader::load(
            m_dartTexture, (t_context.settings.media_path / "image/anim/dart.png"));

        m_shooters.reserve(t_rectDirs.size());
        for (const DartRectDir & rectDir : t_rectDirs)
        {
            m_shooters.emplace_back(
                rectDir.is_left,
                m_shooterTexture,
                rectDir.rect,
                t_context.random.fromTo(3.5f, 6.5f),
                t_context.layout.calScaleBasedOnResolution(t_context, 1.5f));
        }
    }

    DartTrapAnimationLayer::~DartTrapAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void DartTrapAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const Dart & dart : m_darts)
        {
            if (wholeScreenRect.findIntersection(dart.sprite.getGlobalBounds()))
            {
                t_target.draw(dart.sprite, t_states);
            }
        }

        for (const Shooter & shooter : m_shooters)
        {
            if (wholeScreenRect.findIntersection(shooter.sprite.getGlobalBounds()))
            {
                t_target.draw(shooter.sprite, t_states);
            }
        }
    }

    void DartTrapAnimationLayer::move(const Context &, const float t_amount)
    {
        for (Dart & dart : m_darts)
        {
            dart.sprite.move({ t_amount, 0.0f });
            dart.region.position.x += t_amount;
        }

        for (Shooter & shooter : m_shooters)
        {
            shooter.sprite.move({ t_amount, 0.0f });
            shooter.region.position.x += t_amount;
        }
    }

    void DartTrapAnimationLayer::update(Context & t_context, const float t_frameTimeSec)
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (Shooter & shooter : m_shooters)
        {
            shooter.elapsed_time_sec += t_frameTimeSec;
            if (shooter.elapsed_time_sec > shooter.time_between_shots_sec)
            {
                shooter.elapsed_time_sec -= shooter.time_between_shots_sec;

                m_darts.emplace_back(
                    shooter.is_left,
                    m_dartTexture,
                    shooter.region,
                    t_context.layout.calScaleBasedOnResolution(t_context, 1.1f),
                    shooter.sprite.getGlobalBounds());

                if (wholeScreenRect.findIntersection(shooter.sprite.getGlobalBounds()))
                {
                    t_context.sfx.play("dart-shoot");
                }
            }
        }

        const float moveAmount{ 750.0f * t_frameTimeSec };

        bool didAnyDartsHitTheWall{ false };
        for (Dart & dart : m_darts)
        {
            if (dart.is_left)
            {
                dart.sprite.move({ -moveAmount, 0.0f });

                if (dart.sprite.getGlobalBounds().position.x < dart.region.position.x)
                {
                    if (wholeScreenRect.findIntersection(dart.sprite.getGlobalBounds()))
                    {
                        t_context.sfx.play("dart-hit");
                    }

                    didAnyDartsHitTheWall = true;
                    dart.is_alive         = false;
                }
            }
            else
            {
                dart.sprite.move({ moveAmount, 0.0f });

                if (util::right(dart.sprite.getGlobalBounds()) > util::right(dart.region))
                {
                    if (wholeScreenRect.findIntersection(dart.sprite.getGlobalBounds()))
                    {
                        t_context.sfx.play("dart-hit");
                    }

                    didAnyDartsHitTheWall = true;
                    dart.is_alive         = false;
                }
            }
        }

        if (didAnyDartsHitTheWall)
        {
            std::erase_if(m_darts, [](const Dart & dart) { return !dart.is_alive; });
        }
    }

    Harm DartTrapAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        bool didAnyDartsHitThePlayer{ false };
        for (Dart & dart : m_darts)
        {
            const sf::FloatRect dartRect{ dart.sprite.getGlobalBounds() };
            if (t_avatarRect.findIntersection(dartRect))
            {
                harm.rect   = dartRect;
                harm.damage = 10;
                harm.sfx    = "dart-hit";

                didAnyDartsHitThePlayer = true;
                dart.is_alive           = false;

                break;
            }
        }

        if (didAnyDartsHitThePlayer)
        {
            std::erase_if(m_darts, [](const Dart & dart) { return !dart.is_alive; });
        }

        return harm;
    }

} // namespace bramblefore
