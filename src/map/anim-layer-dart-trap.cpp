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

    DartTrapAnimationLayer::DartTrapAnimationLayer(
        Context & t_context, const std::vector<DartRectDir> & t_rectDirs)
        : m_shooterTexture{}
        , m_dartTexture{}
        , m_shooters{}
        , m_darts{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        //

        util::TextureLoader::load(
            m_shooterTexture, (t_context.settings.media_path / "image/anim/dart-shooter.png"));

        util::TextureLoader::load(
            m_dartTexture, (t_context.settings.media_path / "image/anim/dart.png"));

        //

        const float shooterScale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.5f) };

        m_shooters.reserve(t_rectDirs.size());
        for (const DartRectDir & rectDir : t_rectDirs)
        {
            Shooter & shooter{ m_shooters.emplace_back(
                rectDir.is_left,
                m_shooterTexture,
                rectDir.rect,
                t_context.random.fromTo(3.5f, 6.5f)) };

            shooter.sprite.setScale({ shooterScale, shooterScale });

            if (rectDir.is_left)
            {
                shooter.sprite.scale({ -1.0f, 1.0f });

                shooter.sprite.setPosition({ (util::right(rectDir.rect) +
                                              (shooter.sprite.getGlobalBounds().size.x * 0.5f)),
                                             (util::center(rectDir.rect).y -
                                              (shooter.sprite.getGlobalBounds().size.y * 0.5f)) });
            }
            else
            {
                shooter.sprite.setPosition(
                    { (rectDir.rect.position.x - (shooter.sprite.getGlobalBounds().size.x * 0.5f)),
                      (util::center(rectDir.rect).y -
                       (shooter.sprite.getGlobalBounds().size.y * 0.5f)) });
            }
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

                Dart & dart{ m_darts.emplace_back(shooter.is_left, m_dartTexture, shooter.region) };

                const float dartScale{ t_context.layout.calScaleBasedOnResolution(
                    t_context, 1.1f) };

                dart.sprite.setScale({ dartScale, dartScale });

                if (dart.is_left)
                {
                    dart.sprite.scale({ -1.0f, 1.0f });

                    dart.sprite.setPosition({ (shooter.sprite.getGlobalBounds().position.x +
                                               dart.sprite.getGlobalBounds().size.x),
                                              (util::center(dart.region).y -
                                               (dart.sprite.getGlobalBounds().size.y * 0.5f)) });
                }
                else
                {
                    dart.sprite.setPosition({ (util::right(shooter.sprite.getGlobalBounds()) -
                                               dart.sprite.getGlobalBounds().size.x),
                                              (util::center(dart.region).y -
                                               (dart.sprite.getGlobalBounds().size.y * 0.5f)) });
                }

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
