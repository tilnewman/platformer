// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// gui-window-paper.cpp
//
#include "gui-window-paper.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "font.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "text-layout.hpp"
#include "texture-stats.hpp"

namespace platformer
{

    GuiWindowPaper::GuiWindowPaper()
        : m_info()
        , m_innerRect()
        , m_outerRect()
        , m_paper1Texture()
        , m_paper2Texture()
        , m_tapeLeftTexture()
        , m_tapeRightTexture()
        , m_tapeMiddleTexture()
        , m_paperSprite()
        , m_tapeLeftSprite()
        , m_tapeRightSprite()
        , m_tapeMiddleSprite()
        , m_titleText()
        , m_contentTexts()
        , m_bgFadeVerts()
    {
        m_bgFadeVerts.reserve(util::verts_per_quad);
    }

    void GuiWindowPaper::setup(const Settings & settings)
    {
        m_paper1Texture.loadFromFile((settings.media_path / "image/ui/paper1.png").string());
        m_paper2Texture.loadFromFile((settings.media_path / "image/ui/paper2.png").string());

        TextureStats::instance().process(m_paper1Texture);
        TextureStats::instance().process(m_paper2Texture);

        m_tapeLeftTexture.loadFromFile((settings.media_path / "image/ui/tape-left.png").string());
        m_tapeRightTexture.loadFromFile((settings.media_path / "image/ui/tape-right.png").string());

        m_tapeMiddleTexture.loadFromFile(
            (settings.media_path / "image/ui/tape-middle.png").string());

        TextureStats::instance().process(m_tapeLeftTexture);
        TextureStats::instance().process(m_tapeRightTexture);
        TextureStats::instance().process(m_tapeMiddleTexture);
    }

    void GuiWindowPaper::create(
        Context & context, const bool useBigPaper, const GuiWindowInfo & info)
    {
        m_bgFadeVerts.clear();
        util::appendQuadVerts(context.layout.wholeRect(), m_bgFadeVerts, sf::Color(0, 0, 0, 127));

        m_info = info;

        if (useBigPaper)
        {
            m_paperSprite.setTexture(m_paper2Texture);

            m_paperSprite.setPosition(
                ((context.layout.wholeSize().x * 0.5f) -
                 (m_paperSprite.getGlobalBounds().width * 0.5f)),
                ((context.layout.wholeSize().y * 0.5f) -
                 (m_paperSprite.getGlobalBounds().height * 0.5f)));

            m_innerRect.left   = (m_paperSprite.getPosition().x + 32.0f);
            m_innerRect.top    = (m_paperSprite.getPosition().y + 32.0f);
            m_innerRect.width  = 432.0f;
            m_innerRect.height = 256.0f;
        }
        else
        {
            m_paperSprite.setTexture(m_paper1Texture);

            m_paperSprite.setPosition(
                ((context.layout.wholeSize().x * 0.5f) -
                 (m_paperSprite.getGlobalBounds().width * 0.5f)),
                ((context.layout.wholeSize().y * 0.5f) -
                 (m_paperSprite.getGlobalBounds().height * 0.5f)));

            m_innerRect.left   = (m_paperSprite.getPosition().x + 48.0f);
            m_innerRect.top    = (m_paperSprite.getPosition().y + 48.0f);
            m_innerRect.width  = 410.0f;
            m_innerRect.height = 410.0f;
        }

        m_outerRect = m_paperSprite.getGlobalBounds();

        if (!m_info.title.empty())
        {
            m_tapeLeftSprite.setTexture(m_tapeLeftTexture);

            m_tapeLeftSprite.setPosition(
                ((context.layout.wholeSize().x * 0.5f) - m_tapeLeftSprite.getGlobalBounds().width),
                (m_outerRect.top - (m_tapeLeftSprite.getGlobalBounds().height * 0.65f)));

            m_tapeRightSprite.setTexture(m_tapeRightTexture);

            m_tapeRightSprite.setPosition(
                (context.layout.wholeSize().x * 0.5f),
                (m_outerRect.top - (m_tapeRightSprite.getGlobalBounds().height * 0.65f)));

            sf::FloatRect titleRect;
            titleRect.left   = ((context.layout.wholeSize().x * 0.5f) - 100.0f);
            titleRect.top    = (m_tapeLeftSprite.getPosition().y + 14.0f);
            titleRect.width  = 200.0f;
            titleRect.height = 37.0f;

            m_titleText = context.font.makeText(
                Font::Default, FontSize::Medium, m_info.title, sf::Color(32, 32, 32));

            util::fitAndCenterInside(m_titleText, titleRect);

            const float betweenTapeSize{ m_titleText.getGlobalBounds().width - titleRect.width };

            if (betweenTapeSize > 0.0f)
            {
                m_tapeLeftSprite.move(-(betweenTapeSize * 0.5f), 0.0f);
                m_tapeRightSprite.move((betweenTapeSize * 0.5f), 0.0f);

                titleRect.left -= (betweenTapeSize * 0.5f);
                titleRect.width += betweenTapeSize;

                util::fitAndCenterInside(m_titleText, titleRect);

                m_tapeMiddleSprite.setTexture(m_tapeMiddleTexture);
                sf::FloatRect tapeMiddleRect;

                tapeMiddleRect.left =
                    ((context.layout.wholeSize().x * 0.5f) - (betweenTapeSize * 0.5f));

                tapeMiddleRect.top    = (m_tapeLeftSprite.getPosition().y + 8.0f);
                tapeMiddleRect.width  = betweenTapeSize;
                tapeMiddleRect.height = m_tapeMiddleSprite.getGlobalBounds().height;

                util::scaleAndCenterInside(m_tapeMiddleSprite, tapeMiddleRect);
            }

            m_outerRect.width += ((m_outerRect.left - m_tapeLeftSprite.getPosition().x) * 2.0f);
            m_outerRect.left = m_tapeLeftSprite.getPosition().x;

            m_outerRect.height += (m_outerRect.top - m_tapeLeftSprite.getPosition().y);
            m_outerRect.top = m_tapeLeftSprite.getPosition().y;
        }

        m_contentTexts = TextLayout::layout(context, m_info.content, m_innerRect, m_info.details);
    }

    void GuiWindowPaper::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(&m_bgFadeVerts[0], m_bgFadeVerts.size(), sf::Quads, states);
        target.draw(m_paperSprite, states);
        target.draw(m_tapeLeftSprite, states);
        target.draw(m_tapeRightSprite, states);
        target.draw(m_tapeMiddleSprite, states);
        target.draw(m_titleText, states);

        for (const sf::Text & text : m_contentTexts)
        {
            target.draw(text, states);
        }
    }

} // namespace platformer
