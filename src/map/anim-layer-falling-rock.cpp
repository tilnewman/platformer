// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-falling-rock.cpp
//
#include "anim-layer-falling-rock.hpp"

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

    RockShatterAnim::RockShatterAnim(
        const sf::Sprite & t_sprite, const Rock t_rock, const sf::FloatRect & t_collRect)
        : is_alive{ true }
        , elapsed_time_sec{ 0.0f }
        , time_between_frames_sec{ 0.1f }
        , frame_index{ 0 }
        , sprite{ t_sprite }
        , coll_rect{ t_collRect }
        , rock{ t_rock }
    {}

    //

    RockDropAnim::RockDropAnim(
        const sf::Sprite & t_sprite, const Rock t_rock, const sf::FloatRect & t_rect)
        : is_alive{ true }
        , velocity{ 0.0f }
        , sprite{ t_sprite }
        , rock{ t_rock }
        , fall_region{ t_rect }
    {}

    //

    RockHangingAnim::RockHangingAnim(
        const Context & t_context,
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const Rock t_rock,
        const sf::FloatRect & t_rect,
        const float t_scale)
        : has_dropped{ false }
        , sprite{ t_texture, t_textureRect }
        , trigger_region{}
        , rock{ t_rock }
        , fall_region{ t_rect }
    {
        trigger_region.size.y     = t_context.avatar.collisionRect().size.y;
        trigger_region.position.y = (util::bottom(fall_region) - trigger_region.size.y);
        trigger_region.size.x     = (fall_region.size.y * 0.5f);
        trigger_region.position.x = (util::center(fall_region).x - (trigger_region.size.x * 0.5f));

        sprite.scale({ t_scale, t_scale });

        sprite.setPosition({ (util::center(t_rect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
                             (t_rect.position.y - (sprite.getGlobalBounds().size.y * 0.25f)) });

        if (rock == Rock::Rock1)
        {
            sprite.move({ 0.0f, -(sprite.getGlobalBounds().size.y * 0.1f) });
        }
        else if (rock == Rock::Rock2)
        {
            sprite.move({ 0.0f, -(sprite.getGlobalBounds().size.y * 0.15f) });
        }
        else if (rock == Rock::Rock3)
        {
            sprite.move({ 0.0f, -(sprite.getGlobalBounds().size.y * 0.15f) });
        }
        else if (rock == Rock::Rock4)
        {
            sprite.move({ 0.0f, -(sprite.getGlobalBounds().size.y * 0.25f) });
        }
    }

    //

    FallingRockAnimationLayer::FallingRockAnimationLayer(
        Context & t_context, const std::vector<RectRock> & t_rectRocks)
        : m_texture1{}
        , m_texture2{}
        , m_texture3{}
        , m_texture4{}
        , m_hangingAnims{}
        , m_dropAnims{}
        , m_shatterAnims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        //

        util::TextureLoader::load(
            m_texture1, (t_context.settings.media_path / "image/anim/rock-spike-1.png"));

        util::TextureLoader::load(
            m_texture2, (t_context.settings.media_path / "image/anim/rock-spike-2.png"));

        util::TextureLoader::load(
            m_texture3, (t_context.settings.media_path / "image/anim/rock-spike-3.png"));

        util::TextureLoader::load(
            m_texture4, (t_context.settings.media_path / "image/anim/rock-spike-4.png"));

        //

        m_hangingAnims.reserve(t_rectRocks.size());
        for (const RectRock & rectRock : t_rectRocks)
        {
            m_hangingAnims.emplace_back(
                t_context,
                texture(rectRock.rock),
                textureRect(texture(rectRock.rock), 0),
                rectRock.rock,
                rectRock.rect,
                t_context.layout.calScaleBasedOnResolution(t_context, 1.5f));
        }
    }

    FallingRockAnimationLayer::~FallingRockAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void FallingRockAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const RockHangingAnim & anim : m_hangingAnims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }

        for (const RockDropAnim & anim : m_dropAnims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }

        for (const RockShatterAnim & anim : m_shatterAnims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void FallingRockAnimationLayer::move(const Context &, const float t_amount)
    {
        for (RockHangingAnim & anim : m_hangingAnims)
        {
            anim.sprite.move({ t_amount, 0.0f });
            anim.trigger_region.position.x += t_amount;
            anim.fall_region.position.x += t_amount;
        }

        for (RockDropAnim & anim : m_dropAnims)
        {
            anim.sprite.move({ t_amount, 0.0f });
            anim.fall_region.position.x += t_amount;
        }

        for (RockShatterAnim & anim : m_shatterAnims)
        {
            anim.sprite.move({ t_amount, 0.0f });
            anim.coll_rect.position.x += t_amount;
        }
    }

    void FallingRockAnimationLayer::update(Context & t_context, const float t_frameTimeSec)
    {
        updateHangingRocks(t_context, t_frameTimeSec);
        updateDroppingRocks(t_context, t_frameTimeSec);
        updateShatteringRocks(t_context, t_frameTimeSec);
    }

    void FallingRockAnimationLayer::updateHangingRocks(const Context & t_context, const float)
    {
        const sf::FloatRect avatarCollRect{ t_context.avatar.collisionRect() };

        bool haveAnyDropped{ false };
        for (RockHangingAnim & anim : m_hangingAnims)
        {
            if (!avatarCollRect.findIntersection(anim.trigger_region))
            {
                continue;
            }

            haveAnyDropped   = true;
            anim.has_dropped = true;

            m_dropAnims.emplace_back(anim.sprite, anim.rock, anim.fall_region);
        }

        if (haveAnyDropped)
        {
            std::erase_if(
                m_hangingAnims, [](const RockHangingAnim & anim) { return anim.has_dropped; });
        }
    }

    void FallingRockAnimationLayer::updateDroppingRocks(
        const Context & t_context, const float t_frameTimeSec)
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        bool didAnyRocksLand{ false };
        for (RockDropAnim & anim : m_dropAnims)
        {
            anim.velocity += (t_frameTimeSec * 50.0f);
            anim.sprite.move({ 0.0f, anim.velocity });

            if (util::bottom(util::scaleRectInPlaceCopy(anim.sprite.getGlobalBounds(), 0.5f)) <
                util::bottom(anim.fall_region))
            {
                continue;
            }

            didAnyRocksLand = true;
            anim.is_alive   = false;

            m_shatterAnims.emplace_back(
                anim.sprite,
                anim.rock,
                util::scaleRectInPlaceCopy(anim.sprite.getGlobalBounds(), 0.75f));

            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_context.sfx.play("rock-crumble");
            }
        }

        if (didAnyRocksLand)
        {
            std::erase_if(m_dropAnims, [](const RockDropAnim & anim) { return !anim.is_alive; });
        }
    }

    void FallingRockAnimationLayer::updateShatteringRocks(
        const Context &, const float t_frameTimeSec)
    {
        bool didAnyShatterAnimsFinish{ false };
        for (RockShatterAnim & anim : m_shatterAnims)
        {
            anim.elapsed_time_sec += t_frameTimeSec;
            if (anim.elapsed_time_sec > anim.time_between_frames_sec)
            {
                anim.elapsed_time_sec -= anim.time_between_frames_sec;

                const sf::Texture & rockTexture{ texture(anim.rock) };

                ++anim.frame_index;
                if (anim.frame_index >= frameCount(rockTexture))
                {
                    anim.frame_index         = 0;
                    anim.is_alive            = false;
                    didAnyShatterAnimsFinish = true;
                }

                anim.sprite.setTextureRect(textureRect(rockTexture, anim.frame_index));
            }
        }

        if (didAnyShatterAnimsFinish)
        {
            std::erase_if(
                m_shatterAnims, [](const RockShatterAnim & anim) { return !anim.is_alive; });
        }
    }

    const sf::Texture & FallingRockAnimationLayer::texture(const Rock t_rock) const
    {
        if (t_rock == Rock::Rock1)
        {
            return m_texture1;
        }
        else if (t_rock == Rock::Rock2)
        {
            return m_texture2;
        }
        else if (t_rock == Rock::Rock3)
        {
            return m_texture3;
        }
        else
        {
            return m_texture4;
        }
    }

    std::size_t FallingRockAnimationLayer::frameCount(const sf::Texture & t_texture) const noexcept
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

    sf::IntRect FallingRockAnimationLayer::textureRect(
        const sf::Texture & t_texture, const std::size_t frame) const noexcept
    {
        sf::IntRect rect;
        rect.size.x     = static_cast<int>(t_texture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(frame) * rect.size.x);

        return rect;
    }

    Harm FallingRockAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        for (const RockDropAnim & anim : m_dropAnims)
        {
            const sf::FloatRect rockRect{ anim.sprite.getGlobalBounds() };
            if (t_avatarRect.findIntersection(rockRect))
            {
                return makeHarm(anim.rock, rockRect);
            }
        }

        for (const RockShatterAnim & anim : m_shatterAnims)
        {
            const sf::FloatRect shatterRect{ util::scaleRectInPlaceCopy(
                anim.sprite.getGlobalBounds(), 0.7f) };

            if (t_avatarRect.findIntersection(shatterRect))
            {
                return makeHarm(anim.rock, shatterRect);
            }
        }

        return Harm{};
    }

    Harm FallingRockAnimationLayer::makeHarm(
        const Rock t_rock, const sf::FloatRect & t_rect) const noexcept
    {
        Harm harm;
        harm.rect = t_rect;
        harm.sfx  = "";

        if (t_rock == Rock::Rock1)
        {
            harm.damage = 20;
        }
        else if (t_rock == Rock::Rock2)
        {
            harm.damage = 16;
        }
        else if (t_rock == Rock::Rock3)
        {
            harm.damage = 12;
        }
        else
        {
            harm.damage = 8;
        }

        return harm;
    }

} // namespace bramblefore
