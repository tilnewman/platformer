// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// spell-select-menu.hpp
//
#include "ui/spell-select-menu.hpp"

#include "avatar/spells-anim.hpp"
#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/font.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

#include <limits>

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    SpellSelectMenu::SpellSelectMenu()
        : m_isVisible{ false }
        , m_elapsedTimeSec{ 0.0f }
        , m_spellSprites{}
        , m_glowRects{}
        , m_fadeRects{}
        , m_windowFrame{}
        , m_slider{}
        , m_selectionTarget{ 0 }
        , m_selectionRect{}
        , m_isMovingSelection{ false }
        , m_nameText{}
    {}

    void SpellSelectMenu::loadTextures(const Settings & t_settings)
    {
        m_windowFrame.loadTextures(t_settings);
    }

    void SpellSelectMenu::setup(Context & t_context, const std::size_t t_selectionOrig)
    {
        m_fadeRects.clear();
        m_glowRects.clear();
        m_spellSprites.clear();

        m_isVisible      = true;
        m_elapsedTimeSec = 0.0f;

        const std::vector<PlayerSpell> & playerSpells{ t_context.player.spells() };

        m_nameText = t_context.font.makeText(Font::Default, FontSize::Small, "");

        const float iconSize{ t_context.layout.wholeSize().y * 0.1f };

        sf::Vector2f pos{ t_context.layout.wholeSize() * sf::Vector2f{ 0.333f, 0.1f } };

        //

        // setup m_selectionRect and m_windowFrame if first time calling setup()
        if (m_selectionRect.width < 1.0f)
        {
            m_selectionRect.left   = pos.x;
            m_selectionRect.top    = pos.y;
            m_selectionRect.width  = iconSize;
            m_selectionRect.height = iconSize;

            m_windowFrame.arrange(t_context, makeGuiWindowInfo(m_selectionRect));
        }

        //

        // players press number keys 1-9 to get here but playerSpells is zero indexed,
        // so subtract one from the original selection that the player provided
        const std::size_t selection{ t_selectionOrig - 1 };

        if (m_selectionTarget != selection)
        {
            if ((selection < playerSpells.size()) && (playerSpells.at(selection).is_learned))
            {
                m_isMovingSelection = true;
                t_context.sfx.play("ui-select-magic");
                m_selectionTarget = selection;

                const float targetHorizPos{ pos.x +
                                            (static_cast<float>(selection) * (iconSize * 1.5f)) };

                m_slider = util::SliderFromTo<float>(m_selectionRect.left, targetHorizPos, 10.0f);

                t_context.player.setCurentSpellIndex(selection);
            }
            else
            {
                t_context.sfx.play("ui-reject-2");
            }
        }

        //

        for (std::size_t spellIndex{ 0 }; spellIndex < playerSpells.size(); ++spellIndex)
        {
            const PlayerSpell & playerSpell{ playerSpells.at(spellIndex) };
            const sf::FloatRect rect{ pos.x, pos.y, iconSize, iconSize };

            sf::Sprite & sprite{ m_spellSprites.emplace_back() };
            sprite.setTexture(t_context.spell.iconTexture(playerSpell.spell));
            util::fitAndCenterInside(sprite, rect);

            //

            GlowRect & glowRect{ m_glowRects.emplace_back() };

            glowRect.setup(sf::Color(0, 0, 0, 200), util::scaleRectInPlaceCopy(rect, 1.25f), rect);

            //

            if (!playerSpell.is_learned)
            {
                m_fadeRects.push_back(rect);
            }

            //

            pos.x += rect.width;
            pos.x += (rect.width * 0.5f);
        }

        //

        if (m_selectionTarget < playerSpells.size())
        {
            m_nameText.setString(std::string{ toName(playerSpells.at(m_selectionTarget).spell) });
            util::setOriginToPosition(m_nameText);
        }

        m_nameText.setPosition(
            (util::center(m_selectionRect).x - (m_nameText.getGlobalBounds().width * 0.5f)),
            (util::bottom(m_selectionRect) + (m_selectionRect.height * 0.15f)));
    }

    void SpellSelectMenu::update(Context & t_context, const float t_frameTimeSec)
    {
        m_elapsedTimeSec += t_frameTimeSec;
        if (m_elapsedTimeSec > 3.0f)
        {
            m_isVisible = false;
        }

        if (m_isMovingSelection)
        {
            m_nameText.setFillColor(sf::Color::Transparent);

            m_selectionRect.left = m_slider.update(t_frameTimeSec);
            m_windowFrame.arrange(t_context, makeGuiWindowInfo(m_selectionRect));
            m_isMovingSelection = m_slider.isMoving();

            if (!m_isMovingSelection)
            {
                m_nameText.setFillColor(sf::Color::White);

                m_nameText.setPosition(
                    (util::center(m_selectionRect).x - (m_nameText.getGlobalBounds().width * 0.5f)),
                    (util::bottom(m_selectionRect) + (m_selectionRect.height * 0.15f)));
            }
        }
    }

    void SpellSelectMenu::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        if (!m_isVisible)
        {
            return;
        }

        for (const GlowRect & glowRect : m_glowRects)
        {
            glowRect.draw(t_target);
        }

        for (const sf::Sprite & sprite : m_spellSprites)
        {
            t_target.draw(sprite, t_states);
        }

        for (const sf::FloatRect & rect : m_fadeRects)
        {
            util::drawRectangleShape(t_target, rect, true, sf::Color(0, 0, 0, 200));
        }

        m_windowFrame.draw(t_target, t_states);

        t_target.draw(m_nameText, t_states);
    }

    GuiWindowInfo SpellSelectMenu::makeGuiWindowInfo(const sf::FloatRect & region) const
    {
        GuiWindowInfo info;
        info.border = GuiWindowBorder::Fancy;
        info.region = region;
        return info;
    }

} // namespace bramblefore
