// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// gui-window.cpp
//
#include "gui-window.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

namespace platformer
{

    GuiWindow::GuiWindow()
        : m_innerRect()
        , m_outerRect()
        , m_borderTopLeftTexture()
        , m_borderTopRightTexture()
        , m_borderBotLeftTexture()
        , m_borderBotRightTexture()
        , m_borderTopTexture()
        , m_borderBotTexture()
        , m_borderLeftTexture()
        , m_borderRightTexture()
        , m_bgTopLeftTexture()
        , m_bgTopRightTexture()
        , m_bgBotLeftTexture()
        , m_bgBotRightTexture()
        , m_bgTopTexture()
        , m_bgBotTexture()
        , m_bgLeftTexture()
        , m_bgRightTexture()
        , m_tapeLeftTexture()
        , m_tapeRightTexture()
        , m_tapeMiddleTexture()
        , m_borderTopLeftSprite()
        , m_borderTopRightSprite()
        , m_borderBotLeftSprite()
        , m_borderBotRightSprite()
        , m_borderTopSprite()
        , m_borderBotSprite()
        , m_borderLeftSprite()
        , m_borderRightSprite()
        , m_bgTopLeftSprite()
        , m_bgTopRightSprite()
        , m_bgBotLeftSprite()
        , m_bgBotRightSprite()
        , m_bgTopSprite()
        , m_bgBotSprite()
        , m_bgLeftSprite()
        , m_bgRightSprite()
        , m_tapeLeftSprite()
        , m_tapeRightSprite()
        , m_tapeMiddleSprite()
        , m_bgColor(74, 76, 41)
        , m_bgCenterRect()
        , m_bgCenterVerts()
    {
        m_bgCenterVerts.reserve(util::verts_per_quad);
    }

    void GuiWindow::setup(const Settings & settings)
    {
        m_borderTopLeftTexture.loadFromFile(
            (settings.media_path / "image/ui/border-top-left.png").string());

        m_borderTopRightTexture.loadFromFile(
            (settings.media_path / "image/ui/border-top-right.png").string());

        m_borderBotLeftTexture.loadFromFile(
            (settings.media_path / "image/ui/border-bottom-left.png").string());

        m_borderBotRightTexture.loadFromFile(
            (settings.media_path / "image/ui/border-bottom-right.png").string());

        m_borderTopTexture.loadFromFile((settings.media_path / "image/ui/border-top.png").string());

        m_borderBotTexture.loadFromFile(
            (settings.media_path / "image/ui/border-bottom.png").string());

        m_borderLeftTexture.loadFromFile(
            (settings.media_path / "image/ui/border-left.png").string());

        m_borderRightTexture.loadFromFile(
            (settings.media_path / "image/ui/border-right.png").string());

        TextureStats::instance().process(m_borderTopLeftTexture);
        TextureStats::instance().process(m_borderTopRightTexture);
        TextureStats::instance().process(m_borderBotLeftTexture);
        TextureStats::instance().process(m_borderBotRightTexture);
        TextureStats::instance().process(m_borderTopTexture);
        TextureStats::instance().process(m_borderBotTexture);
        TextureStats::instance().process(m_borderLeftTexture);
        TextureStats::instance().process(m_borderRightTexture);

        //

        m_bgTopLeftTexture.loadFromFile(
            (settings.media_path / "image/ui/menu-bg-top-left.png").string());

        m_bgTopRightTexture.loadFromFile(
            (settings.media_path / "image/ui/menu-bg-top-right.png").string());

        m_bgBotLeftTexture.loadFromFile(
            (settings.media_path / "image/ui/menu-bg-bottom-left.png").string());

        m_bgBotRightTexture.loadFromFile(
            (settings.media_path / "image/ui/menu-bg-bottom-right.png").string());

        m_bgTopTexture.loadFromFile((settings.media_path / "image/ui/menu-bg-top.png").string());
        m_bgBotTexture.loadFromFile((settings.media_path / "image/ui/menu-bg-bottom.png").string());
        m_bgLeftTexture.loadFromFile((settings.media_path / "image/ui/menu-bg-left.png").string());

        m_bgRightTexture.loadFromFile(
            (settings.media_path / "image/ui/menu-bg-right.png").string());

        TextureStats::instance().process(m_bgTopLeftTexture);
        TextureStats::instance().process(m_bgTopRightTexture);
        TextureStats::instance().process(m_bgBotLeftTexture);
        TextureStats::instance().process(m_bgBotRightTexture);
        TextureStats::instance().process(m_bgTopTexture);
        TextureStats::instance().process(m_bgBotTexture);
        TextureStats::instance().process(m_bgLeftTexture);
        TextureStats::instance().process(m_bgRightTexture);

        //

        m_tapeLeftTexture.loadFromFile((settings.media_path / "image/ui/tape-left.png").string());
        m_tapeRightTexture.loadFromFile((settings.media_path / "image/ui/tape-right.png").string());

        m_tapeMiddleTexture.loadFromFile(
            (settings.media_path / "image/ui/tape-middle.png").string());

        TextureStats::instance().process(m_tapeLeftTexture);
        TextureStats::instance().process(m_tapeRightTexture);
        TextureStats::instance().process(m_tapeMiddleTexture);

        //

        m_borderTopLeftSprite.setTexture(m_borderTopLeftTexture);
        m_borderTopRightSprite.setTexture(m_borderTopRightTexture);
        m_borderBotLeftSprite.setTexture(m_borderBotLeftTexture);
        m_borderBotRightSprite.setTexture(m_borderBotRightTexture);
        m_borderTopSprite.setTexture(m_borderTopTexture);
        m_borderBotSprite.setTexture(m_borderBotTexture);
        m_borderLeftSprite.setTexture(m_borderLeftTexture);
        m_borderRightSprite.setTexture(m_borderRightTexture);

        //

        m_bgTopLeftSprite.setTexture(m_bgTopLeftTexture);
        m_bgTopRightSprite.setTexture(m_bgTopRightTexture);
        m_bgBotLeftSprite.setTexture(m_bgBotLeftTexture);
        m_bgBotRightSprite.setTexture(m_bgBotRightTexture);
        m_bgTopSprite.setTexture(m_bgTopTexture);
        m_bgBotSprite.setTexture(m_bgBotTexture);
        m_bgLeftSprite.setTexture(m_bgLeftTexture);
        m_bgRightSprite.setTexture(m_bgRightTexture);

        //

        m_tapeLeftSprite.setTexture(m_tapeLeftTexture);
        m_tapeRightSprite.setTexture(m_tapeRightTexture);
        m_tapeMiddleSprite.setTexture(m_tapeMiddleTexture);
    }

