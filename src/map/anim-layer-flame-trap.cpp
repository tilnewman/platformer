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
#include "subsystem/texture-stats.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

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

        m_emitterUpTexture.loadFromFile(
            (t_context.settings.media_path / "image/anim/flame-trap-up.png").string());

        TextureStats::instance().process(m_emitterUpTexture);

        m_emitterDownTexture.loadFromFile(
            (t_context.settings.media_path / "image/anim/flame-trap-down.png").string());

        TextureStats::instance().process(m_emitterDownTexture);

        m_emitterLeftTexture.loadFromFile(
            (t_context.settings.media_path / "image/anim/flame-trap-left.png").string());

        TextureStats::instance().process(m_emitterLeftTexture);

        m_emitterRightTexture.loadFromFile(
            (t_context.settings.media_path / "image/anim/flame-trap-right.png").string());

        TextureStats::instance().process(m_emitterRightTexture);

        //

        m_flamesUpTexture.loadFromFile(
            (t_context.settings.media_path / "image/anim/flame-trap-flames-up.png").string());

        TextureStats::instance().process(m_flamesUpTexture);

        m_flamesDownTexture.loadFromFile(
            (t_context.settings.media_path / "image/anim/flame-trap-flames-down.png").string());

        TextureStats::instance().process(m_flamesDownTexture);

        m_flamesLeftTexture.loadFromFile(
            (t_context.settings.media_path / "image/anim/flame-trap-flames-left.png").string());

        TextureStats::instance().process(m_flamesLeftTexture);

        m_flamesRightTexture.loadFromFile(
            (t_context.settings.media_path / "image/anim/flame-trap-flames-right.png")
                .string());

        TextureStats::instance().process(m_flamesRightTexture);

        //

        m_emitterSprites.reserve(t_rectDirs.size());
        m_anims.reserve(t_rectDirs.size());

        for (const FlameTrapRectDir & rectDir : t_rectDirs)
        {
            sf::Sprite & emitterSprite{ m_emitterSprites.emplace_back() };
            emitterSprite.scale(m_emitterScale, m_emitterScale);

            FlameAnim & anim{ m_anims.emplace_back() };
            anim.direction                = rectDir.direction;
            anim.time_between_flaming_sec = t_context.random.fromTo(1.5f, 4.0f);

            if (rectDir.direction == TrapDirection::Up)
            {
                emitterSprite.setTexture(m_emitterUpTexture);

                emitterSprite.setPosition(
                    (util::center(rectDir.rect).x - (emitterSprite.getGlobalBounds().width * 0.5f)),
                    (util::bottom(rectDir.rect) - emitterSprite.getGlobalBounds().height));

                anim.sprite = sf::Sprite(m_flamesUpTexture, textureRect(anim.direction, 0));
                anim.sprite.scale(m_flameScale, m_flameScale);

                anim.sprite.setPosition(
                    util::center(rectDir.rect).x - (anim.sprite.getGlobalBounds().width * 0.5f),
                    (emitterSprite.getGlobalBounds().top - anim.sprite.getGlobalBounds().height));

                anim.coll_rect = anim.sprite.getGlobalBounds();
            }
            else if (rectDir.direction == TrapDirection::Down)
            {
                emitterSprite.setTexture(m_emitterDownTexture);

                emitterSprite.setPosition(
                    (util::center(rectDir.rect).x - (emitterSprite.getGlobalBounds().width * 0.5f)),
                    rectDir.rect.top);

                anim.sprite = sf::Sprite(m_flamesDownTexture, textureRect(anim.direction, 0));
                anim.sprite.scale(m_flameScale, m_flameScale);

                anim.sprite.setPosition(
                    util::center(rectDir.rect).x - (anim.sprite.getGlobalBounds().width * 0.5f),
                    util::bottom(emitterSprite.getGlobalBounds()));

                anim.coll_rect = anim.sprite.getGlobalBounds();
            }
            else if (rectDir.direction == TrapDirection::Left)
            {
                emitterSprite.setTexture(m_emitterLeftTexture);

                emitterSprite.setPosition(
                    (util::right(rectDir.rect) - emitterSprite.getGlobalBounds().width),
                    (util::center(rectDir.rect).y -
                     (emitterSprite.getGlobalBounds().height * 0.5f)));

                anim.sprite = sf::Sprite(m_flamesLeftTexture, textureRect(anim.direction, 0));
                anim.sprite.scale(m_flameScale, m_flameScale);

                anim.sprite.setPosition(
                    (emitterSprite.getGlobalBounds().left - anim.sprite.getGlobalBounds().width),
                    util::center(rectDir.rect).y - (anim.sprite.getGlobalBounds().height * 0.5f));

                anim.coll_rect = anim.sprite.getGlobalBounds();
            }
            else
            {
                emitterSprite.setTexture(m_emitterRightTexture);

                emitterSprite.setPosition(
                    rectDir.rect.left,
                    (util::center(rectDir.rect).y -
                     (emitterSprite.getGlobalBounds().height * 0.5f)));

                anim.sprite = sf::Sprite(m_flamesRightTexture, textureRect(anim.direction, 0));
                anim.sprite.scale(m_flameScale, m_flameScale);

                anim.sprite.setPosition(
                    util::right(emitterSprite.getGlobalBounds()),
                    util::center(rectDir.rect).y - (anim.sprite.getGlobalBounds().height * 0.5f));

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
            if (wholeScreenRect.intersects(sprite.getGlobalBounds()))
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

            if (wholeScreenRect.intersects(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void FlameTrapAnimationLayer::move(const Context &, const float t_amount)
    {
        for (sf::Sprite & sprite : m_emitterSprites)
        {
            sprite.move(t_amount, 0.0f);
        }

        for (FlameAnim & anim : m_anims)
        {
            anim.sprite.move(t_amount, 0.0f);
            anim.coll_rect.left += t_amount;
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

                    if (wholeScreenRect.intersects(anim.coll_rect))
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
            rect.width  = 32;
            rect.height = 112;
            rect.top    = 0;
            rect.left   = (static_cast<int>(frame) * rect.width);
        }
        else
        {
            rect.width  = 112;
            rect.height = 32;
            rect.top    = (static_cast<int>(frame) * rect.height);
            rect.left   = 0;
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

            if (t_avatarRect.intersects(anim.coll_rect))
            {
                harm.rect   = anim.coll_rect;
                harm.damage = 10;
                break;
            }
        }

        return harm;
    }

} // namespace bramblefore
