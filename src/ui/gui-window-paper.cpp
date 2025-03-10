// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// gui-window-paper.cpp
//
#include "gui-window-paper.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/font.hpp"
#include "subsystem/screen-layout.hpp"
#include "ui/text-layout.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    GuiWindowPaper::GuiWindowPaper()
        : m_info{}
        , m_innerRect{}
        , m_outerRect{}
        , m_paper1Texture{}
        , m_paper2Texture{}
        , m_tapeLeftTexture{}
        , m_tapeRightTexture{}
        , m_tapeMiddleTexture{}
        , m_paperSprite{ m_paper1Texture }
        , m_tapeLeftSprite{ m_tapeLeftTexture }
        , m_tapeRightSprite{ m_tapeRightTexture }
        , m_tapeMiddleSprite{ m_tapeMiddleTexture }
        , m_titleText{ util::SfmlDefaults::instance().font() }
        , m_contentTexts{}
        , m_bgFadeVerts{}
    {
        m_bgFadeVerts.reserve(util::verts_per_quad);
    }

    void GuiWindowPaper::setup(const Settings & t_settings)
    {
        // TODO um...should we always be reloading these big textures?

        util::TextureLoader::load(m_paper1Texture, (t_settings.media_path / "image/ui/paper1.png"));
        util::TextureLoader::load(m_paper2Texture, (t_settings.media_path / "image/ui/paper2.png"));

        util::TextureLoader::load(
            m_tapeLeftTexture, (t_settings.media_path / "image/ui/tape-left.png"));

        util::TextureLoader::load(
            m_tapeRightTexture, (t_settings.media_path / "image/ui/tape-right.png"));

        util::TextureLoader::load(
            m_tapeMiddleTexture, (t_settings.media_path / "image/ui/tape-middle.png"));
    }

    void GuiWindowPaper::create(
        Context & t_context, const bool t_useBigPaper, const GuiWindowInfo & t_info)
    {
        m_bgFadeVerts.clear();

        util::appendTriangleVerts(
            t_context.layout.wholeRect(), m_bgFadeVerts, sf::Color(0, 0, 0, 127));

        m_info = t_info;

        const sf::Vector2f wholeSize{ t_context.layout.wholeSize() };

        if (t_useBigPaper)
        {
            m_paperSprite.setTexture(m_paper2Texture);

            m_paperSprite.setPosition(
                { ((wholeSize.x * 0.5f) - (m_paperSprite.getGlobalBounds().size.x * 0.5f)),
                  ((wholeSize.y * 0.5f) - (m_paperSprite.getGlobalBounds().size.y * 0.5f)) });

            m_innerRect.position.x = (m_paperSprite.getPosition().x + 32.0f);
            m_innerRect.position.y = (m_paperSprite.getPosition().y + 32.0f);
            m_innerRect.size.x     = 432.0f;
            m_innerRect.size.y     = 256.0f;
        }
        else
        {
            m_paperSprite.setTexture(m_paper1Texture);

            m_paperSprite.setPosition(
                { ((wholeSize.x * 0.5f) - (m_paperSprite.getGlobalBounds().size.x * 0.5f)),
                  ((wholeSize.y * 0.5f) - (m_paperSprite.getGlobalBounds().size.y * 0.5f)) });

            m_innerRect.position.x = (m_paperSprite.getPosition().x + 48.0f);
            m_innerRect.position.y = (m_paperSprite.getPosition().y + 48.0f);
            m_innerRect.size.x     = 410.0f;
            m_innerRect.size.y     = 410.0f;
        }

        m_outerRect = m_paperSprite.getGlobalBounds();

        if (!m_info.title.empty())
        {
            m_tapeLeftSprite.setTexture(m_tapeLeftTexture);

            m_tapeLeftSprite.setPosition(
                { ((wholeSize.x * 0.5f) - m_tapeLeftSprite.getGlobalBounds().size.x),
                  (m_outerRect.position.y - (m_tapeLeftSprite.getGlobalBounds().size.y * 0.65f)) });

            m_tapeRightSprite.setTexture(m_tapeRightTexture);

            m_tapeRightSprite.setPosition(
                { (wholeSize.x * 0.5f),
                  (m_outerRect.position.y -
                   (m_tapeRightSprite.getGlobalBounds().size.y * 0.65f)) });

            sf::FloatRect titleRect;
            titleRect.position.x = ((wholeSize.x * 0.5f) - 100.0f);
            titleRect.position.y = (m_tapeLeftSprite.getPosition().y + 14.0f);
            titleRect.size.x     = 200.0f;
            titleRect.size.y     = 37.0f;

            m_titleText = t_context.font.makeText(
                Font::Title, FontSize::Medium, m_info.title, sf::Color(32, 32, 32));

            util::fitAndCenterInside(m_titleText, titleRect);

            const float betweenTapeSize{ m_titleText.getGlobalBounds().size.x - titleRect.size.x };

            if (betweenTapeSize > 0.0f)
            {
                m_tapeLeftSprite.move({ -(betweenTapeSize * 0.5f), 0.0f });
                m_tapeRightSprite.move({ (betweenTapeSize * 0.5f), 0.0f });

                titleRect.position.x -= (betweenTapeSize * 0.5f);
                titleRect.size.x += betweenTapeSize;

                util::fitAndCenterInside(m_titleText, titleRect);

                m_tapeMiddleSprite.setTexture(m_tapeMiddleTexture);

                sf::FloatRect tapeMiddleRect;

                tapeMiddleRect.position.x =
                    ((t_context.layout.wholeSize().x * 0.5f) - (betweenTapeSize * 0.5f));

                tapeMiddleRect.position.y = (m_tapeLeftSprite.getPosition().y + 8.0f);
                tapeMiddleRect.size.x     = betweenTapeSize;
                tapeMiddleRect.size.y     = m_tapeMiddleSprite.getGlobalBounds().size.y;

                util::scaleAndCenterInside(m_tapeMiddleSprite, tapeMiddleRect);
            }

            m_outerRect.size.x +=
                ((m_outerRect.position.x - m_tapeLeftSprite.getPosition().x) * 2.0f);

            m_outerRect.position.x = m_tapeLeftSprite.getPosition().x;

            m_outerRect.size.y += (m_outerRect.position.y - m_tapeLeftSprite.getPosition().y);
            m_outerRect.position.y = m_tapeLeftSprite.getPosition().y;
        }

        m_contentTexts = TextLayout::layout(t_context, m_info.content, m_innerRect, m_info.details);
    }

    void GuiWindowPaper::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        if (!m_bgFadeVerts.empty())
        {
            t_target.draw(
                &m_bgFadeVerts[0], m_bgFadeVerts.size(), sf::PrimitiveType::Triangles, t_states);
        }

        t_target.draw(m_paperSprite, t_states);
        t_target.draw(m_tapeLeftSprite, t_states);
        t_target.draw(m_tapeRightSprite, t_states);
        t_target.draw(m_tapeMiddleSprite, t_states);
        t_target.draw(m_titleText, t_states);

        for (const sf::Text & text : m_contentTexts)
        {
            t_target.draw(text, t_states);
        }
    }

} // namespace bramblefore
