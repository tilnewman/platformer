#ifndef SPELL_SELECT_MENU_HPP_INCLUDED
#define SPELL_SELECT_MENU_HPP_INCLUDED
//
// spell-select-menu.hpp
//
#include "glow-rect.hpp"
#include "gui-window.hpp"
#include "player-info.hpp"

#include <vector>

#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
} // namespace sf

namespace platformer
{

    struct Context;

    //

    class SpellSelectMenu
    {
      public:
        SpellSelectMenu();

        void setup(Context & context);

        inline void isVisible(const bool isVisible) { m_isVisible = isVisible; }
        inline bool isVisible() const { return m_isVisible; }
        
        void draw(sf::RenderTarget & target, sf::RenderStates states) const;

      private:
        bool m_isVisible;
        std::vector<sf::Sprite> m_spellSprites;
        std::vector<GlowRect> m_glowRects;
        std::vector<sf::FloatRect> m_fadeRects;
        std::vector<GuiWindow> m_windows;
    };

} // namespace platformer

#endif // SPELL_SELECT_MENU_HPP_INCLUDED
