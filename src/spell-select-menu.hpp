#ifndef SPELL_SELECT_MENU_HPP_INCLUDED
#define SPELL_SELECT_MENU_HPP_INCLUDED
//
// spell-select-menu.hpp
//
#include "glow-rect.hpp"
#include "gui-window.hpp"
#include "player-info.hpp"
#include "sliders.hpp"

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

        void setup(Context & context, const std::size_t selection);

        inline bool isVisible() const { return m_isVisible; }

        void update(Context & context, const float frameTimeSec);
        void draw(sf::RenderTarget & target, sf::RenderStates states) const;

      private:
        bool m_isVisible;
        float m_elapsedTimeSec;
        std::vector<sf::Sprite> m_spellSprites;
        std::vector<GlowRect> m_glowRects;
        std::vector<sf::FloatRect> m_fadeRects;
        GuiWindow m_windowFrame;
        util::SliderFromTo<float, float> m_slider;
        std::size_t m_selectionTarget;

    };

} // namespace platformer

#endif // SPELL_SELECT_MENU_HPP_INCLUDED
