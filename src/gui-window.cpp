// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// gui-window.cpp
//
#include "gui-window.hpp"

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

    GuiWindow::GuiWindow()
        : m_info()
        , m_innerRect()
        , m_outerRect()
        , m_borderTopLeftTexture()
        , m_borderTopRightTexture()
        , m_borderBotLeftTexture()
        , m_borderBotRightTexture()
        , m_borderTopTexture()
        , m_borderBotTexture()
        , m_borderLeftTexture()
        , m_borderRightTexture()
        , m_smallBorderTopLeftTexture()
        , m_smallBorderTopRightTexture()
        , m_smallBorderBotLeftTexture()
        , m_smallBorderBotRightTexture()
        , m_smallBorderTopTexture()
        , m_smallBorderBotTexture()
        , m_smallBorderLeftTexture()
        , m_smallBorderRightTexture()
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
        , m_tapeLeftSprite()
        , m_tapeRightSprite()
        , m_tapeMiddleSprite()
        , m_bgColor(74, 76, 41)
        , m_bgCenterRect()
        , m_bgCenterVerts()
        , m_sprites()
        , m_titleText()
        , m_contentTexts()
    {
        m_bgCenterVerts.reserve(util::verts_per_quad);
        m_sprites.reserve(16);
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

        m_smallBorderTopLeftTexture.loadFromFile(
            (settings.media_path / "image/ui/small-border-top-left.png").string());

        m_smallBorderTopRightTexture.loadFromFile(
            (settings.media_path / "image/ui/small-border-top-right.png").string());

        m_smallBorderBotLeftTexture.loadFromFile(
            (settings.media_path / "image/ui/small-border-bottom-left.png").string());

        m_smallBorderBotRightTexture.loadFromFile(
            (settings.media_path / "image/ui/small-border-bottom-right.png").string());

        m_smallBorderTopTexture.loadFromFile(
            (settings.media_path / "image/ui/small-border-top.png").string());

        m_smallBorderBotTexture.loadFromFile(
            (settings.media_path / "image/ui/small-border-bottom.png").string());

        m_smallBorderLeftTexture.loadFromFile(
            (settings.media_path / "image/ui/small-border-left.png").string());

        m_smallBorderRightTexture.loadFromFile(
            (settings.media_path / "image/ui/small-border-right.png").string());

        TextureStats::instance().process(m_smallBorderTopLeftTexture);
        TextureStats::instance().process(m_smallBorderTopRightTexture);
        TextureStats::instance().process(m_smallBorderBotLeftTexture);
        TextureStats::instance().process(m_smallBorderBotRightTexture);
        TextureStats::instance().process(m_smallBorderTopTexture);
        TextureStats::instance().process(m_smallBorderBotTexture);
        TextureStats::instance().process(m_smallBorderLeftTexture);
        TextureStats::instance().process(m_smallBorderRightTexture);

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

        m_tapeLeftSprite.setTexture(m_tapeLeftTexture);
        m_tapeRightSprite.setTexture(m_tapeRightTexture);
        m_tapeMiddleSprite.setTexture(m_tapeMiddleTexture);
    }

    void GuiWindow::create(Context & context, const GuiWindowInfo & info)
    {
        m_sprites.clear();

        m_info = info;

        sf::Vector2f betweenCornerSize;

        betweenCornerSize.x = (m_info.inner_size.x - 2.0f);
        if (betweenCornerSize.x < 0.0f)
        {
            betweenCornerSize.x = 0.0f;
        }

        betweenCornerSize.y = (m_info.inner_size.y - 2.0f);
        if (betweenCornerSize.y < 0.0f)
        {
            betweenCornerSize.y = 0.0f;
        }

        sf::Sprite & bgTopLeftSprite{ m_sprites.emplace_back(m_bgTopLeftTexture) };

        const sf::Vector2f totalSize{
            (betweenCornerSize.x + (bgTopLeftSprite.getGlobalBounds().width * 2.0f)),
            (betweenCornerSize.y + (bgTopLeftSprite.getGlobalBounds().height * 2.0f))
        };

        bgTopLeftSprite.setPosition((context.layout.wholeSize() * 0.5f) - (totalSize * 0.5f));

        if (betweenCornerSize.x > 0.0f)
        {
            sf::FloatRect bgTopRect;
            bgTopRect.left   = util::right(bgTopLeftSprite);
            bgTopRect.top    = bgTopLeftSprite.getPosition().y;
            bgTopRect.width  = betweenCornerSize.x;
            bgTopRect.height = bgTopLeftSprite.getGlobalBounds().height;

            sf::Sprite & bgTopSprite{ m_sprites.emplace_back(m_bgTopTexture) };
            util::scaleAndCenterInside(bgTopSprite, bgTopRect);

            sf::FloatRect bgBotRect;
            bgBotRect.left   = bgTopRect.left;
            bgBotRect.top    = (util::bottom(bgTopLeftSprite) + betweenCornerSize.y);
            bgBotRect.width  = bgTopRect.width;
            bgBotRect.height = bgTopRect.height;

            sf::Sprite & bgBotSprite{ m_sprites.emplace_back(m_bgBotTexture) };
            util::scaleAndCenterInside(bgBotSprite, bgBotRect);
        }

        sf::Sprite & bgTopRightSprite{ m_sprites.emplace_back(m_bgTopRightTexture) };

        bgTopRightSprite.setPosition(
            (util::right(bgTopLeftSprite) + betweenCornerSize.x), bgTopLeftSprite.getPosition().y);

        if (betweenCornerSize.y > 0.0f)
        {
            sf::FloatRect bgLeftRect;
            bgLeftRect.left   = bgTopLeftSprite.getPosition().x;
            bgLeftRect.top    = util::bottom(bgTopLeftSprite);
            bgLeftRect.width  = bgTopLeftSprite.getGlobalBounds().width;
            bgLeftRect.height = betweenCornerSize.y;

            sf::Sprite & bgLeftSprite{ m_sprites.emplace_back(m_bgLeftTexture) };
            util::scaleAndCenterInside(bgLeftSprite, bgLeftRect);

            sf::FloatRect bgRightRect;
            bgRightRect.left   = bgTopRightSprite.getPosition().x;
            bgRightRect.top    = bgLeftRect.top;
            bgRightRect.width  = bgLeftRect.width;
            bgRightRect.height = bgLeftRect.height;

            sf::Sprite & bgRightSprite{ m_sprites.emplace_back(m_bgRightTexture) };
            util::scaleAndCenterInside(bgRightSprite, bgRightRect);
        }

        sf::Sprite & bgBotLeftSprite{ m_sprites.emplace_back(m_bgBotLeftTexture) };

        bgBotLeftSprite.setPosition(
            bgTopLeftSprite.getPosition().x, (util::bottom(bgTopLeftSprite) + betweenCornerSize.y));

        sf::Sprite & bgBotRightSprite{ m_sprites.emplace_back(m_bgBotRightTexture) };

        bgBotRightSprite.setPosition(
            bgTopRightSprite.getPosition().x, bgBotLeftSprite.getPosition().y);

        //

        m_bgCenterRect.left   = util::right(bgTopLeftSprite);
        m_bgCenterRect.top    = util::bottom(bgTopLeftSprite);
        m_bgCenterRect.width  = (bgTopRightSprite.getPosition().x - util::right(bgTopLeftSprite));
        m_bgCenterRect.height = (bgBotLeftSprite.getPosition().y - util::bottom(bgTopLeftSprite));

        util::appendQuadVerts(m_bgCenterRect, m_bgCenterVerts, m_bgColor);

        //

        m_innerRect.left   = (m_bgCenterRect.left - 1.0f);
        m_innerRect.top    = (m_bgCenterRect.top - 1.0f);
        m_innerRect.width  = (m_bgCenterRect.width + 2.0f);
        m_innerRect.height = (m_bgCenterRect.height + 2.0f);

        //

        if (GuiWindowBorder::Fancy == m_info.border)
        {
            sf::Sprite & borderTopLeftSprite{ m_sprites.emplace_back(m_borderTopLeftTexture) };

            borderTopLeftSprite.setPosition(
                (bgTopLeftSprite.getPosition().x - 14.0f),
                (bgTopLeftSprite.getPosition().y - 16.0f));

            sf::Sprite & borderTopRightSprite{ m_sprites.emplace_back(m_borderTopRightTexture) };

            borderTopRightSprite.setPosition(
                (bgTopRightSprite.getPosition().x - 26.0f),
                (bgTopRightSprite.getPosition().y - 16.0f));

            const float betweenSizeHoriz{ borderTopRightSprite.getPosition().x -
                                          util::right(borderTopLeftSprite) };

            sf::Sprite & borderBotLeftSprite{ m_sprites.emplace_back(m_borderBotLeftTexture) };

            borderBotLeftSprite.setPosition(
                (bgBotLeftSprite.getPosition().x - 14.0f),
                (bgBotLeftSprite.getPosition().y - 28.0f));

            sf::Sprite & borderBotRightSprite{ m_sprites.emplace_back(m_borderBotRightTexture) };

            borderBotRightSprite.setPosition(
                (bgBotRightSprite.getPosition().x - 26.0f),
                (bgBotRightSprite.getPosition().y - 28.0f));

            const float betweenSizeVert{ borderBotLeftSprite.getPosition().y -
                                         util::bottom(borderTopLeftSprite) };

            if (betweenSizeHoriz > 0.0f)
            {
                sf::Sprite & borderTopSprite{ m_sprites.emplace_back(m_borderTopTexture) };

                sf::FloatRect borderTopRect;
                borderTopRect.left   = util::right(borderTopLeftSprite);
                borderTopRect.top    = (borderTopLeftSprite.getPosition().y + 5.0f);
                borderTopRect.width  = betweenSizeHoriz;
                borderTopRect.height = borderTopSprite.getGlobalBounds().height;

                util::scaleAndCenterInside(borderTopSprite, borderTopRect);

                //

                sf::Sprite & borderBotSprite{ m_sprites.emplace_back(m_borderBotTexture) };

                sf::FloatRect borderBotRect;
                borderBotRect.left = borderTopRect.left;

                borderBotRect.top =
                    ((util::bottom(borderBotLeftSprite) -
                      borderBotSprite.getGlobalBounds().height) -
                     3.0f);

                borderBotRect.width  = borderTopRect.width;
                borderBotRect.height = borderTopRect.height;

                util::scaleAndCenterInside(borderBotSprite, borderBotRect);
            }

            if (betweenSizeVert > 0.0f)
            {
                sf::Sprite & borderLeftpSprite{ m_sprites.emplace_back(m_borderLeftTexture) };

                sf::FloatRect borderLeftRect;
                borderLeftRect.left   = (borderTopLeftSprite.getPosition().x + 3.0f);
                borderLeftRect.top    = util::bottom(borderTopLeftSprite);
                borderLeftRect.width  = borderLeftpSprite.getGlobalBounds().width;
                borderLeftRect.height = betweenSizeVert;

                util::scaleAndCenterInside(borderLeftpSprite, borderLeftRect);

                //

                sf::Sprite & borderRightpSprite{ m_sprites.emplace_back(m_borderRightTexture) };

                sf::FloatRect borderRightRect;
                borderRightRect.left =
                    ((util::right(borderTopRightSprite) -
                      borderRightpSprite.getGlobalBounds().width) -
                     3.0f);

                borderRightRect.top    = borderLeftRect.top;
                borderRightRect.width  = borderRightpSprite.getGlobalBounds().width;
                borderRightRect.height = betweenSizeVert;

                util::scaleAndCenterInside(borderRightpSprite, borderRightRect);
            }

            //

            m_outerRect.left   = borderTopLeftSprite.getPosition().x;
            m_outerRect.top    = borderTopLeftSprite.getPosition().y;
            m_outerRect.width  = (util::right(borderTopRightSprite) - m_outerRect.left);
            m_outerRect.height = (util::bottom(borderBotRightSprite) - m_outerRect.top);
        }
        else if (GuiWindowBorder::Small == m_info.border)
        {
            sf::Sprite & borderTopLeftSprite{ m_sprites.emplace_back(m_smallBorderTopLeftTexture) };

            borderTopLeftSprite.setPosition(
                (bgTopLeftSprite.getPosition().x - 3.0f), (bgTopLeftSprite.getPosition().y - 4.0f));

            sf::Sprite & borderTopRightSprite{ m_sprites.emplace_back(
                m_smallBorderTopRightTexture) };

            borderTopRightSprite.setPosition(
                (bgTopRightSprite.getPosition().x + 4.0f),
                (bgTopRightSprite.getPosition().y - 4.0f));

            const float betweenSizeHoriz{ borderTopRightSprite.getPosition().x -
                                          util::right(borderTopLeftSprite) };

            sf::Sprite & borderBotLeftSprite{ m_sprites.emplace_back(m_smallBorderBotLeftTexture) };

            borderBotLeftSprite.setPosition(
                (bgBotLeftSprite.getPosition().x - 3.0f),
                (bgBotLeftSprite.getPosition().y + 12.0f));

            sf::Sprite & borderBotRightSprite{ m_sprites.emplace_back(
                m_smallBorderBotRightTexture) };

            borderBotRightSprite.setPosition(
                (bgBotRightSprite.getPosition().x + 4.0f),
                (bgBotRightSprite.getPosition().y + 12.0f));

            const float betweenSizeVert{ borderBotLeftSprite.getPosition().y -
                                         util::bottom(borderTopLeftSprite) };

            if (betweenSizeHoriz > 0.0f)
            {
                sf::Sprite & borderTopSprite{ m_sprites.emplace_back(m_smallBorderTopTexture) };

                sf::FloatRect borderTopRect;
                borderTopRect.left   = util::right(borderTopLeftSprite);
                borderTopRect.top    = (borderTopLeftSprite.getPosition().y + 1.0f);
                borderTopRect.width  = betweenSizeHoriz;
                borderTopRect.height = borderTopSprite.getGlobalBounds().height;

                util::scaleAndCenterInside(borderTopSprite, borderTopRect);

                //

                sf::Sprite & borderBotSprite{ m_sprites.emplace_back(m_smallBorderBotTexture) };

                sf::FloatRect borderBotRect;
                borderBotRect.left = borderTopRect.left;

                borderBotRect.top =
                    (util::bottom(borderBotLeftSprite) - borderBotSprite.getGlobalBounds().height);

                borderBotRect.width  = borderTopRect.width;
                borderBotRect.height = borderTopRect.height;

                util::scaleAndCenterInside(borderBotSprite, borderBotRect);
            }

            if (betweenSizeVert > 0.0f)
            {
                sf::Sprite & borderLeftpSprite{ m_sprites.emplace_back(m_smallBorderLeftTexture) };

                sf::FloatRect borderLeftRect;
                borderLeftRect.left   = borderTopLeftSprite.getPosition().x;
                borderLeftRect.top    = util::bottom(borderTopLeftSprite);
                borderLeftRect.width  = borderLeftpSprite.getGlobalBounds().width;
                borderLeftRect.height = betweenSizeVert;

                util::scaleAndCenterInside(borderLeftpSprite, borderLeftRect);

                //

                sf::Sprite & borderRightpSprite{ m_sprites.emplace_back(
                    m_smallBorderRightTexture) };

                sf::FloatRect borderRightRect;
                borderRightRect.left =
                    (util::right(borderTopRightSprite) -
                     borderRightpSprite.getGlobalBounds().width);

                borderRightRect.top    = borderLeftRect.top;
                borderRightRect.width  = borderRightpSprite.getGlobalBounds().width;
                borderRightRect.height = betweenSizeVert;

                util::scaleAndCenterInside(borderRightpSprite, borderRightRect);
            }

            //

            m_outerRect.left   = borderTopLeftSprite.getPosition().x;
            m_outerRect.top    = borderTopLeftSprite.getPosition().y;
            m_outerRect.width  = (util::right(borderTopRightSprite) - m_outerRect.left);
            m_outerRect.height = (util::bottom(borderBotRightSprite) - m_outerRect.top);
        }
        else // GuiWindowBorder::None
        {
            m_outerRect.left   = bgTopLeftSprite.getPosition().x;
            m_outerRect.top    = bgTopLeftSprite.getPosition().y;
            m_outerRect.width  = (util::right(bgTopRightSprite) - m_outerRect.left);
            m_outerRect.height = (util::bottom(bgBotRightSprite) - m_outerRect.top);
        }

        if (!m_info.title.empty())
        {
            sf::Sprite & tapeLeftSprite{ m_sprites.emplace_back(m_tapeLeftTexture) };

            tapeLeftSprite.setPosition(
                ((context.layout.wholeSize().x * 0.5f) - tapeLeftSprite.getGlobalBounds().width),
                (m_outerRect.top - (tapeLeftSprite.getGlobalBounds().height * 0.5f)));

            sf::Sprite & tapeRightSprite{ m_sprites.emplace_back(m_tapeRightTexture) };

            tapeRightSprite.setPosition(
                (context.layout.wholeSize().x * 0.5f),
                (m_outerRect.top - (tapeRightSprite.getGlobalBounds().height * 0.5f)));

            sf::FloatRect titleRect;
            titleRect.left   = ((context.layout.wholeSize().x * 0.5f) - 100.0f);
            titleRect.top    = (tapeLeftSprite.getPosition().y + 14.0f);
            titleRect.width  = 200.0f;
            titleRect.height = 37.0f;

            m_titleText = context.font.makeText(
                Font::Default, FontSize::Medium, m_info.title, sf::Color(32, 32, 32));

            util::fitAndCenterInside(m_titleText, titleRect);

            const float betweenTapeSize{ m_titleText.getGlobalBounds().width - titleRect.width };

            if (betweenTapeSize > 0.0f)
            {
                tapeLeftSprite.move(-(betweenTapeSize * 0.5f), 0.0f);
                tapeRightSprite.move((betweenTapeSize * 0.5f), 0.0f);

                titleRect.left -= (betweenTapeSize * 0.5f);
                titleRect.width += betweenTapeSize;

                util::fitAndCenterInside(m_titleText, titleRect);

                sf::Sprite & tapeMiddleSprite{ m_sprites.emplace_back(m_tapeMiddleTexture) };
                sf::FloatRect tapeMiddleRect;

                tapeMiddleRect.left =
                    ((context.layout.wholeSize().x * 0.5f) - (betweenTapeSize * 0.5f));

                tapeMiddleRect.top    = (tapeLeftSprite.getPosition().y + 8.0f);
                tapeMiddleRect.width  = betweenTapeSize;
                tapeMiddleRect.height = tapeMiddleSprite.getGlobalBounds().height;

                util::scaleAndCenterInside(tapeMiddleSprite, tapeMiddleRect);
            }

            m_outerRect.width += ((m_outerRect.left - tapeLeftSprite.getPosition().x) * 2.0f);
            m_outerRect.left = tapeLeftSprite.getPosition().x;

            m_outerRect.height += (m_outerRect.top - tapeLeftSprite.getPosition().y);
            m_outerRect.top = tapeLeftSprite.getPosition().y;
        }

        m_contentTexts = TextLayout::layout(context, m_info.content, m_innerRect, m_info.details);
    }

    void GuiWindow::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const sf::Sprite & sprite : m_sprites)
        {
            target.draw(sprite, states);
        }

        target.draw(&m_bgCenterVerts[0], m_bgCenterVerts.size(), sf::Quads, states);
        target.draw(m_titleText, states);

        for (const sf::Text & text : m_contentTexts)
        {
            target.draw(text, states);
        }
    }

} // namespace platformer
