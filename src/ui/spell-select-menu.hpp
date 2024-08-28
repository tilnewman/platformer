#ifndef SPELL_SELECT_MENU_HPP_INCLUDED
#define SPELL_SELECT_MENU_HPP_INCLUDED
//
// spell-select-menu.hpp
//
#include "glow-rect.hpp"
#include "gui-window.hpp"
#include "player/player-info.hpp"
#include "util/sliders.hpp"

#include <vector>

#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
} // namespace sf

namespace bramblefore
{

    struct Context;
    struct Settings;

    //

    class SpellSelectMenu
    {
      public:
        SpellSelectMenu();

        void loadTextures(const Settings & t_settings);

        void setup(Context & t_context, const std::size_t t_selection);

        [[nodiscard]] inline constexpr bool isVisible() const noexcept { return m_isVisible; }

        void update(Context & t_context, const float t_frameTimeSec);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;

      private:
        GuiWindowInfo makeGuiWindowInfo(const sf::FloatRect & t_region) const;

      private:
        bool m_isVisible;
        float m_elapsedTimeSec;
        std::vector<sf::Sprite> m_spellSprites;
        std::vector<GlowRect> m_glowRects;
        std::vector<sf::FloatRect> m_fadeRects;
        GuiWindow m_windowFrame;
        util::SliderFromTo<float, float> m_slider;
        std::size_t m_selectionTarget;
        sf::FloatRect m_selectionRect;
        bool m_isMovingSelection;
        sf::Text m_nameText;
    };

} // namespace bramblefore

#endif // SPELL_SELECT_MENU_HPP_INCLUDED
