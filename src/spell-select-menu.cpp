// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// spell-select-menu.hpp
//
#include "spell-select-menu.hpp"

#include "context.hpp"
#include "screen-layout.hpp"
#include "sfml-util.hpp"
#include "sound-player.hpp"
#include "spells-anim.hpp"

#include <limits>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
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
    {}

    void SpellSelectMenu::setup(Context & t_context, const std::size_t t_selectionOrig)
    {
        m_fadeRects.clear();
        m_glowRects.clear();
        m_spellSprites.clear();

        m_isVisible      = true;
        m_elapsedTimeSec = 0.0f;

        const std::vector<PlayerSpell> & playerSpells{ t_context.player.spells() };

        // players press number keys 1-9 to get here but playerSpells is zero indexed,
        // so subtract one from the number the player provides
        const std::size_t selection{ t_selectionOrig - 1 };
        
        if (m_selectionTarget != selection)
        {
            if ((selection < playerSpells.size()) && (playerSpells.at(selection).is_learned))
            {
                t_context.sfx.play("ui-select-magic");
                m_selectionTarget = selection;
                // start anim
            }
            else
            {
                t_context.sfx.play("ui-reject-2");
            }
        }

        const float iconSize{ t_context.layout.wholeSize().y * 0.1f };

        sf::Vector2f pos{ t_context.layout.wholeSize() * sf::Vector2f{ 0.333f, 0.1f } };

        for (std::size_t spellIndex{ 0 }; spellIndex < playerSpells.size(); ++spellIndex)
        {
            const PlayerSpell & playerSpell{ playerSpells.at(spellIndex) };
            const sf::FloatRect rect{ pos.x, pos.y, iconSize, iconSize };

            sf::Sprite & sprite{ m_spellSprites.emplace_back() };
            sprite.setTexture(t_context.spell.iconTexture(playerSpell.spell));
            util::fitAndCenterInside(sprite, rect);

            //

            if ((selection == spellIndex) && (playerSpell.is_learned))
            {
                GuiWindowInfo info;
                info.border = GuiWindowBorder::Small;
                info.region = rect;
                m_windowFrame.create(t_context, info);
            }

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
    }

    void SpellSelectMenu::update(Context &, const float t_frameTimeSec)
    {
        m_elapsedTimeSec += t_frameTimeSec;
        if (m_elapsedTimeSec > 4.0f)
        {
            m_isVisible = false;
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
    }

} // namespace platformer
