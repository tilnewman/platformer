// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// spell-select-menu.hpp
//
#include "spell-select-menu.hpp"

#include "context.hpp"
#include "screen-layout.hpp"
#include "sfml-util.hpp"
#include "spells-anim.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    SpellSelectMenu::SpellSelectMenu()
        : m_isVisible{ false }
        , m_spellSprites{}
        , m_glowRects{}
        , m_windows{}
    {}

    void SpellSelectMenu::setup(Context & context)
    {
        m_windows.clear();
        m_fadeRects.clear();
        m_glowRects.clear();
        m_spellSprites.clear();

        sf::Vector2f pos{ context.layout.wholeSize() * sf::Vector2f{ 0.333f, 0.1f } };

        const std::vector<PlayerSpell> & playerSpells{ context.player.spells() };
        for (const PlayerSpell & playerSpell : playerSpells)
        {
            sf::Sprite & sprite{ m_spellSprites.emplace_back() };
            sprite.setTexture(context.spell.iconTexture(playerSpell.spell));
            sprite.scale(2.0f, 2.0f);
            sprite.setPosition(pos);

            const sf::FloatRect bounds{ sprite.getGlobalBounds() };

            //

            GuiWindowInfo info;
            info.border = GuiWindowBorder::Small;
            info.region = bounds;

            GuiWindow & window{ m_windows.emplace_back() };
            window.setup(context.settings);
            window.create(context, info);

            //

            GlowRect & glowRect{ m_glowRects.emplace_back() };

            glowRect.setup(
                sf::Color(0, 0, 0, 200), util::scaleRectInPlaceCopy(bounds, 1.25f), bounds);

            //

            if (!playerSpell.is_learned)
            {
                m_fadeRects.push_back(bounds);
            }

            //

            pos.x += bounds.width;
            pos.x += (bounds.width * 0.5f);
        }
    }

    void SpellSelectMenu::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        if (!m_isVisible)
        {
            return;
        }

        for (const GlowRect & glowRect : m_glowRects)
        {
            glowRect.draw(target);
        }

        for (const sf::Sprite & sprite : m_spellSprites)
        {
            target.draw(sprite, states);
        }

        for (const sf::FloatRect & rect : m_fadeRects)
        {
            util::drawRectangleShape(target, rect, true, sf::Color(0, 0, 0, 200));
        }

        for (const GuiWindow & window : m_windows)
        {
            window.draw(target, states);
        }
    }

} // namespace platformer
