// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// pickups.cpp
//
#include "pickups.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "random.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "sound-player.hpp"
#include "texture-stats.hpp"

namespace platformer
{

    PickupAnimations::PickupAnimations()
        : m_textures()
        , m_anims()
        , m_flareAnims()
        , m_elapsedTimeSec(0.0f)
        , m_timePerFrameSec(0.0f)
        , m_scale(0.0f, 0.0f)
    {
        m_anims.reserve(256);
        m_flareAnims.reserve(8);
    }

    void PickupAnimations::setup(const Settings & settings)
    {
        m_scale.x = settings.tile_scale;
        m_scale.y = settings.tile_scale;

        m_timePerFrameSec = settings.pickups_time_per_frame;

        m_textures.reserve(static_cast<std::size_t>(Pickup::Count));

        for (std::size_t i(0); i < static_cast<std::size_t>(Pickup::Count); ++i)
        {
            const Pickup pickup{ static_cast<Pickup>(i) };

            sf::Texture & texture{ m_textures.emplace_back() };

            texture.loadFromFile(
                (settings.media_path / "image/pickup-anim" / toFilename(pickup)).string());

            TextureStats::instance().process(texture);

            texture.setSmooth(true);
        }
    }

    void PickupAnimations::add(
        const Context & context, const sf::FloatRect & rect, const std::string & name)
    {
        const Pickup pickup{ stringToPickup(name) };
        if (Pickup::Count == pickup)
        {
            std::cout << "Error: PickupAnimations::add(\"" << name
                      << "\") given an unknown name.\n";

            return;
        }

        PickupAnim & anim{ m_anims.emplace_back() };
        anim.which      = pickup;
        anim.anim_index = context.random.zeroToOneLessThan(frameCount(anim.which));
        anim.sprite.setTexture(m_textures.at(static_cast<std::size_t>(pickup)), true);
        anim.sprite.setTextureRect(textureRect(pickup, 0));
        anim.sprite.setScale(m_scale);
        util::setOriginToCenter(anim.sprite);
        anim.sprite.setPosition(util::center(rect));
    }

    std::size_t PickupAnimations::frameCount(const Pickup which) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(which)) };
        return static_cast<std::size_t>(texture.getSize().x / texture.getSize().y);
    }

    const sf::IntRect
        PickupAnimations::textureRect(const Pickup which, const std::size_t frame) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(which)) };

        sf::IntRect rect;
        rect.width  = static_cast<int>(texture.getSize().y);
        rect.height = rect.width;
        rect.top    = 0;
        rect.left   = (static_cast<int>(frame) * static_cast<int>(rect.width));

        return rect;
    }

    void PickupAnimations::update(Context &, const float frameTimeSec)
    {
        m_elapsedTimeSec += frameTimeSec;
        if (m_elapsedTimeSec > m_timePerFrameSec)
        {
            m_elapsedTimeSec -= m_timePerFrameSec;

            for (PickupAnim & anim : m_anims)
            {
                ++anim.anim_index;
                if (anim.anim_index >= frameCount(anim.which))
                {
                    anim.anim_index = 0;
                }

                anim.sprite.setTextureRect(textureRect(anim.which, anim.anim_index));
            }
        }

        bool didAnyFlaresFinish{ false };
        for (PickupFlareAnim & anim : m_flareAnims)
        {
            anim.sprite.scale(1.08f, 1.08f);

            sf::Uint8 alpha = anim.sprite.getColor().a;
            if (alpha >= 10)
            {
                alpha -= 10;
                anim.sprite.setColor(sf::Color(255, 255, 255, alpha));
            }
            else
            {
                didAnyFlaresFinish = true;
                anim.is_alive      = false;
            }
        }

        if (didAnyFlaresFinish)
        {
            m_flareAnims.erase(
                std::remove_if(
                    std::begin(m_flareAnims),
                    std::end(m_flareAnims),
                    [](const PickupFlareAnim & anim) { return (anim.is_alive == false); }),
                std::end(m_flareAnims));
        }
    }

    void PickupAnimations::draw(
        const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const PickupAnim & anim : m_anims)
        {
            if (context.layout.wholeRect().intersects(anim.sprite.getGlobalBounds()))
            {
                target.draw(anim.sprite, states);
            }
        }

        for (const PickupFlareAnim & anim : m_flareAnims)
        {
            if (context.layout.wholeRect().intersects(anim.sprite.getGlobalBounds()))
            {
                target.draw(anim.sprite, states);
            }
        }
    }

    void PickupAnimations::move(const float amount)
    {
        for (PickupAnim & anim : m_anims)
        {
            anim.sprite.move(amount, 0.0f);
        }

        for (PickupFlareAnim & anim : m_flareAnims)
        {
            anim.sprite.move(amount, 0.0f);
        }
    }

    void PickupAnimations::processCollisionWithAvatar(
        Context & context, const sf::FloatRect & avatarRect)
    {
        bool wereAnyPickedUp{ false };
        for (PickupAnim & anim : m_anims)
        {
            if (!avatarRect.intersects(anim.sprite.getGlobalBounds()))
            {
                continue;
            }

            context.sfx.play("pickup");

            wereAnyPickedUp = true;
            anim.is_alive   = false;

            PickupFlareAnim & flare{ m_flareAnims.emplace_back() };
            flare.is_alive = true;
            flare.sprite   = anim.sprite;
            flare.sprite.setTextureRect(textureRect(anim.which, 0));
        }

        if (wereAnyPickedUp)
        {
            m_anims.erase(
                std::remove_if(
                    std::begin(m_anims),
                    std::end(m_anims),
                    [](const PickupAnim & anim) { return (anim.is_alive == false); }),
                std::end(m_anims));
        }
    }

} // namespace platformer
