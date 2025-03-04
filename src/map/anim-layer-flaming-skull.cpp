// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-flaming-skull.cpp
//
#include "anim-layer-flaming-skull.hpp"

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

    FlamingSkullAnimationLayer::FlamingSkullAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_scale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.8f) }
        , m_skullBlockTexture{}
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

        for (const sf::FloatRect & rect : t_rects)
        {
            sf::Sprite & blockSprite{ m_skullBlockSprites.emplace_back(m_skullBlockTexture) };
            blockSprite.scale({ m_scale, m_scale });

            blockSprite.setPosition(
                { (util::center(rect).x - (blockSprite.getGlobalBounds().size.x * 0.5f)),
                  (util::center(rect).y - (blockSprite.getGlobalBounds().size.y * 0.5f)) });

            const float spawnSecMin{ 1.5f };
            const float spawnSecMax{ 4.0f };

            const float collLengthRatio{ 0.5f };
            const float collWidthRatio{ 0.3f };

            //

            FlamesAnim & upAnim{ m_anims.emplace_back(
                getTexture(FlameDirection::Up), FlameDirection::Up) };

            upAnim.time_between_flaming = t_context.random.fromTo(spawnSecMin, spawnSecMax);
            upAnim.sprite.setTextureRect(textureRect(getTexture(upAnim.direction), 0));
            upAnim.sprite.setScale({ m_scale, m_scale });

            upAnim.sprite.setPosition(
                { (util::center(rect).x - (upAnim.sprite.getGlobalBounds().size.x * 0.5f)),
                  (rect.position.y - (upAnim.sprite.getGlobalBounds().size.y * 0.8f)) });

            upAnim.coll_rect = util::scaleRectInPlaceCopy(
                upAnim.sprite.getGlobalBounds(), { collWidthRatio, collLengthRatio });

            //

            FlamesAnim & downAnim{ m_anims.emplace_back(
                getTexture(FlameDirection::Down), FlameDirection::Down) };

            downAnim.time_between_flaming = t_context.random.fromTo(spawnSecMin, spawnSecMax);
            downAnim.sprite.setTextureRect(textureRect(getTexture(downAnim.direction), 0));
            downAnim.sprite.setScale({ m_scale, m_scale });

            downAnim.sprite.setPosition(
                { (util::center(rect).x - (downAnim.sprite.getGlobalBounds().size.x * 0.5f)),
                  (util::bottom(rect) - (downAnim.sprite.getGlobalBounds().size.y * 0.2f)) });

            downAnim.coll_rect = util::scaleRectInPlaceCopy(
                downAnim.sprite.getGlobalBounds(), { collWidthRatio, collLengthRatio });

            //

            FlamesAnim & leftAnim{ m_anims.emplace_back(
                getTexture(FlameDirection::Left), FlameDirection::Left) };

            leftAnim.time_between_flaming = t_context.random.fromTo(spawnSecMin, spawnSecMax);
            leftAnim.sprite.setTextureRect(textureRect(getTexture(leftAnim.direction), 0));
            leftAnim.sprite.setScale({ m_scale, m_scale });

            leftAnim.sprite.setPosition(
                { (rect.position.x - (leftAnim.sprite.getGlobalBounds().size.x * 0.8f)),
                  (util::center(rect).y - (leftAnim.sprite.getGlobalBounds().size.y * 0.5f)) });

            leftAnim.coll_rect = util::scaleRectInPlaceCopy(
                leftAnim.sprite.getGlobalBounds(), { collLengthRatio, collWidthRatio });

            //

            FlamesAnim & rightAnim{ m_anims.emplace_back(
                getTexture(FlameDirection::Right), FlameDirection::Right) };

            rightAnim.time_between_flaming = t_context.random.fromTo(spawnSecMin, spawnSecMax);
            rightAnim.sprite.setTextureRect(textureRect(getTexture(rightAnim.direction), 0));
            rightAnim.sprite.setScale({ m_scale, m_scale });

            rightAnim.sprite.setPosition(
                { (util::right(rect) - (rightAnim.sprite.getGlobalBounds().size.x * 0.2f)),
                  (util::center(rect).y - (rightAnim.sprite.getGlobalBounds().size.y * 0.5f)) });

            rightAnim.coll_rect = util::scaleRectInPlaceCopy(
                rightAnim.sprite.getGlobalBounds(), { collLengthRatio, collWidthRatio });
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
                if (anim.elapsed_time_sec > anim.time_between_flaming)
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

} // namespace bramblefore
