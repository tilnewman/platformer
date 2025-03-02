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
#include "text-layout.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

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

        util::TextureLoader::load(
            m_borderTopLeftTexture, (t_settings.media_path / "image/ui/border-top-left.png"));

        util::TextureLoader::load(
            m_borderTopRightTexture, (t_settings.media_path / "image/ui/border-top-right.png"));

        util::TextureLoader::load(
            m_borderBotLeftTexture, (t_settings.media_path / "image/ui/border-bottom-left.png"));

        util::TextureLoader::load(
            m_borderBotRightTexture, (t_settings.media_path / "image/ui/border-bottom-right.png"));

        util::TextureLoader::load(
            m_borderTopTexture, (t_settings.media_path / "image/ui/border-top.png"));

        util::TextureLoader::load(
            m_borderBotTexture, (t_settings.media_path / "image/ui/border-bottom.png"));

        util::TextureLoader::load(
            m_borderLeftTexture, (t_settings.media_path / "image/ui/border-left.png"));

        util::TextureLoader::load(
            m_borderRightTexture, (t_settings.media_path / "image/ui/border-right.png"));

        //

        util::TextureLoader::load(
            m_smallBorderTopLeftTexture,
            (t_settings.media_path / "image/ui/small-border-top-left.png"));

        util::TextureLoader::load(
            m_smallBorderTopRightTexture,
            (t_settings.media_path / "image/ui/small-border-top-right.png"));

        util::TextureLoader::load(
            m_smallBorderBotLeftTexture,
            (t_settings.media_path / "image/ui/small-border-bottom-left.png"));

        util::TextureLoader::load(
            m_smallBorderBotRightTexture,
            (t_settings.media_path / "image/ui/small-border-bottom-right.png"));

        util::TextureLoader::load(
            m_smallBorderTopTexture, (t_settings.media_path / "image/ui/small-border-top.png"));

        util::TextureLoader::load(
            m_smallBorderBotTexture, (t_settings.media_path / "image/ui/small-border-bottom.png"));

        util::TextureLoader::load(
            m_smallBorderLeftTexture, (t_settings.media_path / "image/ui/small-border-left.png"));

        util::TextureLoader::load(
            m_smallBorderRightTexture, (t_settings.media_path / "image/ui/small-border-right.png"));

        //

        util::TextureLoader::load(
            m_bgTopLeftTexture, (t_settings.media_path / "image/ui/menu-bg-top-left.png"));

        util::TextureLoader::load(
            m_bgTopRightTexture, (t_settings.media_path / "image/ui/menu-bg-top-right.png"));

        util::TextureLoader::load(
            m_bgBotLeftTexture, (t_settings.media_path / "image/ui/menu-bg-bottom-left.png"));

        util::TextureLoader::load(
            m_bgBotRightTexture, (t_settings.media_path / "image/ui/menu-bg-bottom-right.png"));

        util::TextureLoader::load(
            m_bgTopTexture, (t_settings.media_path / "image/ui/menu-bg-top.png"));

        util::TextureLoader::load(
            m_bgBotTexture, (t_settings.media_path / "image/ui/menu-bg-bottom.png"));

        util::TextureLoader::load(
            m_bgLeftTexture, (t_settings.media_path / "image/ui/menu-bg-left.png"));

        util::TextureLoader::load(
            m_bgRightTexture, (t_settings.media_path / "image/ui/menu-bg-right.png"));

        //

        util::TextureLoader::load(
            m_tapeLeftTexture, (t_settings.media_path / "image/ui/tape-left.png"));

        util::TextureLoader::load(
            m_tapeRightTexture, (t_settings.media_path / "image/ui/tape-right.png"));

        util::TextureLoader::load(
            m_tapeMiddleTexture, (t_settings.media_path / "image/ui/tape-middle.png"));
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
                Font::Title, FontSize::Medium, m_info.title, sf::Color(32, 32, 32));

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
