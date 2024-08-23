#ifndef GUI_WINDOW_PAPER_HPP_INCLUDED
#define GUI_WINDOW_PAPER_HPP_INCLUDED
//
// gui-window-paper.hpp
//
#include "font.hpp"
#include "gui-window.hpp"

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

namespace platformer
{

    struct Context;
    struct Settings;

    //

    class GuiWindowPaper
    {
      public:
        GuiWindowPaper();

        void setup(const Settings & settings);
        void create(Context & context, const bool useBigPaper, const GuiWindowInfo & info);
        void draw(sf::RenderTarget & target, sf::RenderStates states) const;
        inline const sf::FloatRect innerRect() const { return m_innerRect; }
        inline const sf::FloatRect outerRect() const { return m_outerRect; }

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

} // namespace platformer

#endif // GUI_WINDOW_PAPER_HPP_INCLUDED
