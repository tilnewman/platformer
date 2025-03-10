// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-flaming-skull.cpp
//
#include "anim-layer-flaming-skull.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    FlamesAnim::FlamesAnim(
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const FlameDirection t_dir,
        const float t_timeBetweenFlamingSec,
        const float t_scale,
        const sf::FloatRect & t_screenRect)
        : elapsed_time_sec{ 0.0f }
        , time_between_flaming_sec{ t_timeBetweenFlamingSec }
        , time_between_frames_sec{ 0.15f }
        , frame_index{ 0 }
        , sprite{ t_texture, t_textureRect }
        , is_flaming{ false }
        , direction{ t_dir }
        , coll_rect{}
    {
        sprite.setScale({ t_scale, t_scale });

        const float collLengthRatio{ 0.5f };
        const float collWidthRatio{ 0.3f };

        if (FlameDirection::Up == direction)
        {
            sprite.setPosition(
                { (util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
                  (t_screenRect.position.y - (sprite.getGlobalBounds().size.y * 0.8f)) });

            coll_rect = util::scaleRectInPlaceCopy(
                sprite.getGlobalBounds(), { collWidthRatio, collLengthRatio });
        }
        else if (FlameDirection::Down == direction)
        {
            sprite.setPosition(
                { (util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
                  (util::bottom(t_screenRect) - (sprite.getGlobalBounds().size.y * 0.2f)) });

            coll_rect = util::scaleRectInPlaceCopy(
                sprite.getGlobalBounds(), { collWidthRatio, collLengthRatio });
        }
        else if (FlameDirection::Left == direction)
        {
            sprite.setPosition(
                { (t_screenRect.position.x - (sprite.getGlobalBounds().size.x * 0.8f)),
                  (util::center(t_screenRect).y - (sprite.getGlobalBounds().size.y * 0.5f)) });

            coll_rect = util::scaleRectInPlaceCopy(
                sprite.getGlobalBounds(), { collLengthRatio, collWidthRatio });
        }
        else if (FlameDirection::Right == direction)
        {
            sprite.setPosition(
                { (util::right(t_screenRect) - (sprite.getGlobalBounds().size.x * 0.2f)),
                  (util::center(t_screenRect).y - (sprite.getGlobalBounds().size.y * 0.5f)) });

            coll_rect = util::scaleRectInPlaceCopy(
                sprite.getGlobalBounds(), { collLengthRatio, collWidthRatio });
        }
    }

    //

    FlamingSkullAnimationLayer::FlamingSkullAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_skullBlockTexture{}
        , m_flamesUpTexture{}
        , m_flamesDownTexture{}
        , m_flamesLeftTexture{}
        , m_flamesRightTexture{}
        , m_skullBlockSprites{}
        , m_anims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        //

        util::TextureLoader::load(
            m_skullBlockTexture,
            (t_context.settings.media_path / "image/anim/skull-flames-block.png"));

        util::TextureLoader::load(
            m_flamesUpTexture, (t_context.settings.media_path / "image/anim/skull-flames-up.png"));

        util::TextureLoader::load(
            m_flamesDownTexture,
            (t_context.settings.media_path / "image/anim/skull-flames-down.png"));

        util::TextureLoader::load(
            m_flamesLeftTexture,
            (t_context.settings.media_path / "image/anim/skull-flames-left.png"));

        util::TextureLoader::load(
            m_flamesRightTexture,
            (t_context.settings.media_path / "image/anim/skull-flames-right.png"));

        //

        const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.8f) };

        const float spawnSecMin{ 1.5f };
        const float spawnSecMax{ 4.0f };

        m_anims.reserve(t_rects.size() * 4);
        m_skullBlockSprites.reserve(t_rects.size());
        for (const sf::FloatRect & rect : t_rects)
        {
            sf::Sprite & blockSprite{ m_skullBlockSprites.emplace_back(m_skullBlockTexture) };
            blockSprite.scale({ scale, scale });

            blockSprite.setPosition(
                { (util::center(rect).x - (blockSprite.getGlobalBounds().size.x * 0.5f)),
                  (util::center(rect).y - (blockSprite.getGlobalBounds().size.y * 0.5f)) });

            //

            m_anims.emplace_back(
                getTexture(FlameDirection::Up),
                textureRect(getTexture(FlameDirection::Up), 0),
                FlameDirection::Up,
                t_context.random.fromTo(spawnSecMin, spawnSecMax),
                scale,
                rect);

            m_anims.emplace_back(
                getTexture(FlameDirection::Down),
                textureRect(getTexture(FlameDirection::Down), 0),
                FlameDirection::Down,
                t_context.random.fromTo(spawnSecMin, spawnSecMax),
                scale,
                rect);

            m_anims.emplace_back(
                getTexture(FlameDirection::Left),
                textureRect(getTexture(FlameDirection::Left), 0),
                FlameDirection::Left,
                t_context.random.fromTo(spawnSecMin, spawnSecMax),
                scale,
                rect);

            m_anims.emplace_back(
                getTexture(FlameDirection::Right),
                textureRect(getTexture(FlameDirection::Right), 0),
                FlameDirection::Right,
                t_context.random.fromTo(spawnSecMin, spawnSecMax),
                scale,
                rect);
        }
    }

    FlamingSkullAnimationLayer::~FlamingSkullAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void FlamingSkullAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const FlamesAnim & anim : m_anims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }

        for (const sf::Sprite & sprite : m_skullBlockSprites)
        {
            if (wholeScreenRect.findIntersection(sprite.getGlobalBounds()))
            {
                t_target.draw(sprite, t_states);
            }
        }
    }

    void FlamingSkullAnimationLayer::move(const Context &, const float t_amount)
    {
        for (FlamesAnim & anim : m_anims)
        {
            anim.sprite.move({ t_amount, 0.0f });
            anim.coll_rect.position.x += t_amount;
        }

        for (sf::Sprite & sprite : m_skullBlockSprites)
        {
            sprite.move({ t_amount, 0.0f });
        }
    }

    void FlamingSkullAnimationLayer::update(Context & t_context, const float t_frameTimeSec)
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (FlamesAnim & anim : m_anims)
        {
            if (anim.is_flaming)
            {
                anim.elapsed_time_sec += t_frameTimeSec;
                if (anim.elapsed_time_sec > anim.time_between_frames_sec)
                {
                    anim.elapsed_time_sec -= anim.time_between_frames_sec;

                    const sf::Texture & texture{ getTexture(anim.direction) };

                    ++anim.frame_index;
                    if (anim.frame_index >= frameCount(texture))
                    {
                        anim.frame_index      = 0;
                        anim.elapsed_time_sec = 0.0f;
                        anim.is_flaming       = false;
                    }

                    anim.sprite.setTextureRect(textureRect(texture, anim.frame_index));
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

    std::size_t FlamingSkullAnimationLayer::frameCount(const sf::Texture & t_texture) const noexcept
    {
        if (t_texture.getSize().y > 0)
        {
            return static_cast<std::size_t>(t_texture.getSize().x / t_texture.getSize().y);
        }
        else
        {
            return 0;
        }
    }

    sf::IntRect FlamingSkullAnimationLayer::textureRect(
        const sf::Texture & t_texture, const std::size_t frame) const noexcept
    {
        sf::IntRect rect;
        rect.size.x     = static_cast<int>(t_texture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(frame) * rect.size.x);
        return rect;
    }

    Harm FlamingSkullAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        for (const FlamesAnim & anim : m_anims)
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

    const sf::Texture &
        FlamingSkullAnimationLayer::getTexture(const FlameDirection t_direction) const
    {
        if (FlameDirection::Up == t_direction)
        {
            return m_flamesUpTexture;
        }
        else if (FlameDirection::Down == t_direction)
        {
            return m_flamesDownTexture;
        }
        else if (FlameDirection::Left == t_direction)
        {
            return m_flamesLeftTexture;
        }
        else
        {
            return m_flamesRightTexture;
        }
    }

    void FlamingSkullAnimationLayer::dumpInfo() const
    {
        std::clog << "\tFlaming Skull Trap Animation Layer: x" << m_skullBlockSprites.size() << '\n';
    }

} // namespace bramblefore
