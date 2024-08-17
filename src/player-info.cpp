// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// player-info.hpp
//
#include "player-info.hpp"

#include "avatar-textures.hpp"
#include "avatar.hpp"
#include "context.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

#include <filesystem>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    PlayerInfoDisplay::PlayerInfoDisplay()
        : m_halfFrameTexture()
        , m_halfFrameSprite()
        , m_fullFrameTexture()
        , m_fullFrameSprite()
        , m_avatarIconSprite()
        , m_bgFadeVerts()
        , m_barFrameTexture()
        , m_healthBarFrameSprite()
        , m_manaBarFrameSprite()
        , m_barFillLeftTexture()
        , m_barFillMiddleTexture()
        , m_barFillRightTexture()
        , m_healthBarRect()
        , m_healthBarLeftSprite()
        , m_healthBarMiddleSprite()
        , m_healthBarRightSprite()
        , m_manaBarRect()
        , m_manaBarLeftSprite()
        , m_manaBarMiddleSprite()
        , m_manaBarRightSprite()
        , m_healthColor(255, 100, 100)
        , m_manaColor(100, 100, 255)
    {}

    void PlayerInfoDisplay::setup(const Context & context)
    {
        m_halfFrameTexture.loadFromFile(
            (context.settings.media_path / "image/ui/half-frame.png").string());

        TextureStats::instance().process(m_halfFrameTexture);
        m_halfFrameSprite.setTexture(m_halfFrameTexture);
        m_halfFrameSprite.scale(2.0f, 2.0f);

        const float posDimm{ context.layout.wholeSize().y * 0.125f };
        m_halfFrameSprite.setPosition(posDimm, posDimm);

        //

        m_fullFrameTexture.loadFromFile(
            (context.settings.media_path / "image/ui/full-frame.png").string());

        TextureStats::instance().process(m_fullFrameTexture);
        m_fullFrameSprite.setTexture(m_fullFrameTexture);
        m_fullFrameSprite.scale(2.0f, 2.0f);

        m_fullFrameSprite.setPosition(m_halfFrameSprite.getPosition() + sf::Vector2f(5.0f, 5.0f));

        //

        const sf::FloatRect m_bgRect = m_fullFrameSprite.getGlobalBounds();
        util::appendQuadVerts(m_bgRect, m_bgFadeVerts, sf::Color::Black);
        m_bgFadeVerts.at(0).color = sf::Color(64, 64, 64);
        m_bgFadeVerts.at(1).color = m_bgFadeVerts.at(0).color;

        //

        m_avatarIconSprite.setTexture(context.avatar_texture.iconTexture(context.avatar.type()));
        util::scaleAndCenterInside(m_avatarIconSprite, m_bgRect);

        //

        m_barFrameTexture.loadFromFile(
            (context.settings.media_path / "image/ui/bar-frame.png").string());

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
            (context.settings.media_path / "image/ui/bar-fill-left.png").string());

        TextureStats::instance().process(m_barFillLeftTexture);

        m_healthBarLeftSprite.setTexture(m_barFillLeftTexture);
        m_healthBarLeftSprite.scale(2.0f, 2.0f);
        m_healthBarLeftSprite.setColor(m_healthColor);

        m_healthBarLeftSprite.setPosition(
            m_healthBarFrameSprite.getPosition() + sf::Vector2f(6.0f, 8.0f));

        //

        m_barFillMiddleTexture.loadFromFile(
            (context.settings.media_path / "image/ui/bar-fill-middle.png").string());

        TextureStats::instance().process(m_barFillMiddleTexture);

        m_healthBarMiddleSprite.setTexture(m_barFillMiddleTexture);
        m_healthBarMiddleSprite.scale(2.0f, 2.0f);
        m_healthBarMiddleSprite.setColor(m_healthColor);

        m_healthBarRect.left   = util::right(m_healthBarLeftSprite);
        m_healthBarRect.top    = m_healthBarLeftSprite.getPosition().y;
        m_healthBarRect.width  = 226.0f;
        m_healthBarRect.height = m_healthBarMiddleSprite.getGlobalBounds().height;

        util::scaleAndCenterInside(m_healthBarMiddleSprite, m_healthBarRect);

        //

        m_barFillRightTexture.loadFromFile(
            (context.settings.media_path / "image/ui/bar-fill-right.png").string());

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
        m_manaBarRect.width  = 226.0f;
        m_manaBarRect.height = m_manaBarMiddleSprite.getGlobalBounds().height;

        util::scaleAndCenterInside(m_manaBarMiddleSprite, m_manaBarRect);

        //

        m_manaBarRightSprite.setTexture(m_barFillRightTexture);
        m_manaBarRightSprite.scale(2.0f, 2.0f);
        m_manaBarRightSprite.setColor(m_manaColor);

        m_manaBarRightSprite.setPosition(
            util::right(m_manaBarMiddleSprite), m_manaBarMiddleSprite.getPosition().y);
    }

    void PlayerInfoDisplay::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_halfFrameSprite, states);
        target.draw(&m_bgFadeVerts[0], m_bgFadeVerts.size(), sf::Quads, states);
        target.draw(m_avatarIconSprite, states);
        target.draw(m_fullFrameSprite, states);

        target.draw(m_healthBarFrameSprite, states);
        target.draw(m_healthBarLeftSprite, states);
        target.draw(m_healthBarMiddleSprite, states);
        target.draw(m_healthBarRightSprite, states);

        target.draw(m_manaBarFrameSprite, states);
        target.draw(m_manaBarLeftSprite, states);
        target.draw(m_manaBarMiddleSprite, states);
        target.draw(m_manaBarRightSprite, states);
    }

} // namespace platformer
