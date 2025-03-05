// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-flame-trap.cpp
//
#include "anim-layer-flame-trap.hpp"

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

    FlameTrapAnimationLayer::FlameTrapAnimationLayer(
        Context & t_context, const std::vector<FlameTrapRectDir> & t_rectDirs)
        : m_emitterScale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.8f) }
        , m_emitterUpTexture{}
        , m_emitterDownTexture{}
        , m_emitterLeftTexture{}
        , m_emitterRightTexture{}
        , m_flameScale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.25f) }
        , m_flamesUpTexture{}
        , m_flamesDownTexture{}
        , m_flamesLeftTexture{}
        , m_flamesRightTexture{}
        , m_emitterSprites{}
        , m_anims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        //

        util::TextureLoader::load(
            m_emitterUpTexture, (t_context.settings.media_path / "image/anim/flame-trap-up.png"));

        util::TextureLoader::load(
            m_emitterDownTexture,
            (t_context.settings.media_path / "image/anim/flame-trap-down.png"));

        util::TextureLoader::load(
            m_emitterLeftTexture,
            (t_context.settings.media_path / "image/anim/flame-trap-left.png"));

        util::TextureLoader::load(
            m_emitterRightTexture,
            (t_context.settings.media_path / "image/anim/flame-trap-right.png"));

        //

        util::TextureLoader::load(
            m_flamesUpTexture,
            (t_context.settings.media_path / "image/anim/flame-trap-flames-up.png"));

        util::TextureLoader::load(
            m_flamesDownTexture,
            (t_context.settings.media_path / "image/anim/flame-trap-flames-down.png"));

        util::TextureLoader::load(
            m_flamesLeftTexture,
            (t_context.settings.media_path / "image/anim/flame-trap-flames-left.png"));

        util::TextureLoader::load(
            m_flamesRightTexture,
            (t_context.settings.media_path / "image/anim/flame-trap-flames-right.png"));

        //

        m_anims.reserve(t_rectDirs.size());
        m_emitterSprites.reserve(t_rectDirs.size());
        
        for (const FlameTrapRectDir & rectDir : t_rectDirs)
        {
            sf::Sprite & emitterSprite{ m_emitterSprites.emplace_back(
                util::SfmlDefaults::instance().texture()) };

            emitterSprite.scale({ m_emitterScale, m_emitterScale });

            FlameAnim & anim{ m_anims.emplace_back() };
            anim.direction                = rectDir.direction;
            anim.time_between_flaming_sec = t_context.random.fromTo(1.5f, 4.0f);

            if (rectDir.direction == TrapDirection::Up)
            {
                emitterSprite.setTexture(m_emitterUpTexture, true);

                emitterSprite.setPosition(
                    { (util::center(rectDir.rect).x -
                       (emitterSprite.getGlobalBounds().size.x * 0.5f)),
                      (util::bottom(rectDir.rect) - emitterSprite.getGlobalBounds().size.y) });

                anim.sprite = sf::Sprite(m_flamesUpTexture, textureRect(anim.direction, 0));
                anim.sprite.scale({ m_flameScale, m_flameScale });

                anim.sprite.setPosition(
                    { util::center(rectDir.rect).x - (anim.sprite.getGlobalBounds().size.x * 0.5f),
                      (emitterSprite.getGlobalBounds().position.y -
                       anim.sprite.getGlobalBounds().size.y) });

                anim.coll_rect = anim.sprite.getGlobalBounds();
            }
            else if (rectDir.direction == TrapDirection::Down)
            {
                emitterSprite.setTexture(m_emitterDownTexture, true);

                emitterSprite.setPosition({ (util::center(rectDir.rect).x -
                                             (emitterSprite.getGlobalBounds().size.x * 0.5f)),
                                            rectDir.rect.position.y });

                anim.sprite = sf::Sprite(m_flamesDownTexture, textureRect(anim.direction, 0));
                anim.sprite.scale({ m_flameScale, m_flameScale });

                anim.sprite.setPosition(
                    { util::center(rectDir.rect).x - (anim.sprite.getGlobalBounds().size.x * 0.5f),
                      util::bottom(emitterSprite.getGlobalBounds()) });

                anim.coll_rect = anim.sprite.getGlobalBounds();
            }
            else if (rectDir.direction == TrapDirection::Left)
            {
                emitterSprite.setTexture(m_emitterLeftTexture, true);

                emitterSprite.setPosition(
                    { (util::right(rectDir.rect) - emitterSprite.getGlobalBounds().size.x),
                      (util::center(rectDir.rect).y -
                       (emitterSprite.getGlobalBounds().size.y * 0.5f)) });

                anim.sprite = sf::Sprite(m_flamesLeftTexture, textureRect(anim.direction, 0));
                anim.sprite.scale({ m_flameScale, m_flameScale });

                anim.sprite.setPosition({ (emitterSprite.getGlobalBounds().position.x -
                                           anim.sprite.getGlobalBounds().size.x),
                                          util::center(rectDir.rect).y -
                                              (anim.sprite.getGlobalBounds().size.y * 0.5f) });

                anim.coll_rect = anim.sprite.getGlobalBounds();
            }
            else
            {
                emitterSprite.setTexture(m_emitterRightTexture, true);

                emitterSprite.setPosition({ rectDir.rect.position.x,
                                            (util::center(rectDir.rect).y -
                                             (emitterSprite.getGlobalBounds().size.y * 0.5f)) });

                anim.sprite = sf::Sprite(m_flamesRightTexture, textureRect(anim.direction, 0));
                anim.sprite.scale({ m_flameScale, m_flameScale });

                anim.sprite.setPosition({ util::right(emitterSprite.getGlobalBounds()),
                                          util::center(rectDir.rect).y -
                                              (anim.sprite.getGlobalBounds().size.y * 0.5f) });

                anim.coll_rect = anim.sprite.getGlobalBounds();
            }
        }
    }

    FlameTrapAnimationLayer::~FlameTrapAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void FlameTrapAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const sf::Sprite & sprite : m_emitterSprites)
        {
            if (wholeScreenRect.findIntersection(sprite.getGlobalBounds()))
            {
                t_target.draw(sprite, t_states);
            }
        }

        for (const FlameAnim & anim : m_anims)
        {
            if (!anim.is_flaming)
            {
                continue;
            }

            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void FlameTrapAnimationLayer::move(const Context &, const float t_amount)
    {
        for (sf::Sprite & sprite : m_emitterSprites)
        {
            sprite.move({ t_amount, 0.0f });
        }

        for (FlameAnim & anim : m_anims)
        {
            anim.sprite.move({ t_amount, 0.0f });
            anim.coll_rect.position.x += t_amount;
        }
    }

    void FlameTrapAnimationLayer::update(Context & t_context, const float t_frameTimeSec)
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (FlameAnim & anim : m_anims)
        {
            if (anim.is_flaming)
            {
                anim.elapsed_time_sec += t_frameTimeSec;
                if (anim.elapsed_time_sec > anim.time_between_frames_sec)
                {
                    anim.elapsed_time_sec -= anim.time_between_frames_sec;

                    ++anim.frame_index;
                    if (anim.frame_index >= 9)
                    {
                        anim.frame_index      = 0;
                        anim.elapsed_time_sec = 0.0f;
                        anim.is_flaming       = false;
                    }

                    anim.sprite.setTextureRect(textureRect(anim.direction, anim.frame_index));
                }
            }
            else
            {
                anim.elapsed_time_sec += t_frameTimeSec;
                if (anim.elapsed_time_sec > anim.time_between_flaming_sec)
                {
                    anim.elapsed_time_sec = 0.0f;
                    anim.is_flaming       = true;

                    if (wholeScreenRect.findIntersection(anim.coll_rect))
                    {
                        t_context.sfx.play("fire-puff");
                    }
                }
            }
        }
    }

    sf::IntRect FlameTrapAnimationLayer::textureRect(
        const TrapDirection t_direction, const std::size_t frame) const noexcept
    {
        sf::IntRect rect;

        if ((t_direction == TrapDirection::Up) || (t_direction == TrapDirection::Down))
        {
            rect.size.x     = 32;
            rect.size.y     = 112;
            rect.position.y = 0;
            rect.position.x = (static_cast<int>(frame) * rect.size.x);
        }
        else
        {
            rect.size.x     = 112;
            rect.size.y     = 32;
            rect.position.y = (static_cast<int>(frame) * rect.size.y);
            rect.position.x = 0;
        }

        return rect;
    }

    Harm FlameTrapAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        for (const FlameAnim & anim : m_anims)
        {
            if (!anim.is_flaming)
            {
                continue;
            }

            if (t_avatarRect.findIntersection(anim.coll_rect))
            {
                harm.rect   = anim.coll_rect;
                harm.damage = 10;
                break;
            }
        }

        return harm;
    }

} // namespace bramblefore
