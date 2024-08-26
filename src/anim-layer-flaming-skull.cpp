// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-flaming-skull.cpp
//
#include "anim-layer-flaming-skull.hpp"

#include "avatar.hpp"
#include "context.hpp"
#include "level.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "sound-player.hpp"
#include "texture-stats.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    FlamingSkullAnimationLayer::FlamingSkullAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_scale{ 1.5f }
        , m_skullBlockTexture{}
        , m_flamesUpTexture{}
        , m_flamesDownTexture{}
        , m_flamesLeftTexture{}
        , m_flamesRightTexture{}
        , m_skullBlockSprites{}
        , m_anims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        m_skullBlockTexture.loadFromFile(
            (t_context.settings.media_path / "image/map-anim/skull-flames-block.png").string());

        TextureStats::instance().process(m_skullBlockTexture);

        m_flamesUpTexture.loadFromFile(
            (t_context.settings.media_path / "image/map-anim/skull-flames-up.png").string());

        TextureStats::instance().process(m_flamesUpTexture);

        m_flamesDownTexture.loadFromFile(
            (t_context.settings.media_path / "image/map-anim/skull-flames-down.png").string());

        TextureStats::instance().process(m_flamesDownTexture);

        m_flamesLeftTexture.loadFromFile(
            (t_context.settings.media_path / "image/map-anim/skull-flames-left.png").string());

        TextureStats::instance().process(m_flamesLeftTexture);

        m_flamesRightTexture.loadFromFile(
            (t_context.settings.media_path / "image/map-anim/skull-flames-right.png").string());

        TextureStats::instance().process(m_flamesRightTexture);

        for (const sf::FloatRect & rect : t_rects)
        {
            sf::Sprite & blockSprite{ m_skullBlockSprites.emplace_back() };
            blockSprite.setTexture(m_skullBlockTexture);
            blockSprite.scale(2.0f, 2.0f);

            blockSprite.setPosition(
                (util::center(rect).x - (blockSprite.getGlobalBounds().width * 0.5f)),
                (util::center(rect).y - (blockSprite.getGlobalBounds().height * 0.5f)));

            FlamesAnim & upAnim{ m_anims.emplace_back() };
            upAnim.direction            = FlameDirection::Up;
            upAnim.time_between_flaming = t_context.random.fromTo(1.5f, 4.0f);
            upAnim.sprite.setTexture(getTexture(upAnim.direction));
            upAnim.sprite.setTextureRect(textureRect(getTexture(upAnim.direction), 0));
            upAnim.sprite.setScale(1.5f, 1.5f);

            upAnim.sprite.setPosition(
                (util::center(rect).x - (upAnim.sprite.getGlobalBounds().width * 0.5f)),
                (rect.top - (upAnim.sprite.getGlobalBounds().height * 0.8f)));

            FlamesAnim & downAnim{ m_anims.emplace_back() };
            downAnim.direction            = FlameDirection::Down;
            downAnim.time_between_flaming = t_context.random.fromTo(1.5f, 4.0f);
            downAnim.sprite.setTexture(getTexture(downAnim.direction));
            downAnim.sprite.setTextureRect(textureRect(getTexture(downAnim.direction), 0));
            downAnim.sprite.setScale(1.5f, 1.5f);

            downAnim.sprite.setPosition(
                (util::center(rect).x - (downAnim.sprite.getGlobalBounds().width * 0.5f)),
                (util::bottom(rect) - (downAnim.sprite.getGlobalBounds().height * 0.2f)));

            FlamesAnim & leftAnim{ m_anims.emplace_back() };
            leftAnim.direction            = FlameDirection::Left;
            leftAnim.time_between_flaming = t_context.random.fromTo(1.5f, 4.0f);
            leftAnim.sprite.setTexture(getTexture(leftAnim.direction));
            leftAnim.sprite.setTextureRect(textureRect(getTexture(leftAnim.direction), 0));
            leftAnim.sprite.setScale(1.5f, 1.5f);

            leftAnim.sprite.setPosition(
                (rect.left - (leftAnim.sprite.getGlobalBounds().width * 0.8f)),
                (util::center(rect).y - (leftAnim.sprite.getGlobalBounds().height * 0.5f)));

            FlamesAnim & rightAnim{ m_anims.emplace_back() };
            rightAnim.direction            = FlameDirection::Right;
            rightAnim.time_between_flaming = t_context.random.fromTo(1.5f, 4.0f);
            rightAnim.sprite.setTexture(getTexture(rightAnim.direction));
            rightAnim.sprite.setTextureRect(textureRect(getTexture(rightAnim.direction), 0));
            rightAnim.sprite.setScale(1.5f, 1.5f);

            rightAnim.sprite.setPosition(
                (util::right(rect) - (rightAnim.sprite.getGlobalBounds().width * 0.2f)),
                (util::center(rect).y - (rightAnim.sprite.getGlobalBounds().height * 0.5f)));
        }
    }

    FlamingSkullAnimationLayer::~FlamingSkullAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void FlamingSkullAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const FlamesAnim & anim : m_anims)
        {
            if (t_context.layout.wholeRect().intersects(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }

        for (const sf::Sprite & sprite : m_skullBlockSprites)
        {
            if (t_context.layout.wholeRect().intersects(sprite.getGlobalBounds()))
            {
                t_target.draw(sprite, t_states);
            }
        }
    }

    void FlamingSkullAnimationLayer::move(const Context &, const float t_amount)
    {
        for (FlamesAnim & anim : m_anims)
        {
            anim.sprite.move(t_amount, 0.0f);
        }

        for (sf::Sprite & sprite : m_skullBlockSprites)
        {
            sprite.move(t_amount, 0.0f);
        }
    }

    void FlamingSkullAnimationLayer::update(Context &, const float t_frameTimeSec)
    {
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
                }
            }
        }
    }

    std::size_t FlamingSkullAnimationLayer::frameCount(const sf::Texture & t_texture) const
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
        const sf::Texture & t_texture, const std::size_t frame) const
    {
        sf::IntRect rect;
        rect.width  = static_cast<int>(t_texture.getSize().y);
        rect.height = rect.width;
        rect.top    = 0;
        rect.left   = (static_cast<int>(frame) * rect.width);
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

            const sf::FloatRect flamesRect{ util::scaleRectInPlaceCopy(
                anim.sprite.getGlobalBounds(), 0.7f) };

            if (t_avatarRect.intersects(flamesRect))
            {
                harm.rect   = flamesRect;
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

} // namespace platformer
