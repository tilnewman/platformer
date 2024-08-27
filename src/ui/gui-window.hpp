#ifndef GUI_WINDOW_HPP_INCLUDED
#define GUI_WINDOW_HPP_INCLUDED
//
// gui-window.hpp
//
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
        sf::FloatRect region{};
        GuiWindowBorder border{ GuiWindowBorder::None };
        bool will_draw_background{ false };
        bool will_fade_whole_screen{ false };
    };

    //

    class GuiWindow
    {
      public:
        GuiWindow();

        void loadTextures(const Settings & t_settings);

        void arrange(Context & t_context, const GuiWindowInfo & t_info);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        [[nodiscard]] inline sf::FloatRect innerRect() const noexcept { return m_innerRect; }
        [[nodiscard]] inline sf::FloatRect outerRect() const noexcept { return m_outerRect; }

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
