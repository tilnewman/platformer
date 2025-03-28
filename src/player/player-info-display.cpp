// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// player-info-display.hpp
//
#include "player-info-display.hpp"

#include "avatar/avatar-textures.hpp"
#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/font.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

#include <filesystem>
#include <string>

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    PlayerInfoDisplay::PlayerInfoDisplay()
        : m_halfFrameTexture{}
        , m_halfFrameSprite{ m_halfFrameTexture }
        , m_fullFrameTexture{}
        , m_fullFrameSprite{ m_fullFrameTexture }
        , m_avatarIconSprite{ util::SfmlDefaults::instance().texture() }
        , m_bgAvatarBgVerts{}
        , m_barFrameTexture{}
        , m_healthBarFrameSprite{ util::SfmlDefaults::instance().texture() }
        , m_manaBarFrameSprite{ util::SfmlDefaults::instance().texture() }
        , m_barFillLeftTexture{}
        , m_barFillMiddleTexture{}
        , m_barFillRightTexture{}
        , m_healthBarRect{}
        , m_healthBarLeftSprite{ m_barFillLeftTexture }
        , m_healthBarMiddleSprite{ m_barFillMiddleTexture }
        , m_healthBarRightSprite{ m_barFillRightTexture }
        , m_manaBarRect{}
        , m_manaBarLeftSprite{ m_barFillLeftTexture }
        , m_manaBarMiddleSprite{ m_barFillMiddleTexture }
        , m_manaBarRightSprite{ m_barFillRightTexture }
        , m_healthColor{ 255, 50, 50 }
        , m_manaColor{ 50, 50, 255 }
        , m_barFillMax{ 0.0f }
        , m_willDrawHealthBarRight{ true }
        , m_willDrawHealthBarLeft{ true }
        , m_willDrawManaBarRight{ true }
        , m_willDrawManaBarLeft{ true }
        , m_coinTexture{}
        , m_coinSprite{ m_coinTexture }
        , m_coinText{ util::SfmlDefaults::instance().font() }
        , m_starBrownTexture{}
        , m_starBrown1Sprite{ m_starBrownTexture }
        , m_starBrown2Sprite{ m_starBrownTexture }
        , m_starBrown3Sprite{ m_starBrownTexture }
        , m_starBrown4Sprite{ m_starBrownTexture }
        , m_starBrown5Sprite{ m_starBrownTexture }
        , m_starYellowTexture{}
        , m_starYellow1Sprite{ m_starYellowTexture }
        , m_starYellow2Sprite{ m_starYellowTexture }
        , m_starYellow3Sprite{ m_starYellowTexture }
        , m_starYellow4Sprite{ m_starYellowTexture }
        , m_starYellow5Sprite{ m_starYellowTexture }
    {}

    void PlayerInfoDisplay::setup(const Context & t_context)
    {
        m_barFillMax = t_context.layout.calScaleBasedOnResolution(t_context, 226.0f);

        util::TextureLoader::load(
            m_halfFrameTexture, (t_context.settings.media_path / "image/ui/half-frame.png"));

        m_halfFrameSprite.setTexture(m_halfFrameTexture, true);

        const float frameScale{ t_context.layout.calScaleBasedOnResolution(t_context, 2.0f) };
        m_halfFrameSprite.scale({ frameScale, frameScale });

        const float posDimm{ t_context.layout.wholeSize().y * 0.1f };
        m_halfFrameSprite.setPosition({ posDimm, posDimm });

        //

        util::TextureLoader::load(
            m_fullFrameTexture, (t_context.settings.media_path / "image/ui/full-frame.png"));

        m_fullFrameSprite.setTexture(m_fullFrameTexture, true);
        m_fullFrameSprite.scale({ frameScale, frameScale });
        m_fullFrameSprite.setPosition(m_halfFrameSprite.getPosition() + sf::Vector2f(8.0f, 8.0f));

        //

        const sf::FloatRect m_bgRect = m_fullFrameSprite.getGlobalBounds();
        util::appendTriangleVerts(m_bgRect, m_bgAvatarBgVerts, sf::Color::Black);
        const sf::Color avatarBgColor{ 64, 64, 64 };
        m_bgAvatarBgVerts.at(0).color = avatarBgColor;
        m_bgAvatarBgVerts.at(1).color = avatarBgColor;
        m_bgAvatarBgVerts.at(4).color = avatarBgColor;

        //

        m_avatarIconSprite.setTexture(
            AvatarTextureManager::instance().getIcon(t_context.player.avatarType()), true);

        util::scaleAndCenterInside(m_avatarIconSprite, m_bgRect);

        //

        util::TextureLoader::load(
            m_barFrameTexture, (t_context.settings.media_path / "image/ui/bar-frame.png"));

        m_healthBarFrameSprite.setTexture(m_barFrameTexture, true);
        m_healthBarFrameSprite.scale({ frameScale, frameScale });

        m_healthBarFrameSprite.setPosition(
            { (util::right(m_fullFrameSprite) - 2.0f),
              (util::bottom(m_fullFrameSprite) -
               (m_healthBarFrameSprite.getGlobalBounds().size.y * 2.0f)) });

        //

        m_manaBarFrameSprite.setTexture(m_barFrameTexture, true);
        m_manaBarFrameSprite.scale({ frameScale, frameScale });

        m_manaBarFrameSprite.setPosition(
            { (util::right(m_fullFrameSprite) - 2.0f),
              (util::bottom(m_fullFrameSprite) - m_manaBarFrameSprite.getGlobalBounds().size.y) });

        //

        util::TextureLoader::load(
            m_barFillLeftTexture, (t_context.settings.media_path / "image/ui/bar-fill-left.png"));

        m_healthBarLeftSprite.setTexture(m_barFillLeftTexture, true);
        m_healthBarLeftSprite.scale({ frameScale, frameScale });
        m_healthBarLeftSprite.setColor(m_healthColor);

        const sf::Vector2f barPosOffset{
            t_context.layout.calScaleBasedOnResolution(t_context, 6.0f),
            t_context.layout.calScaleBasedOnResolution(t_context, 8.0f)
        };

        m_healthBarLeftSprite.setPosition(m_healthBarFrameSprite.getPosition() + barPosOffset);

        //

        util::TextureLoader::load(
            m_barFillMiddleTexture,
            (t_context.settings.media_path / "image/ui/bar-fill-middle.png"));

        m_healthBarMiddleSprite.setTexture(m_barFillMiddleTexture, true);
        m_healthBarMiddleSprite.scale({ frameScale, frameScale });
        m_healthBarMiddleSprite.setColor(m_healthColor);

        m_healthBarRect.position.x = util::right(m_healthBarLeftSprite);
        m_healthBarRect.position.y = m_healthBarLeftSprite.getPosition().y;
        m_healthBarRect.size.x     = m_barFillMax;
        m_healthBarRect.size.y     = m_healthBarMiddleSprite.getGlobalBounds().size.y;

        util::scaleAndCenterInside(m_healthBarMiddleSprite, m_healthBarRect);

        //

        util::TextureLoader::load(
            m_barFillRightTexture, (t_context.settings.media_path / "image/ui/bar-fill-right.png"));

        m_healthBarRightSprite.setTexture(m_barFillRightTexture, true);
        m_healthBarRightSprite.scale({ frameScale, frameScale });
        m_healthBarRightSprite.setColor(m_healthColor);

        m_healthBarRightSprite.setPosition(
            { util::right(m_healthBarMiddleSprite), m_healthBarMiddleSprite.getPosition().y });

        //

        m_manaBarLeftSprite.setTexture(m_barFillLeftTexture, true);
        m_manaBarLeftSprite.scale({ frameScale, frameScale });
        m_manaBarLeftSprite.setColor(m_manaColor);
        m_manaBarLeftSprite.setPosition(m_manaBarFrameSprite.getPosition() + barPosOffset);

        //

        m_manaBarMiddleSprite.setTexture(m_barFillMiddleTexture, true);
        m_manaBarMiddleSprite.scale({ frameScale, frameScale });
        m_manaBarMiddleSprite.setColor(m_manaColor);

        m_manaBarRect.position.x = util::right(m_manaBarLeftSprite);
        m_manaBarRect.position.y = m_manaBarLeftSprite.getPosition().y;
        m_manaBarRect.size.x     = m_barFillMax;
        m_manaBarRect.size.y     = m_manaBarMiddleSprite.getGlobalBounds().size.y;

        util::scaleAndCenterInside(m_manaBarMiddleSprite, m_manaBarRect);

        //

        m_manaBarRightSprite.setTexture(m_barFillRightTexture, true);
        m_manaBarRightSprite.scale({ frameScale, frameScale });
        m_manaBarRightSprite.setColor(m_manaColor);

        m_manaBarRightSprite.setPosition(
            { util::right(m_manaBarMiddleSprite), m_manaBarMiddleSprite.getPosition().y });

        //

        const float starRectHeight{ m_healthBarFrameSprite.getPosition().y -
                                    m_fullFrameSprite.getPosition().y };

        const float starRectHeightMiddle{ m_healthBarFrameSprite.getPosition().y -
                                          (starRectHeight * 0.5f) };

        util::TextureLoader::load(
            m_coinTexture, (t_context.settings.media_path / "image/ui/coin.png"), true);

        m_coinSprite.setTexture(m_coinTexture, true);

        const float coinScale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.7f) };
        m_coinSprite.scale({ coinScale, coinScale });

        m_coinSprite.setPosition(
            { util::right(m_fullFrameSprite),
              (starRectHeightMiddle - (m_coinSprite.getGlobalBounds().size.y * 0.5f)) });

        //

        m_coinText =
            t_context.font.makeText(Font::Title, FontSize::Large, "0", sf::Color(236, 218, 95));

        m_coinText.scale({ 1.4f, 1.4f });

        m_coinText.setPosition(
            { (util::right(m_coinSprite) + 8.0f),
              (starRectHeightMiddle - (m_coinText.getGlobalBounds().size.y * 0.5f)) });

        //

        util::TextureLoader::load(
            m_starBrownTexture, (t_context.settings.media_path / "image/ui/star-brown.png"), true);

        m_starBrown1Sprite.setTexture(m_starBrownTexture, true);

        const float starScale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.5f) };
        m_starBrown1Sprite.scale({ starScale, starScale });

        m_starBrown1Sprite.setPosition({ (m_fullFrameSprite.getPosition().x + 10.0f),
                                         (util::bottom(m_halfFrameSprite) + 10.0f) });

        m_starBrown2Sprite.setTexture(m_starBrownTexture, true);
        m_starBrown2Sprite.scale(m_starBrown1Sprite.getScale());

        m_starBrown2Sprite.setPosition(
            { util::right(m_starBrown1Sprite), m_starBrown1Sprite.getPosition().y });

        m_starBrown3Sprite.setTexture(m_starBrownTexture, true);
        m_starBrown3Sprite.scale(m_starBrown1Sprite.getScale());

        m_starBrown3Sprite.setPosition(
            { util::right(m_starBrown2Sprite), m_starBrown1Sprite.getPosition().y });

        m_starBrown4Sprite.setTexture(m_starBrownTexture, true);
        m_starBrown4Sprite.scale(m_starBrown1Sprite.getScale());

        m_starBrown4Sprite.setPosition(
            { util::right(m_starBrown3Sprite), m_starBrown1Sprite.getPosition().y });

        m_starBrown5Sprite.setTexture(m_starBrownTexture, true);
        m_starBrown5Sprite.scale(m_starBrown1Sprite.getScale());

        m_starBrown5Sprite.setPosition(
            { util::right(m_starBrown4Sprite), m_starBrown1Sprite.getPosition().y });

        //

        util::TextureLoader::load(
            m_starYellowTexture,
            (t_context.settings.media_path / "image/ui/star-yellow.png"),
            true);

        m_starYellow1Sprite.setTexture(m_starYellowTexture, true);
        m_starYellow1Sprite.scale(m_starBrown1Sprite.getScale());

        m_starYellow2Sprite.setTexture(m_starYellowTexture, true);
        m_starYellow2Sprite.scale(m_starBrown1Sprite.getScale());

        m_starYellow3Sprite.setTexture(m_starYellowTexture, true);
        m_starYellow3Sprite.scale(m_starBrown1Sprite.getScale());

        m_starYellow4Sprite.setTexture(m_starYellowTexture, true);
        m_starYellow4Sprite.scale(m_starBrown1Sprite.getScale());

        m_starYellow5Sprite.setTexture(m_starYellowTexture, true);
        m_starYellow5Sprite.scale(m_starBrown1Sprite.getScale());

        setStarCount(0);
    }

    void PlayerInfoDisplay::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_target.draw(m_halfFrameSprite, t_states);

        if (!m_bgAvatarBgVerts.empty())
        {
            t_target.draw(
                &m_bgAvatarBgVerts[0],
                m_bgAvatarBgVerts.size(),
                sf::PrimitiveType::Triangles,
                t_states);
        }

        t_target.draw(m_avatarIconSprite, t_states);
        t_target.draw(m_fullFrameSprite, t_states);

        t_target.draw(m_healthBarFrameSprite, t_states);
        t_target.draw(m_healthBarMiddleSprite, t_states);

        if (m_willDrawHealthBarLeft)
        {
            t_target.draw(m_healthBarLeftSprite, t_states);
        }

        if (m_willDrawHealthBarRight)
        {
            t_target.draw(m_healthBarRightSprite, t_states);
        }

        t_target.draw(m_manaBarFrameSprite, t_states);
        t_target.draw(m_manaBarMiddleSprite, t_states);

        if (m_willDrawManaBarLeft)
        {
            t_target.draw(m_manaBarLeftSprite, t_states);
        }

        if (m_willDrawManaBarRight)
        {
            t_target.draw(m_manaBarRightSprite, t_states);
        }

        t_target.draw(m_coinSprite, t_states);
        t_target.draw(m_coinText);

        t_target.draw(m_starBrown1Sprite, t_states);
        t_target.draw(m_starBrown2Sprite, t_states);
        t_target.draw(m_starBrown3Sprite, t_states);
        t_target.draw(m_starBrown4Sprite, t_states);
        t_target.draw(m_starBrown5Sprite, t_states);

        // always draw stars because when not used they are positioned off screen
        t_target.draw(m_starYellow1Sprite, t_states);
        t_target.draw(m_starYellow2Sprite, t_states);
        t_target.draw(m_starYellow3Sprite, t_states);
        t_target.draw(m_starYellow4Sprite, t_states);
        t_target.draw(m_starYellow5Sprite, t_states);
    }

    void PlayerInfoDisplay::setHealthBar(const float t_ratio)
    {
        m_willDrawHealthBarRight = (t_ratio >= 1.0f);
        m_willDrawHealthBarLeft  = (t_ratio > 0.0f);
        m_healthBarRect.size.x   = (m_barFillMax * t_ratio);
        util::scaleAndCenterInside(m_healthBarMiddleSprite, m_healthBarRect);
    }

    void PlayerInfoDisplay::setManaBar(const float t_ratio)
    {
        m_willDrawManaBarRight = (t_ratio >= 1.0f);
        m_willDrawManaBarLeft  = (t_ratio > 0.0f);
        m_manaBarRect.size.x   = (m_barFillMax * t_ratio);
        util::scaleAndCenterInside(m_manaBarMiddleSprite, m_manaBarRect);
    }

    void PlayerInfoDisplay::setCoinCount(const Coin_t t_count)
    {
        m_coinText.setString(std::to_string(t_count));
        util::setOriginToPosition(m_coinText);
    }

    void PlayerInfoDisplay::setStarCount(const int t_count)
    {
        // just keep stars offscreen until needed
        m_starYellow1Sprite.setPosition({ -1000.0f, -1000.0f });
        m_starYellow2Sprite.setPosition({ -1000.0f, -1000.0f });
        m_starYellow3Sprite.setPosition({ -1000.0f, -1000.0f });
        m_starYellow4Sprite.setPosition({ -1000.0f, -1000.0f });
        m_starYellow5Sprite.setPosition({ -1000.0f, -1000.0f });

        if (0 == t_count)
        {
        }
        else if (1 == t_count)
        {
            m_starYellow1Sprite.setPosition(m_starBrown1Sprite.getPosition());
        }
        else if (2 == t_count)
        {
            m_starYellow1Sprite.setPosition(m_starBrown1Sprite.getPosition());
            m_starYellow2Sprite.setPosition(m_starBrown2Sprite.getPosition());
        }
        else if (3 == t_count)
        {
            m_starYellow1Sprite.setPosition(m_starBrown1Sprite.getPosition());
            m_starYellow2Sprite.setPosition(m_starBrown2Sprite.getPosition());
            m_starYellow3Sprite.setPosition(m_starBrown3Sprite.getPosition());
        }
        else if (4 == t_count)
        {
            m_starYellow1Sprite.setPosition(m_starBrown1Sprite.getPosition());
            m_starYellow2Sprite.setPosition(m_starBrown2Sprite.getPosition());
            m_starYellow3Sprite.setPosition(m_starBrown3Sprite.getPosition());
            m_starYellow4Sprite.setPosition(m_starBrown4Sprite.getPosition());
        }
        else
        {
            m_starYellow1Sprite.setPosition(m_starBrown1Sprite.getPosition());
            m_starYellow2Sprite.setPosition(m_starBrown2Sprite.getPosition());
            m_starYellow3Sprite.setPosition(m_starBrown3Sprite.getPosition());
            m_starYellow4Sprite.setPosition(m_starBrown4Sprite.getPosition());
            m_starYellow5Sprite.setPosition(m_starBrown5Sprite.getPosition());
        }
    }

} // namespace bramblefore
