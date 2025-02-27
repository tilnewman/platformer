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
#include "subsystem/texture-stats.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

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

        M_CHECK(
            m_dripperTexture1.loadFromFile(
                (t_context.settings.media_path / "image/anim/lava-spill1.png").string()),
            "file not found");

        TextureStats::instance().process(m_dripperTexture1);

        M_CHECK(
            m_dripperTexture2.loadFromFile(
                (t_context.settings.media_path / "image/anim/lava-spill2.png").string()),
            "file not found");

        TextureStats::instance().process(m_dripperTexture2);

        M_CHECK(
            m_dripperTexture3.loadFromFile(
                (t_context.settings.media_path / "image/anim/lava-spill3.png").string()),
            "file not found");

        TextureStats::instance().process(m_dripperTexture3);

        //

        M_CHECK(
            m_dripTexture1.loadFromFile(
                (t_context.settings.media_path / "image/anim/lava-drop1.png").string()),
            "file not found");

        TextureStats::instance().process(m_dripTexture1);

        M_CHECK(
            m_dripTexture2.loadFromFile(
                (t_context.settings.media_path / "image/anim/lava-drop2.png").string()),
            "file not found");

        TextureStats::instance().process(m_dripTexture2);

        M_CHECK(
            m_dripTexture3.loadFromFile(
                (t_context.settings.media_path / "image/anim/lava-drop3.png").string()),
            "file not found");

        TextureStats::instance().process(m_dripTexture3);

        //

        M_CHECK(
            m_splatTexture1.loadFromFile(
                (t_context.settings.media_path / "image/anim/lava-splat1.png").string()),
            "file not found");

        TextureStats::instance().process(m_splatTexture1);

        M_CHECK(
            m_splatTexture2.loadFromFile(
                (t_context.settings.media_path / "image/anim/lava-splat2.png").string()),
            "file not found");

        TextureStats::instance().process(m_splatTexture2);

        M_CHECK(
            m_splatTexture3.loadFromFile(
                (t_context.settings.media_path / "image/anim/lava-splat3.png").string()),
            "file not found");

        TextureStats::instance().process(m_splatTexture3);

        //

        for (const LavaRectSize & rectSize : t_rectSizes)
        {
            LavaDripperAnim & anim{ m_dripperAnims.emplace_back() };
            anim.region             = rectSize.rect;
            anim.size               = rectSize.size;
            anim.time_between_drips = t_context.random.fromTo(1.0f, 4.0f);
            anim.sprite.setTexture(dripperTexture(rectSize.size));
            anim.sprite.setTextureRect(textureRect(dripperTexture(rectSize.size), 0));
            anim.sprite.scale({ m_scale, m_scale });

            if (DripSize::Small == anim.size)
            {
                anim.sprite.setPosition(
                    { (util::center(rectSize.rect).x -
                       (anim.sprite.getGlobalBounds().size.x * 0.5f)),
                      (rectSize.rect.position.y - (anim.sprite.getGlobalBounds().size.x * 0.5f)) });
            }
            else if (DripSize::Medium == anim.size)
            {
                anim.sprite.setPosition({ (util::center(rectSize.rect).x -
                                           (anim.sprite.getGlobalBounds().size.x * 0.5f)),
                                          rectSize.rect.position.y });
            }
            else
            {
                anim.sprite.setPosition(
                    { (util::center(rectSize.rect).x -
                       (anim.sprite.getGlobalBounds().size.x * 0.5f)),
                      (rectSize.rect.position.y - (anim.sprite.getGlobalBounds().size.x * 0.5f)) });
            }
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
                if (anim.elapsed_time_sec > anim.time_between_drips)
                {
                    anim.elapsed_time_sec = 0.0f;
                    anim.is_dripping      = true;

                    LavaDripAnim & drip{ m_dripAnims.emplace_back() };
                    drip.region = anim.region;
                    drip.size   = anim.size;
                    drip.sprite.setTexture(dripTexture(drip.size), true);
                    drip.sprite.scale({ m_scale, m_scale });

                    drip.sprite.setPosition({ (util::center(anim.region).x -
                                               (drip.sprite.getGlobalBounds().size.x * 0.5f)),
                                              anim.region.position.y });
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

            LavaSplatAnim & splash{ m_splatAnims.emplace_back() };
            splash.size = anim.size;

            const sf::Texture & texture{ splatTexture(anim.size) };
            splash.sprite.setTexture(texture);
            splash.sprite.setTextureRect(textureRect(texture, 0));

            const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.0f) };
            splash.sprite.scale({ scale, scale });

            splash.sprite.setPosition(
                { (util::center(anim.region).x - (splash.sprite.getGlobalBounds().size.x * 0.5f)),
                  (util::bottom(anim.region) - (splash.sprite.getGlobalBounds().size.y * 0.7f)) });

            if (t_context.layout.wholeRect().findIntersection(splash.sprite.getGlobalBounds()))
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
