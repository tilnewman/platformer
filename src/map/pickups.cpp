// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// pickups.cpp
//
#include "map/pickups.hpp"

#include "bramblefore/settings.hpp"
#include "player/player-info.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

namespace bramblefore
{

    PickupAnimations::PickupAnimations()
        : m_textures{}
        , m_anims{}
        , m_flareAnims{}
        , m_elapsedTimeSec{ 0.0f }
        , m_timePerFrameSec{ 0.1f }
        , m_scale{}
    {
        // these are just harmless guesses about what is in the average map
        m_anims.reserve(64);
        m_flareAnims.reserve(16);
    }

    void PickupAnimations::setup(const Context & t_context)
    {
        const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 2.0f) };
        m_scale.x = scale;
        m_scale.y = scale;

        m_textures.reserve(static_cast<std::size_t>(Pickup::Count));
        for (std::size_t i(0); i < static_cast<std::size_t>(Pickup::Count); ++i)
        {
            const Pickup pickup{ static_cast<Pickup>(i) };

            sf::Texture & texture{ m_textures.emplace_back() };

            util::TextureLoader::load(
                texture, (t_context.settings.media_path / "image/anim" / toFilename(pickup)), true);
        }
    }

    void PickupAnimations::add(
        const Context & t_context, const sf::FloatRect & t_rect, const std::string & t_name)
    {
        const Pickup pickup{ stringToPickup(t_name) };

        M_CHECK((Pickup::Count != pickup), "t_name=\"" << t_name << "\" which is an unknown name.");

        PickupAnim & anim{ m_anims.emplace_back() };
        anim.which      = pickup;
        anim.anim_index = t_context.random.zeroToOneLessThan(frameCount(anim.which));
        anim.sprite.setTexture(m_textures.at(static_cast<std::size_t>(pickup)), true);
        anim.sprite.setTextureRect(textureRect(pickup, 0));
        anim.sprite.setScale(m_scale);
        util::setOriginToCenter(anim.sprite);
        anim.sprite.setPosition(util::center(t_rect));
    }

    std::size_t PickupAnimations::frameCount(const Pickup which) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(which)) };

        if (texture.getSize().y > 0)
        {
            return static_cast<std::size_t>(texture.getSize().x / texture.getSize().y);
        }
        else
        {
            return 0;
        }
    }

    sf::IntRect PickupAnimations::textureRect(const Pickup t_which, const std::size_t t_frame) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(t_which)) };

        sf::IntRect rect;
        rect.size.x     = static_cast<int>(texture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(t_frame) * rect.size.x);

        return rect;
    }

    void PickupAnimations::update(Context &, const float t_frameTimeSec)
    {
        m_elapsedTimeSec += t_frameTimeSec;
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
            anim.sprite.scale({ 1.08f, 1.08f });

            std::uint8_t alpha = anim.sprite.getColor().a;
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
            std::erase_if(
                m_flareAnims, [](const PickupFlareAnim & anim) { return !anim.is_alive; });
        }
    }

    void PickupAnimations::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const PickupAnim & anim : m_anims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }

        for (const PickupFlareAnim & anim : m_flareAnims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void PickupAnimations::move(const float t_amount)
    {
        for (PickupAnim & anim : m_anims)
        {
            anim.sprite.move({ t_amount, 0.0f });
        }

        for (PickupFlareAnim & anim : m_flareAnims)
        {
            anim.sprite.move({ t_amount, 0.0f });
        }
    }

    void PickupAnimations::processCollisionWithAvatar(
        Context & t_context, const sf::FloatRect & t_avatarRect)
    {
        bool wereAnyPickedUp{ false };
        for (PickupAnim & anim : m_anims)
        {
            if (!t_avatarRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                continue;
            }

            t_context.sfx.play("pickup");

            wereAnyPickedUp = true;
            anim.is_alive   = false;

            PickupFlareAnim & flare{ m_flareAnims.emplace_back() };
            flare.is_alive = true;
            flare.sprite   = anim.sprite;
            flare.sprite.setTextureRect(textureRect(anim.which, 0));

            if (Pickup::Bottle == anim.which)
            {
                t_context.player.manaAdjust(t_context, 50);
            }
            else if (Pickup::Coin1 == anim.which)
            {
                t_context.player.coinAdjust(t_context, 1);
            }
            else if (Pickup::Coin10 == anim.which)
            {
                t_context.player.coinAdjust(t_context, 10);
            }
            else if (Pickup::Heart == anim.which)
            {
                t_context.player.healthAdjust(t_context, 50);
            }
            else if (Pickup::Plus == anim.which)
            {
                t_context.player.manaAdjust(t_context, 50);
                t_context.player.healthAdjust(t_context, 50);
            }
            else if (Pickup::Star == anim.which)
            {
                t_context.player.mapStarCollect(t_context);
            }
            // TODO implement a purpose for the crystals...um...
        }

        if (wereAnyPickedUp)
        {
            std::erase_if(m_anims, [](const PickupAnim & anim) { return !anim.is_alive; });
        }
    }

} // namespace bramblefore
