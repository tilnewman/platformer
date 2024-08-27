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
#include "subsystem/texture-stats.hpp"
#include "util/sfml-util.hpp"

#include <filesystem>
#include <string>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    PlayerInfoDisplay::PlayerInfoDisplay()
        : m_halfFrameTexture{}
        , m_halfFrameSprite{}
        , m_fullFrameTexture{}
        , m_fullFrameSprite{}
        , m_avatarIconSprite{}
        , m_bgFadeVerts{}
        , m_barFrameTexture{}
        , m_healthBarFrameSprite{}
        , m_manaBarFrameSprite{}
        , m_barFillLeftTexture{}
        , m_barFillMiddleTexture{}
        , m_barFillRightTexture{}
        , m_healthBarRect{}
        , m_healthBarLeftSprite{}
        , m_healthBarMiddleSprite{}
        , m_healthBarRightSprite{}
        , m_manaBarRect{}
        , m_manaBarLeftSprite{}
        , m_manaBarMiddleSprite{}
        , m_manaBarRightSprite{}
        , m_healthColor{ 255, 50, 50 }
        , m_manaColor{ 50, 50, 255 }
        , m_barFillMax{ 226.0f }
        , m_willDrawHealthBarRight{ true }
        , m_willDrawHealthBarLeft{ true }
        , m_willDrawManaBarRight{ true }
        , m_willDrawManaBarLeft{ true }
        , m_coinTexture{}
        , m_coinSprite{}
        , m_coinText{}
        , m_starBrownTexture{}
        , m_starBrown1Sprite{}
        , m_starBrown2Sprite{}
        , m_starBrown3Sprite{}
        , m_starBrown4Sprite{}
        , m_starBrown5Sprite{}
        , m_starYellowTexture{}
        , m_starYellow1Sprite{}
        , m_starYellow2Sprite{}
        , m_starYellow3Sprite{}
        , m_starYellow4Sprite{}
        , m_starYellow5Sprite{}
    {}

    void PlayerInfoDisplay::setup(const Context & t_context)
    {
        m_halfFrameTexture.loadFromFile(
            (t_context.settings.media_path / "image/ui/half-frame.png").string());

        TextureStats::instance().process(m_halfFrameTexture);
        m_halfFrameSprite.setTexture(m_halfFrameTexture);
        m_halfFrameSprite.scale(2.0f, 2.0f);

        const float posDimm{ t_context.layout.wholeSize().y * 0.1f };
        m_halfFrameSprite.setPosition(posDimm, posDimm);

        //

        m_fullFrameTexture.loadFromFile(
            (t_context.settings.media_path / "image/ui/full-frame.png").string());

        TextureStats::instance().process(m_fullFrameTexture);
        m_fullFrameSprite.setTexture(m_fullFrameTexture);
        m_fullFrameSprite.scale(2.0f, 2.0f);

        m_fullFrameSprite.setPosition(m_halfFrameSprite.getPosition() + sf::Vector2f(8.0f, 8.0f));

        //

        const sf::FloatRect m_bgRect = m_fullFrameSprite.getGlobalBounds();
        util::appendQuadVerts(m_bgRect, m_bgFadeVerts, sf::Color::Black);
        m_bgFadeVerts.at(0).color = sf::Color(64, 64, 64);
        m_bgFadeVerts.at(1).color = m_bgFadeVerts.at(0).color;

        //

        m_avatarIconSprite.setTexture(
            AvatarTextureManager::instance().getIcon(t_context.player.avatarType()));

        util::scaleAndCenterInside(m_avatarIconSprite, m_bgRect);

        //

        m_barFrameTexture.loadFromFile(
            (t_context.settings.media_path / "image/ui/bar-frame.png").string());

        TextureStats::instance().process(m_barFrameTexture);

        m_healthBarFrameSprite.setTexture(m_barFrameTexture);
        m_healthBarFrameSprite.scale(2.0f, 2.0f);

        m_healthBarFrameSprite.setPosition(
            (util::right(m_fullFrameSprite) - 2.0f),
            (util::bottom(m_fullFrameSprite) -
             (m_healthBarFrameSprite.getGlobalBounds().height * 2.0f)));

        //

        m_manaBarFrameSprite.setTexture(m_barFrameTexture);
        m_manaBarFrameSprite.scale(2.0f, 2.0f);

        m_manaBarFrameSprite.setPosition(
            (util::right(m_fullFrameSprite) - 2.0f),
            (util::bottom(m_fullFrameSprite) - m_manaBarFrameSprite.getGlobalBounds().height));

        //

        m_barFillLeftTexture.loadFromFile(
            (t_context.settings.media_path / "image/ui/bar-fill-left.png").string());

        TextureStats::instance().process(m_barFillLeftTexture);

        m_healthBarLeftSprite.setTexture(m_barFillLeftTexture);
        m_healthBarLeftSprite.scale(2.0f, 2.0f);
        m_healthBarLeftSprite.setColor(m_healthColor);

        m_healthBarLeftSprite.setPosition(
            m_healthBarFrameSprite.getPosition() + sf::Vector2f(6.0f, 8.0f));

        //

        m_barFillMiddleTexture.loadFromFile(
            (t_context.settings.media_path / "image/ui/bar-fill-middle.png").string());

        TextureStats::instance().process(m_barFillMiddleTexture);

        m_healthBarMiddleSprite.setTexture(m_barFillMiddleTexture);
        m_healthBarMiddleSprite.scale(2.0f, 2.0f);
        m_healthBarMiddleSprite.setColor(m_healthColor);

        m_healthBarRect.left   = util::right(m_healthBarLeftSprite);
        m_healthBarRect.top    = m_healthBarLeftSprite.getPosition().y;
        m_healthBarRect.width  = m_barFillMax;
        m_healthBarRect.height = m_healthBarMiddleSprite.getGlobalBounds().height;

        util::scaleAndCenterInside(m_healthBarMiddleSprite, m_healthBarRect);

        //

        m_barFillRightTexture.loadFromFile(
            (t_context.settings.media_path / "image/ui/bar-fill-right.png").string());

        TextureStats::instance().process(m_barFillRightTexture);

        m_healthBarRightSprite.setTexture(m_barFillRightTexture);
        m_healthBarRightSprite.scale(2.0f, 2.0f);
        m_healthBarRightSprite.setColor(m_healthColor);

        m_healthBarRightSprite.setPosition(
            util::right(m_healthBarMiddleSprite), m_healthBarMiddleSprite.getPosition().y);

        //

        m_manaBarLeftSprite.setTexture(m_barFillLeftTexture);
        m_manaBarLeftSprite.scale(2.0f, 2.0f);
        m_manaBarLeftSprite.setColor(m_manaColor);

        m_manaBarLeftSprite.setPosition(
            m_manaBarFrameSprite.getPosition() + sf::Vector2f(6.0f, 8.0f));

        //

        m_manaBarMiddleSprite.setTexture(m_barFillMiddleTexture);
        m_manaBarMiddleSprite.scale(2.0f, 2.0f);
        m_manaBarMiddleSprite.setColor(m_manaColor);

        m_manaBarRect.left   = util::right(m_manaBarLeftSprite);
        m_manaBarRect.top    = m_manaBarLeftSprite.getPosition().y;
        m_manaBarRect.width  = m_barFillMax;
        m_manaBarRect.height = m_manaBarMiddleSprite.getGlobalBounds().height;

        util::scaleAndCenterInside(m_manaBarMiddleSprite, m_manaBarRect);

        //

        m_manaBarRightSprite.setTexture(m_barFillRightTexture);
        m_manaBarRightSprite.scale(2.0f, 2.0f);
        m_manaBarRightSprite.setColor(m_manaColor);

        m_manaBarRightSprite.setPosition(
            util::right(m_manaBarMiddleSprite), m_manaBarMiddleSprite.getPosition().y);

        //

        const float starRectHeight{ m_healthBarFrameSprite.getPosition().y -
                                    m_fullFrameSprite.getPosition().y };

        const float starRectHeightMiddle{ m_healthBarFrameSprite.getPosition().y -
                                          (starRectHeight * 0.5f) };

        m_coinTexture.loadFromFile((t_context.settings.media_path / "image/ui/coin.png").string());
        TextureStats::instance().process(m_coinTexture);
        m_coinTexture.setSmooth(true);
        m_coinSprite.setTexture(m_coinTexture);
        m_coinSprite.scale(1.7f, 1.7f);

        m_coinSprite.setPosition(
            util::right(m_fullFrameSprite),
            (starRectHeightMiddle - (m_coinSprite.getGlobalBounds().height * 0.5f)));

        //

        m_coinText =
            t_context.font.makeText(Font::Default, FontSize::Large, "0", sf::Color(236, 218, 95));

        m_coinText.scale(1.4f, 1.4f);

        m_coinText.setPosition(
            (util::right(m_coinSprite) + 8.0f),
            (starRectHeightMiddle - (m_coinText.getGlobalBounds().height * 0.5f)));

        //

        m_starBrownTexture.loadFromFile(
            (t_context.settings.media_path / "image/ui/star-brown.png").string());

        TextureStats::instance().process(m_starBrownTexture);
        m_starBrownTexture.setSmooth(true);

        m_starBrown1Sprite.setTexture(m_starBrownTexture);
        m_starBrown1Sprite.scale(1.5f, 1.5f);

        m_starBrown1Sprite.setPosition(
            (m_fullFrameSprite.getPosition().x + 10.0f), util::bottom(m_halfFrameSprite));

        m_starBrown2Sprite.setTexture(m_starBrownTexture);
        m_starBrown2Sprite.scale(m_starBrown1Sprite.getScale());

        m_starBrown2Sprite.setPosition(
            util::right(m_starBrown1Sprite), m_starBrown1Sprite.getPosition().y);

        m_starBrown3Sprite.setTexture(m_starBrownTexture);
        m_starBrown3Sprite.scale(m_starBrown1Sprite.getScale());

        m_starBrown3Sprite.setPosition(
            util::right(m_starBrown2Sprite), m_starBrown1Sprite.getPosition().y);

        m_starBrown4Sprite.setTexture(m_starBrownTexture);
        m_starBrown4Sprite.scale(m_starBrown1Sprite.getScale());

        m_starBrown4Sprite.setPosition(
            util::right(m_starBrown3Sprite), m_starBrown1Sprite.getPosition().y);

        m_starBrown5Sprite.setTexture(m_starBrownTexture);
        m_starBrown5Sprite.scale(m_starBrown1Sprite.getScale());

        m_starBrown5Sprite.setPosition(
            util::right(m_starBrown4Sprite), m_starBrown1Sprite.getPosition().y);

        //

        m_starYellowTexture.loadFromFile(
            (t_context.settings.media_path / "image/ui/star-yellow.png").string());

        TextureStats::instance().process(m_starYellowTexture);
        m_starYellowTexture.setSmooth(true);

        m_starYellow1Sprite.setTexture(m_starYellowTexture);
        m_starYellow1Sprite.scale(m_starBrown1Sprite.getScale());

        m_starYellow2Sprite.setTexture(m_starYellowTexture);
        m_starYellow2Sprite.scale(m_starBrown1Sprite.getScale());

        m_starYellow3Sprite.setTexture(m_starYellowTexture);
        m_starYellow3Sprite.scale(m_starBrown1Sprite.getScale());

        m_starYellow4Sprite.setTexture(m_starYellowTexture);
        m_starYellow4Sprite.scale(m_starBrown1Sprite.getScale());

        m_starYellow5Sprite.setTexture(m_starYellowTexture);
        m_starYellow5Sprite.scale(m_starBrown1Sprite.getScale());

        setStarCount(0);
        //setHealthBar(0.0f);
        //setManaBar(0.0f);
    }

    void PlayerInfoDisplay::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_target.draw(m_halfFrameSprite, t_states);
        t_target.draw(&m_bgFadeVerts[0], m_bgFadeVerts.size(), sf::Quads, t_states);
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
        m_healthBarRect.width    = (m_barFillMax * t_ratio);
        util::scaleAndCenterInside(m_healthBarMiddleSprite, m_healthBarRect);
    }

    void PlayerInfoDisplay::setManaBar(const float t_ratio)
    {
        m_willDrawManaBarRight = (t_ratio >= 1.0f);
        m_willDrawManaBarLeft  = (t_ratio > 0.0f);
        m_manaBarRect.width    = (m_barFillMax * t_ratio);
        util::scaleAndCenterInside(m_manaBarMiddleSprite, m_manaBarRect);
    }

    void PlayerInfoDisplay::setCoinCount(const Coin_t t_count)
    {
        m_coinText.setString(std::to_string(t_count));
        util::setOriginToPosition(m_coinText);
    }

    void PlayerInfoDisplay::setStarCount(const int t_count)
    {
        m_starYellow1Sprite.setPosition(-100.0f, -100.0f);
        m_starYellow2Sprite.setPosition(-100.0f, -100.0f);
        m_starYellow3Sprite.setPosition(-100.0f, -100.0f);
        m_starYellow4Sprite.setPosition(-100.0f, -100.0f);
        m_starYellow5Sprite.setPosition(-100.0f, -100.0f);

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

} // namespace platformer