    void GuiWindow::create(const sf::Vector2f &, const std::string &)
    {
        m_borderTopLeftSprite.setPosition(100.0f, 100.0f);

        m_borderTopSprite.setPosition(
            util::right(m_borderTopLeftSprite), (m_borderTopLeftSprite.getPosition().y + 4.0f));

        m_borderTopRightSprite.setPosition(
            util::right(m_borderTopSprite), m_borderTopLeftSprite.getPosition().y);

        m_borderLeftSprite.setPosition(
            (m_borderTopLeftSprite.getPosition().x + 3.0f), util::bottom(m_borderTopLeftSprite));

        m_borderRightSprite.setPosition(
            (util::right(m_borderTopRightSprite) - 14.0f), util::bottom(m_borderTopRightSprite));

        m_borderBotLeftSprite.setPosition(
            m_borderTopLeftSprite.getPosition().x, util::bottom(m_borderLeftSprite));

        m_borderBotSprite.setPosition(
            m_borderTopSprite.getPosition().x, (util::bottom(m_borderBotLeftSprite) - 15.0f));

        m_borderBotRightSprite.setPosition(
            m_borderTopRightSprite.getPosition().x, m_borderBotLeftSprite.getPosition().y);

        //

        m_bgTopLeftSprite.setPosition(100.0f, 100.0f);

        m_bgTopSprite.setPosition(
            util::right(m_bgTopLeftSprite), m_bgTopLeftSprite.getPosition().y);

        m_bgTopRightSprite.setPosition(
            util::right(m_bgTopSprite), m_bgTopLeftSprite.getPosition().y);

        m_bgLeftSprite.setPosition(
            m_bgTopLeftSprite.getPosition().x, util::bottom(m_bgTopLeftSprite));

        m_bgRightSprite.setPosition(
            m_bgTopRightSprite.getPosition().x, util::bottom(m_bgTopRightSprite));

        m_bgBotLeftSprite.setPosition(
            m_bgTopLeftSprite.getPosition().x, util::bottom(m_bgLeftSprite));

        m_bgBotSprite.setPosition(
            util::right(m_bgBotLeftSprite), m_bgBotLeftSprite.getPosition().y);

        m_bgBotRightSprite.setPosition(
            m_bgTopRightSprite.getPosition().x, util::bottom(m_bgRightSprite));

        //

        m_bgCenterRect.left = util::right(m_bgLeftSprite);
        m_bgCenterRect.top  = util::bottom(m_bgTopSprite);

        m_bgCenterRect.width =
            (m_bgTopRightSprite.getPosition().x - util::right(m_bgTopLeftSprite));

        m_bgCenterRect.height =
            (m_bgBotLeftSprite.getPosition().y - util::bottom(m_bgTopLeftSprite));

        util::appendQuadVerts(m_bgCenterRect, m_bgCenterVerts, m_bgColor);
    }

    void GuiWindow::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_bgTopLeftSprite, states);
        target.draw(m_bgTopSprite, states);
        target.draw(m_bgTopRightSprite, states);
        target.draw(m_bgLeftSprite, states);
        target.draw(m_bgRightSprite, states);
        target.draw(m_bgBotLeftSprite, states);
        target.draw(m_bgBotSprite, states);
        target.draw(m_bgBotRightSprite, states);
        target.draw(&m_bgCenterVerts[0], m_bgCenterVerts.size(), sf::Quads, states);
    }

} // namespace platformer
