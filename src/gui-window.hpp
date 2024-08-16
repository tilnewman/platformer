#ifndef GUI_WINDOW_HPP_INCLUDED
#define GUI_WINDOW_HPP_INCLUDED
//
// gui-window.hpp
//
#include "font.hpp"

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

namespace platformer
{
    struct Context;
    struct Settings;

    //

    enum class GuiWindowBorder
    {
        None,
        Small,
        Fancy
    };

    //

    struct GuiWindowInfo
    {
        std::string title{};
        std::string content{};
        TextDetails details{};
        sf::Vector2f inner_size{};
        GuiWindowBorder border{ GuiWindowBorder::None };
    };

    //

    class GuiWindow
    {
      public:
        GuiWindow();

        void setup(const Settings & settings);
        void create(Context & context, const GuiWindowInfo & info);
        void draw(sf::RenderTarget & target, sf::RenderStates states) const;
        inline const sf::FloatRect innerRect() const { return m_innerRect; }
        inline const sf::FloatRect outerRect() const { return m_outerRect; }

      private:
        GuiWindowInfo m_info;
        sf::FloatRect m_innerRect;
        sf::FloatRect m_outerRect;
        //
        sf::Texture m_borderTopLeftTexture;
        sf::Texture m_borderTopRightTexture;
        sf::Texture m_borderBotLeftTexture;
        sf::Texture m_borderBotRightTexture;
        sf::Texture m_borderTopTexture;
        sf::Texture m_borderBotTexture;
        sf::Texture m_borderLeftTexture;
        sf::Texture m_borderRightTexture;
        //
        sf::Texture m_smallBorderTopLeftTexture;
        sf::Texture m_smallBorderTopRightTexture;
        sf::Texture m_smallBorderBotLeftTexture;
        sf::Texture m_smallBorderBotRightTexture;
        sf::Texture m_smallBorderTopTexture;
        sf::Texture m_smallBorderBotTexture;
        sf::Texture m_smallBorderLeftTexture;
        sf::Texture m_smallBorderRightTexture;
        //
        sf::Texture m_bgTopLeftTexture;
        sf::Texture m_bgTopRightTexture;
        sf::Texture m_bgBotLeftTexture;
        sf::Texture m_bgBotRightTexture;
        sf::Texture m_bgTopTexture;
        sf::Texture m_bgBotTexture;
        sf::Texture m_bgLeftTexture;
        sf::Texture m_bgRightTexture;
        //
        sf::Texture m_tapeLeftTexture;
        sf::Texture m_tapeRightTexture;
        sf::Texture m_tapeMiddleTexture;

        sf::Color m_bgColor;
        sf::FloatRect m_bgCenterRect;
        std::vector<sf::Vertex> m_bgCenterVerts;
        std::vector<sf::Sprite> m_sprites;
        sf::Text m_titleText;
        std::vector<sf::Text> m_contentTexts;
        std::vector<sf::Vertex> m_bgFadeVerts;
    };

} // namespace platformer

#endif // GUI_WINDOW_HPP_INCLUDED
