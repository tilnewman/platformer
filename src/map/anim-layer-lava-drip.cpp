// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-lava-drip.cpp
//
#include "anim-layer-lava-drip.hpp"

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

    LavaDripperAnim::LavaDripperAnim(
        const DripSize t_size,
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const sf::FloatRect & t_screenRect,
        const float t_timeBetweenDripSec,
        const float t_scale)
        : is_dripping{ false }
        , size{ t_size }
        , elapsed_time_sec{ 0.0f }
        , time_between_drip_sec{ t_timeBetweenDripSec }
        , time_between_frames_sec{ 0.15f }
        , frame_index{ 0 }
        , sprite{ t_texture, t_textureRect }
        , region{ t_screenRect }
    {
        sprite.scale({ t_scale, t_scale });

        if (DripSize::Small == size)
        {
            sprite.setPosition(
                { (util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
                  (t_screenRect.position.y - (sprite.getGlobalBounds().size.x * 0.5f)) });
        }
        else if (DripSize::Medium == size)
        {
            sprite.setPosition(
                { (util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
                  t_screenRect.position.y });
        }
        else
        {
            sprite.setPosition(
                { (util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
                  (t_screenRect.position.y - (sprite.getGlobalBounds().size.x * 0.5f)) });
        }
    }

    //

    LavaSplatAnim::LavaSplatAnim(
        const DripSize t_size,
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const float t_scale,
        const sf::FloatRect & t_screenRect)
        : is_alive{ true }
        , size{ t_size }
        , elapsed_time_sec{ 0.0f }
        , time_between_frames_sec{ 0.1f }
        , frame_index{ 0 }
        , sprite{ t_texture, t_textureRect }
    {
        sprite.scale({ t_scale, t_scale });

        sprite.setPosition(
            { (util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
              (util::bottom(t_screenRect) - (sprite.getGlobalBounds().size.y * 0.7f)) });
    }

    //

    LavaDripAnim::LavaDripAnim(
        const DripSize t_size,
        const sf::Texture & t_texture,
        const sf::FloatRect & t_screenRect,
        const float t_scale)
        : is_alive{ true }
        , size{ t_size }
        , velocity{ 0.0f }
        , sprite{ t_texture }
        , region{ t_screenRect }
    {
        sprite.scale({ t_scale, t_scale });

        sprite.setPosition({ (util::center(region).x - (sprite.getGlobalBounds().size.x * 0.5f)),
                             region.position.y });
    }

    //

    LavaDripAnimationLayer::LavaDripAnimationLayer(
        Context & t_context, const std::vector<LavaRectSize> & t_rectSizes)
        : m_scale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.5f) }
        , m_dripperTexture1{}
        , m_dripperTexture2{}
        , m_dripperTexture3{}
        , m_dripTexture1{}
        , m_dripTexture2{}
        , m_dripTexture3{}
        , m_splatTexture1{}
        , m_splatTexture2{}
        , m_splatTexture3{}
        , m_dripperAnims{}
        , m_dripAnims{}
        , m_splatAnims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        //

        util::TextureLoader::load(
            m_dripperTexture1, (t_context.settings.media_path / "image/anim/lava-spill1.png"));

        util::TextureLoader::load(
            m_dripperTexture2, (t_context.settings.media_path / "image/anim/lava-spill2.png"));

        util::TextureLoader::load(
            m_dripperTexture3, (t_context.settings.media_path / "image/anim/lava-spill3.png"));

        //

        util::TextureLoader::load(
            m_dripTexture1, (t_context.settings.media_path / "image/anim/lava-drop1.png"));

        util::TextureLoader::load(
            m_dripTexture2, (t_context.settings.media_path / "image/anim/lava-drop2.png"));

        util::TextureLoader::load(
            m_dripTexture3, (t_context.settings.media_path / "image/anim/lava-drop3.png"));

        //

        util::TextureLoader::load(
            m_splatTexture1, (t_context.settings.media_path / "image/anim/lava-splat1.png"));

        util::TextureLoader::load(
            m_splatTexture2, (t_context.settings.media_path / "image/anim/lava-splat2.png"));

        util::TextureLoader::load(
            m_splatTexture3, (t_context.settings.media_path / "image/anim/lava-splat3.png"));

        //

        m_dripperAnims.reserve(t_rectSizes.size());
        for (const LavaRectSize & rectSize : t_rectSizes)
        {
            m_dripperAnims.emplace_back(
                rectSize.size,
                dripperTexture(rectSize.size),
                textureRect(dripperTexture(rectSize.size), 0),
                rectSize.rect,
                t_context.random.fromTo(1.0f, 4.0f),
                m_scale);
        }
    }

    LavaDripAnimationLayer::~LavaDripAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void LavaDripAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const LavaDripAnim & anim : m_dripAnims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }

        for (const LavaDripperAnim & anim : m_dripperAnims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }

        for (const LavaSplatAnim & anim : m_splatAnims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void LavaDripAnimationLayer::move(const Context &, const float t_amount)
    {
        for (LavaDripAnim & anim : m_dripAnims)
        {
            anim.sprite.move({ t_amount, 0.0f });
            anim.region.position.x += t_amount;
        }

        for (LavaDripperAnim & anim : m_dripperAnims)
        {
            anim.sprite.move({ t_amount, 0.0f });
            anim.region.position.x += t_amount;
        }

        for (LavaSplatAnim & anim : m_splatAnims)
        {
            anim.sprite.move({ t_amount, 0.0f });
        }
    }

    void LavaDripAnimationLayer::update(Context & t_context, const float t_frameTimeSec)
    {
        updateDrippers(t_frameTimeSec);
        updateDrips(t_context, t_frameTimeSec);
        updateSplats(t_frameTimeSec);
    }

    void LavaDripAnimationLayer::updateDrippers(const float t_frameTimeSec)
    {
        for (LavaDripperAnim & anim : m_dripperAnims)
        {
            if (anim.is_dripping)
            {
                anim.elapsed_time_sec += t_frameTimeSec;
                if (anim.elapsed_time_sec > anim.time_between_frames_sec)
                {
                    anim.elapsed_time_sec -= anim.time_between_frames_sec;

                    const sf::Texture & texture{ dripperTexture(anim.size) };

                    ++anim.frame_index;
                    if (anim.frame_index >= frameCount(texture))
                    {
                        anim.frame_index      = 0;
                        anim.elapsed_time_sec = 0.0f;
                        anim.is_dripping      = false;
                    }

                    anim.sprite.setTextureRect(textureRect(texture, anim.frame_index));
                }
            }
            else
            {
                anim.elapsed_time_sec += t_frameTimeSec;
                if (anim.elapsed_time_sec > anim.time_between_drip_sec)
                {
                    anim.elapsed_time_sec = 0.0f;
                    anim.is_dripping      = true;

                    m_dripAnims.emplace_back(
                        anim.size, dripTexture(anim.size), anim.region, m_scale);
                }
            }
        }
    }

    void LavaDripAnimationLayer::updateDrips(const Context & t_context, const float t_frameTimeSec)
    {
        bool didAnyDripsLand{ false };
        for (LavaDripAnim & anim : m_dripAnims)
        {
            anim.velocity += (t_frameTimeSec * 50.0f);
            anim.sprite.move({ 0.0f, anim.velocity });

            if (util::bottom(anim.sprite) < util::bottom(anim.region))
            {
                continue;
            }

            didAnyDripsLand = true;
            anim.is_alive   = false;

            LavaSplatAnim & splashAnim{ m_splatAnims.emplace_back(
                anim.size,
                splatTexture(anim.size),
                textureRect(splatTexture(anim.size), 0),
                t_context.layout.calScaleBasedOnResolution(t_context, 1.0f),anim.region) };

            if (t_context.layout.wholeRect().findIntersection(splashAnim.sprite.getGlobalBounds()))
            {
                t_context.sfx.play("splat");
            }
        }

        if (didAnyDripsLand)
        {
            std::erase_if(m_dripAnims, [](const LavaDripAnim & anim) { return !anim.is_alive; });
        }
    }

    void LavaDripAnimationLayer::updateSplats(const float t_frameTimeSec)
    {
        bool didAnySplashAnimsFinish{ false };
        for (LavaSplatAnim & anim : m_splatAnims)
        {
            anim.elapsed_time_sec += t_frameTimeSec;
            if (anim.elapsed_time_sec > anim.time_between_frames_sec)
            {
                anim.elapsed_time_sec -= anim.time_between_frames_sec;

                const sf::Texture & texture{ splatTexture(anim.size) };

                ++anim.frame_index;
                if (anim.frame_index >= frameCount(texture))
                {
                    anim.frame_index        = 0;
                    anim.is_alive           = false;
                    didAnySplashAnimsFinish = true;
                }

                anim.sprite.setTextureRect(textureRect(texture, anim.frame_index));
            }
        }

        if (didAnySplashAnimsFinish)
        {
            std::erase_if(m_splatAnims, [](const LavaSplatAnim & anim) { return !anim.is_alive; });
        }
    }

    std::size_t LavaDripAnimationLayer::frameCount(const sf::Texture & t_texture) const noexcept
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

    sf::IntRect LavaDripAnimationLayer::textureRect(
        const sf::Texture & t_texture, const std::size_t frame) const noexcept
    {
        sf::IntRect rect;
        rect.size.x     = static_cast<int>(t_texture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(frame) * rect.size.x);

        return rect;
    }

    Harm LavaDripAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        for (const LavaDripAnim & anim : m_dripAnims)
        {
            const sf::FloatRect acidRect{ anim.sprite.getGlobalBounds() };
            if (t_avatarRect.findIntersection(acidRect))
            {
                return makeHarm(acidRect);
            }
        }

        for (const LavaSplatAnim & anim : m_splatAnims)
        {
            const sf::FloatRect acidRect{ util::scaleRectInPlaceCopy(
                anim.sprite.getGlobalBounds(), 0.7f) };

            if (t_avatarRect.findIntersection(acidRect))
            {
                return makeHarm(acidRect);
            }
        }

        return Harm{};
    }

    Harm LavaDripAnimationLayer::makeHarm(const sf::FloatRect & t_rect) const noexcept
    {
        Harm harm;
        harm.rect   = t_rect;
        harm.damage = 18;
        harm.sfx    = "acid";

        return harm;
    }

} // namespace bramblefore
