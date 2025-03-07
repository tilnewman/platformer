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
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    FlameTrapRectDir::FlameTrapRectDir(
        const sf::FloatRect & t_rect, const TrapDirection t_direction)
        : rect{ t_rect }
        , direction{ t_direction }
    {}

    //

    FlameAnim::FlameAnim(
        const TrapDirection t_dir,
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const float t_timeBetweenFlamingSec,
        const float t_scale,
        const sf::FloatRect & t_screenRect,
        const sf::FloatRect & t_emitterRect)
        : elapsed_time_sec{ 0.0f }
        , time_between_flaming_sec{ t_timeBetweenFlamingSec }
        , time_between_frames_sec{ 0.15f }
        , frame_index{ 0 }
        , sprite{ t_texture, t_textureRect }
        , is_flaming{ false }
        , direction{ t_dir }
        , coll_rect{}
    {
        sprite.scale({ t_scale, t_scale });

        if (TrapDirection::Up == direction)
        {
            sprite.setPosition(
                { util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f),
                  (t_emitterRect.position.y - sprite.getGlobalBounds().size.y) });
        }
        else if (TrapDirection::Down == direction)
        {
            sprite.setPosition(
                { util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f),
                  util::bottom(t_emitterRect) });
        }
        else if (TrapDirection::Left == direction)
        {
            sprite.setPosition(
                { (t_emitterRect.position.x - sprite.getGlobalBounds().size.x),
                  util::center(t_screenRect).y - (sprite.getGlobalBounds().size.y * 0.5f) });
        }
        else
        {
            sprite.setPosition(
                { util::right(t_emitterRect),
                  util::center(t_screenRect).y - (sprite.getGlobalBounds().size.y * 0.5f) });
        }

        coll_rect = sprite.getGlobalBounds();
    }

    //

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

            if (rectDir.direction == TrapDirection::Up)
            {
                emitterSprite.setTexture(m_emitterUpTexture, true);

                emitterSprite.setPosition(
                    { (util::center(rectDir.rect).x -
                       (emitterSprite.getGlobalBounds().size.x * 0.5f)),
                      (util::bottom(rectDir.rect) - emitterSprite.getGlobalBounds().size.y) });
            }
            else if (rectDir.direction == TrapDirection::Down)
            {
                emitterSprite.setTexture(m_emitterDownTexture, true);

                emitterSprite.setPosition({ (util::center(rectDir.rect).x -
                                             (emitterSprite.getGlobalBounds().size.x * 0.5f)),
                                            rectDir.rect.position.y });
            }
            else if (rectDir.direction == TrapDirection::Left)
            {
                emitterSprite.setTexture(m_emitterLeftTexture, true);

                emitterSprite.setPosition(
                    { (util::right(rectDir.rect) - emitterSprite.getGlobalBounds().size.x),
                      (util::center(rectDir.rect).y -
                       (emitterSprite.getGlobalBounds().size.y * 0.5f)) });
            }
            else
            {
                emitterSprite.setTexture(m_emitterRightTexture, true);

                emitterSprite.setPosition({ rectDir.rect.position.x,
                                            (util::center(rectDir.rect).y -
                                             (emitterSprite.getGlobalBounds().size.y * 0.5f)) });
            }

            //

            if (rectDir.direction == TrapDirection::Up)
            {
                m_anims.emplace_back(
                    TrapDirection::Up,
                    m_flamesUpTexture,
                    textureRect(TrapDirection::Up, 0),
                    t_context.random.fromTo(1.5f, 4.0f),
                    m_flameScale,
                    rectDir.rect,
                    emitterSprite.getGlobalBounds());
            }
            else if (rectDir.direction == TrapDirection::Down)
            {
                m_anims.emplace_back(
                    TrapDirection::Down,
                    m_flamesDownTexture,
                    textureRect(TrapDirection::Down, 0),
                    t_context.random.fromTo(1.5f, 4.0f),
                    m_flameScale,
                    rectDir.rect,
                    emitterSprite.getGlobalBounds());
            }
            else if (rectDir.direction == TrapDirection::Left)
            {
                m_anims.emplace_back(
                    TrapDirection::Left,
                    m_flamesLeftTexture,
                    textureRect(TrapDirection::Left, 0),
                    t_context.random.fromTo(1.5f, 4.0f),
                    m_flameScale,
                    rectDir.rect,
                    emitterSprite.getGlobalBounds());
            }
            else
            {
                m_anims.emplace_back(
                    TrapDirection::Right,
                    m_flamesRightTexture,
                    textureRect(TrapDirection::Right, 0),
                    t_context.random.fromTo(1.5f, 4.0f),
                    m_flameScale,
                    rectDir.rect,
                    emitterSprite.getGlobalBounds());
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

    void FlameTrapAnimationLayer::dumpInfo() const
    {
        std::clog << "\tFlame Trap Animation Layer: x" << m_emitterSprites.size() << '\n';
    }

} // namespace bramblefore
