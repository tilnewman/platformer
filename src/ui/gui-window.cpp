// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// gui-window.cpp
//
#include "ui/gui-window.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/font.hpp"
#include "subsystem/screen-layout.hpp"
#include "subsystem/texture-stats.hpp"
#include "text-layout.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    GuiWindow::GuiWindow()
        : m_info{}
        , m_innerRect{}
        , m_outerRect{}
        , m_bgColor{ 74, 76, 41 }
        , m_bgCenterRect{}
        , m_bgCenterVerts{}
        , m_sprites{}
        , m_titleText{ util::SfmlDefaults::instance().font() }
        , m_contentTexts{}
        , m_bgFadeVerts{}
    {
        m_bgCenterVerts.reserve(util::verts_per_quad);
        m_bgFadeVerts.reserve(util::verts_per_quad);
        m_sprites.reserve(16);
    }

    void GuiWindow::loadTextures(const Settings & t_settings)
    {
        // bail if these textures have already been loaded
        if (m_borderTopLeftTexture.getSize().y > 0)
        {
            return;
        }

        M_CHECK(
            m_borderTopLeftTexture.loadFromFile(
                (t_settings.media_path / "image/ui/border-top-left.png").string()),
            "file not found");

        M_CHECK(
            m_borderTopRightTexture.loadFromFile(
                (t_settings.media_path / "image/ui/border-top-right.png").string()),
            "file not found");

        M_CHECK(
            m_borderBotLeftTexture.loadFromFile(
                (t_settings.media_path / "image/ui/border-bottom-left.png").string()),
            "file not found");

        M_CHECK(
            m_borderBotRightTexture.loadFromFile(
                (t_settings.media_path / "image/ui/border-bottom-right.png").string()),
            "file not found");

        M_CHECK(
            m_borderTopTexture.loadFromFile(
                (t_settings.media_path / "image/ui/border-top.png").string()),
            "file not found");

        M_CHECK(
            m_borderBotTexture.loadFromFile(
                (t_settings.media_path / "image/ui/border-bottom.png").string()),
            "file not found");

        M_CHECK(
            m_borderLeftTexture.loadFromFile(
                (t_settings.media_path / "image/ui/border-left.png").string()),
            "file not found");

        M_CHECK(
            m_borderRightTexture.loadFromFile(
                (t_settings.media_path / "image/ui/border-right.png").string()),
            "file not found");

        TextureStats::instance().process(m_borderTopLeftTexture);
        TextureStats::instance().process(m_borderTopRightTexture);
        TextureStats::instance().process(m_borderBotLeftTexture);
        TextureStats::instance().process(m_borderBotRightTexture);
        TextureStats::instance().process(m_borderTopTexture);
        TextureStats::instance().process(m_borderBotTexture);
        TextureStats::instance().process(m_borderLeftTexture);
        TextureStats::instance().process(m_borderRightTexture);

        //

        M_CHECK(
            m_smallBorderTopLeftTexture.loadFromFile(
                (t_settings.media_path / "image/ui/small-border-top-left.png").string()),
            "file not found");

        M_CHECK(
            m_smallBorderTopRightTexture.loadFromFile(
                (t_settings.media_path / "image/ui/small-border-top-right.png").string()),
            "file not found");

        M_CHECK(
            m_smallBorderBotLeftTexture.loadFromFile(
                (t_settings.media_path / "image/ui/small-border-bottom-left.png").string()),
            "file not found");

        M_CHECK(
            m_smallBorderBotRightTexture.loadFromFile(
                (t_settings.media_path / "image/ui/small-border-bottom-right.png").string()),
            "file not found");

        M_CHECK(
            m_smallBorderTopTexture.loadFromFile(
                (t_settings.media_path / "image/ui/small-border-top.png").string()),
            "file not found");

        M_CHECK(
            m_smallBorderBotTexture.loadFromFile(
                (t_settings.media_path / "image/ui/small-border-bottom.png").string()),
            "file not found");

        M_CHECK(
            m_smallBorderLeftTexture.loadFromFile(
                (t_settings.media_path / "image/ui/small-border-left.png").string()),
            "file not found");

        M_CHECK(
            m_smallBorderRightTexture.loadFromFile(
                (t_settings.media_path / "image/ui/small-border-right.png").string()),
            "file not found");

        TextureStats::instance().process(m_smallBorderTopLeftTexture);
        TextureStats::instance().process(m_smallBorderTopRightTexture);
        TextureStats::instance().process(m_smallBorderBotLeftTexture);
        TextureStats::instance().process(m_smallBorderBotRightTexture);
        TextureStats::instance().process(m_smallBorderTopTexture);
        TextureStats::instance().process(m_smallBorderBotTexture);
        TextureStats::instance().process(m_smallBorderLeftTexture);
        TextureStats::instance().process(m_smallBorderRightTexture);

        //

        M_CHECK(
            m_bgTopLeftTexture.loadFromFile(
                (t_settings.media_path / "image/ui/menu-bg-top-left.png").string()),
            "file not found");

        M_CHECK(
            m_bgTopRightTexture.loadFromFile(
                (t_settings.media_path / "image/ui/menu-bg-top-right.png").string()),
            "file not found");

        M_CHECK(
            m_bgBotLeftTexture.loadFromFile(
                (t_settings.media_path / "image/ui/menu-bg-bottom-left.png").string()),
            "file not found");

        M_CHECK(
            m_bgBotRightTexture.loadFromFile(
                (t_settings.media_path / "image/ui/menu-bg-bottom-right.png").string()),
            "file not found");

        M_CHECK(
            m_bgTopTexture.loadFromFile(
                (t_settings.media_path / "image/ui/menu-bg-top.png").string()),
            "file not found");

        M_CHECK(
            m_bgBotTexture.loadFromFile(
                (t_settings.media_path / "image/ui/menu-bg-bottom.png").string()),
            "file not found");

        M_CHECK(
            m_bgLeftTexture.loadFromFile(
                (t_settings.media_path / "image/ui/menu-bg-left.png").string()),
            "file not found");

        M_CHECK(
            m_bgRightTexture.loadFromFile(
                (t_settings.media_path / "image/ui/menu-bg-right.png").string()),
            "file not found");

        TextureStats::instance().process(m_bgTopLeftTexture);
        TextureStats::instance().process(m_bgTopRightTexture);
        TextureStats::instance().process(m_bgBotLeftTexture);
        TextureStats::instance().process(m_bgBotRightTexture);
        TextureStats::instance().process(m_bgTopTexture);
        TextureStats::instance().process(m_bgBotTexture);
        TextureStats::instance().process(m_bgLeftTexture);
        TextureStats::instance().process(m_bgRightTexture);

        //

        M_CHECK(
            m_tapeLeftTexture.loadFromFile(
                (t_settings.media_path / "image/ui/tape-left.png").string()),
            "file not found");

        M_CHECK(
            m_tapeRightTexture.loadFromFile(
                (t_settings.media_path / "image/ui/tape-right.png").string()),
            "file not found");

        M_CHECK(
            m_tapeMiddleTexture.loadFromFile(
                (t_settings.media_path / "image/ui/tape-middle.png").string()),
            "file not found");

        TextureStats::instance().process(m_tapeLeftTexture);
        TextureStats::instance().process(m_tapeRightTexture);
        TextureStats::instance().process(m_tapeMiddleTexture);
    }

    void GuiWindow::arrange(Context & t_context, const GuiWindowInfo & t_info)
    {
        m_sprites.clear();

        m_bgFadeVerts.clear();

        if (t_info.will_fade_whole_screen)
        {
            util::appendTriangleVerts(
                t_context.layout.wholeRect(), m_bgFadeVerts, sf::Color(0, 0, 0, 127));
        }

        m_info = t_info;

        sf::Sprite bgTopLeftSprite{ m_bgTopLeftTexture };

        sf::Vector2f betweenCornerSize;

        betweenCornerSize.x =
            (m_info.region.size.x - (2.0f * bgTopLeftSprite.getGlobalBounds().size.x));

        if (betweenCornerSize.x < 0.0f)
        {
            betweenCornerSize.x = 0.0f;
        }

        betweenCornerSize.y =
            (m_info.region.size.y - (2.0f * bgTopLeftSprite.getGlobalBounds().size.y));

        if (betweenCornerSize.y < 0.0f)
        {
            betweenCornerSize.y = 0.0f;
        }

        bgTopLeftSprite.setPosition({ t_info.region.position.x, t_info.region.position.y });

        if (t_info.will_draw_background && (betweenCornerSize.x > 0.0f))
        {
            sf::FloatRect bgTopRect;
            bgTopRect.position.x = util::right(bgTopLeftSprite);
            bgTopRect.position.y = bgTopLeftSprite.getPosition().y;
            bgTopRect.size.x     = betweenCornerSize.x;
            bgTopRect.size.y     = bgTopLeftSprite.getGlobalBounds().size.y;

            sf::Sprite & bgTopSprite{ m_sprites.emplace_back(m_bgTopTexture) };
            util::scaleAndCenterInside(bgTopSprite, bgTopRect);

            sf::FloatRect bgBotRect;
            bgBotRect.position.x = bgTopRect.position.x;
            bgBotRect.position.y = (util::bottom(bgTopLeftSprite) + betweenCornerSize.y);
            bgBotRect.size.x     = bgTopRect.size.x;
            bgBotRect.size.y     = bgTopRect.size.y;

            sf::Sprite & bgBotSprite{ m_sprites.emplace_back(m_bgBotTexture) };
            util::scaleAndCenterInside(bgBotSprite, bgBotRect);
        }

        sf::Sprite bgTopRightSprite{ m_bgTopRightTexture };

        bgTopRightSprite.setPosition({ (util::right(bgTopLeftSprite) + betweenCornerSize.x),
                                       bgTopLeftSprite.getPosition().y });

        if (t_info.will_draw_background && (betweenCornerSize.y > 0.0f))
        {
            sf::FloatRect bgLeftRect;
            bgLeftRect.position.x = bgTopLeftSprite.getPosition().x;
            bgLeftRect.position.y = util::bottom(bgTopLeftSprite);
            bgLeftRect.size.x     = bgTopLeftSprite.getGlobalBounds().size.x;
            bgLeftRect.size.y     = betweenCornerSize.y;

            sf::Sprite & bgLeftSprite{ m_sprites.emplace_back(m_bgLeftTexture) };
            util::scaleAndCenterInside(bgLeftSprite, bgLeftRect);

            sf::FloatRect bgRightRect;
            bgRightRect.position.x = bgTopRightSprite.getPosition().x;
            bgRightRect.position.y = bgLeftRect.position.y;
            bgRightRect.size.x     = bgLeftRect.size.x;
            bgRightRect.size.y     = bgLeftRect.size.y;

            sf::Sprite & bgRightSprite{ m_sprites.emplace_back(m_bgRightTexture) };
            util::scaleAndCenterInside(bgRightSprite, bgRightRect);
        }

        sf::Sprite bgBotLeftSprite{ m_bgBotLeftTexture };

        bgBotLeftSprite.setPosition({ bgTopLeftSprite.getPosition().x,
                                      (util::bottom(bgTopLeftSprite) + betweenCornerSize.y) });

        sf::Sprite bgBotRightSprite{ m_bgBotRightTexture };

        bgBotRightSprite.setPosition(
            { bgTopRightSprite.getPosition().x, bgBotLeftSprite.getPosition().y });

        //

        m_bgCenterRect.position.x = util::right(bgTopLeftSprite);
        m_bgCenterRect.position.y = util::bottom(bgTopLeftSprite);
        m_bgCenterRect.size.x = (bgTopRightSprite.getPosition().x - util::right(bgTopLeftSprite));
        m_bgCenterRect.size.y = (bgBotLeftSprite.getPosition().y - util::bottom(bgTopLeftSprite));

        //

        if (t_info.will_draw_background)
        {
            m_sprites.push_back(bgTopLeftSprite);
            m_sprites.push_back(bgTopRightSprite);
            m_sprites.push_back(bgBotLeftSprite);
            m_sprites.push_back(bgBotRightSprite);
            util::appendTriangleVerts(m_bgCenterRect, m_bgCenterVerts, m_bgColor);
        }

        //

        m_innerRect.position.x = (m_bgCenterRect.position.x - 1.0f);
        m_innerRect.position.y = (m_bgCenterRect.position.y - 1.0f);
        m_innerRect.size.x     = (m_bgCenterRect.size.x + 2.0f);
        m_innerRect.size.y     = (m_bgCenterRect.size.y + 2.0f);

        //

        if (GuiWindowBorder::Fancy == m_info.border)
        {
            sf::Sprite & borderTopLeftSprite{ m_sprites.emplace_back(m_borderTopLeftTexture) };

            borderTopLeftSprite.setPosition({ (bgTopLeftSprite.getPosition().x - 14.0f),
                                              (bgTopLeftSprite.getPosition().y - 16.0f) });

            sf::Sprite & borderTopRightSprite{ m_sprites.emplace_back(m_borderTopRightTexture) };

            borderTopRightSprite.setPosition({ (bgTopRightSprite.getPosition().x - 26.0f),
                                               (bgTopRightSprite.getPosition().y - 16.0f) });

            const float betweenSizeHoriz{ borderTopRightSprite.getPosition().x -
                                          util::right(borderTopLeftSprite) };

            sf::Sprite & borderBotLeftSprite{ m_sprites.emplace_back(m_borderBotLeftTexture) };

            borderBotLeftSprite.setPosition({ (bgBotLeftSprite.getPosition().x - 14.0f),
                                              (bgBotLeftSprite.getPosition().y - 28.0f) });

            sf::Sprite & borderBotRightSprite{ m_sprites.emplace_back(m_borderBotRightTexture) };

            borderBotRightSprite.setPosition({ (bgBotRightSprite.getPosition().x - 26.0f),
                                               (bgBotRightSprite.getPosition().y - 28.0f) });

            const float betweenSizeVert{ borderBotLeftSprite.getPosition().y -
                                         util::bottom(borderTopLeftSprite) };

            if (betweenSizeHoriz > 0.0f)
            {
                sf::Sprite & borderTopSprite{ m_sprites.emplace_back(m_borderTopTexture) };

                sf::FloatRect borderTopRect;
                borderTopRect.position.x = util::right(borderTopLeftSprite);
                borderTopRect.position.y = (borderTopLeftSprite.getPosition().y + 4.0f);
                borderTopRect.size.x     = betweenSizeHoriz;
                borderTopRect.size.y     = borderTopSprite.getGlobalBounds().size.y;

                util::scaleAndCenterInside(borderTopSprite, borderTopRect);

                //

                sf::Sprite & borderBotSprite{ m_sprites.emplace_back(m_borderBotTexture) };

                sf::FloatRect borderBotRect;
                borderBotRect.position.x = borderTopRect.position.x;

                borderBotRect.position.y =
                    ((util::bottom(borderBotLeftSprite) -
                      borderBotSprite.getGlobalBounds().size.y) -
                     4.0f);

                borderBotRect.size.x = borderTopRect.size.x;
                borderBotRect.size.y = borderTopRect.size.y;

                util::scaleAndCenterInside(borderBotSprite, borderBotRect);
            }

            if (betweenSizeVert > 0.0f)
            {
                sf::Sprite & borderLeftpSprite{ m_sprites.emplace_back(m_borderLeftTexture) };

                sf::FloatRect borderLeftRect;
                borderLeftRect.position.x = (borderTopLeftSprite.getPosition().x + 3.0f);
                borderLeftRect.position.y = util::bottom(borderTopLeftSprite);
                borderLeftRect.size.x     = borderLeftpSprite.getGlobalBounds().size.x;
                borderLeftRect.size.y     = betweenSizeVert;

                util::scaleAndCenterInside(borderLeftpSprite, borderLeftRect);

                //

                sf::Sprite & borderRightpSprite{ m_sprites.emplace_back(m_borderRightTexture) };

                sf::FloatRect borderRightRect;
                borderRightRect.position.x =
                    ((util::right(borderTopRightSprite) -
                      borderRightpSprite.getGlobalBounds().size.x) -
                     3.0f);

                borderRightRect.position.y = borderLeftRect.position.y;
                borderRightRect.size.x     = borderRightpSprite.getGlobalBounds().size.x;
                borderRightRect.size.y     = betweenSizeVert;

                util::scaleAndCenterInside(borderRightpSprite, borderRightRect);
            }

            //

            m_outerRect.position.x = borderTopLeftSprite.getPosition().x;
            m_outerRect.position.y = borderTopLeftSprite.getPosition().y;
            m_outerRect.size.x     = (util::right(borderTopRightSprite) - m_outerRect.position.x);
            m_outerRect.size.y     = (util::bottom(borderBotRightSprite) - m_outerRect.position.y);
        }
        else if (GuiWindowBorder::Small == m_info.border)
        {
            sf::Sprite & borderTopLeftSprite{ m_sprites.emplace_back(m_smallBorderTopLeftTexture) };

            borderTopLeftSprite.setPosition({ (bgTopLeftSprite.getPosition().x - 3.0f),
                                              (bgTopLeftSprite.getPosition().y - 4.0f) });

            sf::Sprite & borderTopRightSprite{ m_sprites.emplace_back(
                m_smallBorderTopRightTexture) };

            borderTopRightSprite.setPosition({ (bgTopRightSprite.getPosition().x + 4.0f),
                                               (bgTopRightSprite.getPosition().y - 4.0f) });

            const float betweenSizeHoriz{ borderTopRightSprite.getPosition().x -
                                          util::right(borderTopLeftSprite) };

            sf::Sprite & borderBotLeftSprite{ m_sprites.emplace_back(m_smallBorderBotLeftTexture) };

            borderBotLeftSprite.setPosition({ (bgBotLeftSprite.getPosition().x - 3.0f),
                                              (bgBotLeftSprite.getPosition().y + 12.0f) });

            sf::Sprite & borderBotRightSprite{ m_sprites.emplace_back(
                m_smallBorderBotRightTexture) };

            borderBotRightSprite.setPosition({ (bgBotRightSprite.getPosition().x + 4.0f),
                                               (bgBotRightSprite.getPosition().y + 12.0f) });

            const float betweenSizeVert{ borderBotLeftSprite.getPosition().y -
                                         util::bottom(borderTopLeftSprite) };

            if (betweenSizeHoriz > 0.0f)
            {
                sf::Sprite & borderTopSprite{ m_sprites.emplace_back(m_smallBorderTopTexture) };

                sf::FloatRect borderTopRect;
                borderTopRect.position.x = util::right(borderTopLeftSprite);
                borderTopRect.position.y = (borderTopLeftSprite.getPosition().y);
                borderTopRect.size.x     = betweenSizeHoriz;
                borderTopRect.size.y     = borderTopSprite.getGlobalBounds().size.y;

                util::scaleAndCenterInside(borderTopSprite, borderTopRect);

                //

                sf::Sprite & borderBotSprite{ m_sprites.emplace_back(m_smallBorderBotTexture) };

                sf::FloatRect borderBotRect;
                borderBotRect.position.x = borderTopRect.position.x;

                borderBotRect.position.y =
                    (util::bottom(borderBotLeftSprite) - borderBotSprite.getGlobalBounds().size.y);

                borderBotRect.size.x = borderTopRect.size.x;
                borderBotRect.size.y = borderTopRect.size.y;

                util::scaleAndCenterInside(borderBotSprite, borderBotRect);
            }

            if (betweenSizeVert > 0.0f)
            {
                sf::Sprite & borderLeftpSprite{ m_sprites.emplace_back(m_smallBorderLeftTexture) };

                sf::FloatRect borderLeftRect;
                borderLeftRect.position.x = borderTopLeftSprite.getPosition().x;
                borderLeftRect.position.y = util::bottom(borderTopLeftSprite);
                borderLeftRect.size.x     = borderLeftpSprite.getGlobalBounds().size.x;
                borderLeftRect.size.y     = betweenSizeVert;

                util::scaleAndCenterInside(borderLeftpSprite, borderLeftRect);

                //

                sf::Sprite & borderRightpSprite{ m_sprites.emplace_back(
                    m_smallBorderRightTexture) };

                sf::FloatRect borderRightRect;

                borderRightRect.position.x =
                    (util::right(borderTopRightSprite) -
                     borderRightpSprite.getGlobalBounds().size.x);

                borderRightRect.position.y = borderLeftRect.position.y;
                borderRightRect.size.x     = borderRightpSprite.getGlobalBounds().size.x;
                borderRightRect.size.y     = betweenSizeVert;

                util::scaleAndCenterInside(borderRightpSprite, borderRightRect);
            }

            //

            m_outerRect.position.x = borderTopLeftSprite.getPosition().x;
            m_outerRect.position.y = borderTopLeftSprite.getPosition().y;
            m_outerRect.size.x     = (util::right(borderTopRightSprite) - m_outerRect.position.x);
            m_outerRect.size.y     = (util::bottom(borderBotRightSprite) - m_outerRect.position.y);
        }
        else // GuiWindowBorder::None
        {
            m_outerRect.position.x = bgTopLeftSprite.getPosition().x;
            m_outerRect.position.y = bgTopLeftSprite.getPosition().y;
            m_outerRect.size.x     = (util::right(bgTopRightSprite) - m_outerRect.position.x);
            m_outerRect.size.y     = (util::bottom(bgBotRightSprite) - m_outerRect.position.y);
        }

        if (!m_info.title.empty())
        {
            sf::Sprite & tapeLeftSprite{ m_sprites.emplace_back(m_tapeLeftTexture) };

            tapeLeftSprite.setPosition(
                { ((t_context.layout.wholeSize().x * 0.5f) -
                   tapeLeftSprite.getGlobalBounds().size.x),
                  (m_outerRect.position.y - (tapeLeftSprite.getGlobalBounds().size.y * 0.65f)) });

            sf::Sprite & tapeRightSprite{ m_sprites.emplace_back(m_tapeRightTexture) };

            tapeRightSprite.setPosition(
                { (t_context.layout.wholeSize().x * 0.5f),
                  (m_outerRect.position.y - (tapeRightSprite.getGlobalBounds().size.y * 0.65f)) });

            sf::FloatRect titleRect;
            titleRect.position.x = ((t_context.layout.wholeSize().x * 0.5f) - 100.0f);
            titleRect.position.y = (tapeLeftSprite.getPosition().y + 14.0f);
            titleRect.size.x     = 200.0f;
            titleRect.size.y     = 37.0f;

            m_titleText = t_context.font.makeText(
                Font::Default, FontSize::Medium, m_info.title, sf::Color(32, 32, 32));

            util::fitAndCenterInside(m_titleText, titleRect);

            const float betweenTapeSize{ m_titleText.getGlobalBounds().size.x - titleRect.size.x };

            if (betweenTapeSize > 0.0f)
            {
                tapeLeftSprite.move({ -(betweenTapeSize * 0.5f), 0.0f });
                tapeRightSprite.move({ (betweenTapeSize * 0.5f), 0.0f });

                titleRect.position.x -= (betweenTapeSize * 0.5f);
                titleRect.size.x += betweenTapeSize;

                util::fitAndCenterInside(m_titleText, titleRect);

                sf::Sprite & tapeMiddleSprite{ m_sprites.emplace_back(m_tapeMiddleTexture) };
                sf::FloatRect tapeMiddleRect;

                tapeMiddleRect.position.x =
                    ((t_context.layout.wholeSize().x * 0.5f) - (betweenTapeSize * 0.5f));

                tapeMiddleRect.position.y = (tapeLeftSprite.getPosition().y + 8.0f);
                tapeMiddleRect.size.x     = betweenTapeSize;
                tapeMiddleRect.size.y     = tapeMiddleSprite.getGlobalBounds().size.y;

                util::scaleAndCenterInside(tapeMiddleSprite, tapeMiddleRect);
            }

            m_outerRect.size.x +=
                ((m_outerRect.position.x - tapeLeftSprite.getPosition().x) * 2.0f);

            m_outerRect.position.x = tapeLeftSprite.getPosition().x;

            m_outerRect.size.y += (m_outerRect.position.y - tapeLeftSprite.getPosition().y);
            m_outerRect.position.y = tapeLeftSprite.getPosition().y;
        }

        m_contentTexts = TextLayout::layout(t_context, m_info.content, m_innerRect, m_info.details);
    }

    void GuiWindow::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        if (!m_bgFadeVerts.empty())
        {
            target.draw(
                &m_bgFadeVerts[0], m_bgFadeVerts.size(), sf::PrimitiveType::Triangles, states);
        }

        for (const sf::Sprite & sprite : m_sprites)
        {
            target.draw(sprite, states);
        }

        if (!m_bgCenterVerts.empty())
        {
            target.draw(
                &m_bgCenterVerts[0], m_bgCenterVerts.size(), sf::PrimitiveType::Triangles, states);
        }

        target.draw(m_titleText, states);

        for (const sf::Text & text : m_contentTexts)
        {
            target.draw(text, states);
        }
    }

} // namespace bramblefore
