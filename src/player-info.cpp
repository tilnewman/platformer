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
        , m_barFrameSprite()
        , m_barFillRect()
        , m_barFillLeftTexture()
        , m_barFillLeftSprite()
        , m_barFillMiddleTexture()
        , m_barFillMiddleSprite()
        , m_barFillRightTexture()
        , m_barFillRightSprite()
    {}

    void PlayerInfoDisplay::setup(const Context & context)
    {
        m_halfFrameTexture.loadFromFile(
            (context.settings.media_path / "image/ui/half-frame.png").string());

        TextureStats::instance().process(m_halfFrameTexture);
        m_halfFrameSprite.setTexture(m_halfFrameTexture);
        m_halfFrameSprite.scale(2.0f, 2.0f);

        const float posDimm{ context.layout.wholeSize().y * 0.175f };
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

        m_barFrameSprite.setTexture(m_barFrameTexture);
        m_barFrameSprite.scale(2.0f, 2.0f);

        m_barFrameSprite.setPosition(
            (util::right(m_fullFrameSprite) - 2.0f),
            (util::bottom(m_fullFrameSprite) - m_barFrameSprite.getGlobalBounds().height));

        //

        m_barFillLeftTexture.loadFromFile(
            (context.settings.media_path / "image/ui/bar-fill-left.png").string());

        TextureStats::instance().process(m_barFillLeftTexture);

        m_barFillLeftSprite.setTexture(m_barFillLeftTexture);
        m_barFillLeftSprite.scale(2.0f, 2.0f);
        m_barFillLeftSprite.setColor(sf::Color::Red);

        m_barFillLeftSprite.setPosition(m_barFrameSprite.getPosition() + sf::Vector2f(6.0f, 8.0f));

        //

        m_barFillMiddleTexture.loadFromFile(
            (context.settings.media_path / "image/ui/bar-fill-middle.png").string());

        TextureStats::instance().process(m_barFillMiddleTexture);

        m_barFillMiddleSprite.setTexture(m_barFillMiddleTexture);
        m_barFillMiddleSprite.scale(2.0f, 2.0f);
        m_barFillMiddleSprite.setColor(sf::Color::Red);

        m_barFillRect.left   = util::right(m_barFillLeftSprite);
        m_barFillRect.top    = m_barFillLeftSprite.getPosition().y;
        m_barFillRect.width  = 226.0f;
        m_barFillRect.height = m_barFillMiddleSprite.getGlobalBounds().height;

        util::scaleAndCenterInside(m_barFillMiddleSprite, m_barFillRect);

        //

        m_barFillRightTexture.loadFromFile(
            (context.settings.media_path / "image/ui/bar-fill-right.png").string());

        TextureStats::instance().process(m_barFillRightTexture);

        m_barFillRightSprite.setTexture(m_barFillRightTexture);
        m_barFillRightSprite.scale(2.0f, 2.0f);
        m_barFillRightSprite.setColor(sf::Color::Red);

        m_barFillRightSprite.setPosition(
            util::right(m_barFillMiddleSprite), m_barFillMiddleSprite.getPosition().y);
    }

    void PlayerInfoDisplay::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_halfFrameSprite, states);
        target.draw(&m_bgFadeVerts[0], m_bgFadeVerts.size(), sf::Quads, states);
        target.draw(m_avatarIconSprite, states);
        target.draw(m_fullFrameSprite, states);
        target.draw(m_barFrameSprite, states);
        target.draw(m_barFillLeftSprite, states);
        target.draw(m_barFillMiddleSprite, states);
        target.draw(m_barFillRightSprite, states);
    }

} // namespace platformer
