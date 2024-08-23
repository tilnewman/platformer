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
    {}

    void SpellSelectMenu::setup(Context & context)
    {
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

        for (const sf::Sprite & sprite : m_spellSprites)
        {
            target.draw(sprite, states);
        }
    }

} // namespace platformer
