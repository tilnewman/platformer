#ifndef GUI_WINDOW_PAPER_HPP_INCLUDED
#define GUI_WINDOW_PAPER_HPP_INCLUDED
//
// gui-window-paper.hpp
//
#include "gui-window.hpp"
#include "subsystem/font.hpp"

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

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

    class GuiWindowPaper
    {
      public:
        GuiWindowPaper();

        void setup(const Settings & t_settings);
        void create(Context & t_context, const bool t_useBigPaper, const GuiWindowInfo & t_info);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        [[nodiscard]] inline sf::FloatRect innerRect() const noexcept { return m_innerRect; }
        [[nodiscard]] inline sf::FloatRect outerRect() const noexcept { return m_outerRect; }

      private:
        GuiWindowInfo m_info;
        sf::FloatRect m_innerRect;
        sf::FloatRect m_outerRect;

        sf::Texture m_paper1Texture;
        sf::Texture m_paper2Texture;
        sf::Texture m_tapeLeftTexture;
        sf::Texture m_tapeRightTexture;
        sf::Texture m_tapeMiddleTexture;

        sf::Sprite m_paperSprite;
        sf::Sprite m_tapeLeftSprite;
        sf::Sprite m_tapeRightSprite;
        sf::Sprite m_tapeMiddleSprite;

        sf::Text m_titleText;
        std::vector<sf::Text> m_contentTexts;
        std::vector<sf::Vertex> m_bgFadeVerts;
    };

} // namespace bramblefore

#endif // GUI_WINDOW_PAPER_HPP_INCLUDED
