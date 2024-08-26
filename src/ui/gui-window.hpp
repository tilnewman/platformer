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

        static void setup(const Settings & t_settings);

        void create(Context & t_context, const GuiWindowInfo & t_info);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        [[nodiscard]] inline sf::FloatRect innerRect() const noexcept { return m_innerRect; }
        [[nodiscard]] inline sf::FloatRect outerRect() const noexcept { return m_outerRect; }

      private:
        GuiWindowInfo m_info;
        sf::FloatRect m_innerRect;
        sf::FloatRect m_outerRect;
        //
        static sf::Texture m_borderTopLeftTexture;
        static sf::Texture m_borderTopRightTexture;
        static sf::Texture m_borderBotLeftTexture;
        static sf::Texture m_borderBotRightTexture;
        static sf::Texture m_borderTopTexture;
        static sf::Texture m_borderBotTexture;
        static sf::Texture m_borderLeftTexture;
        static sf::Texture m_borderRightTexture;
        //
        static sf::Texture m_smallBorderTopLeftTexture;
        static sf::Texture m_smallBorderTopRightTexture;
        static sf::Texture m_smallBorderBotLeftTexture;
        static sf::Texture m_smallBorderBotRightTexture;
        static sf::Texture m_smallBorderTopTexture;
        static sf::Texture m_smallBorderBotTexture;
        static sf::Texture m_smallBorderLeftTexture;
        static sf::Texture m_smallBorderRightTexture;
        //
        static sf::Texture m_bgTopLeftTexture;
        static sf::Texture m_bgTopRightTexture;
        static sf::Texture m_bgBotLeftTexture;
        static sf::Texture m_bgBotRightTexture;
        static sf::Texture m_bgTopTexture;
        static sf::Texture m_bgBotTexture;
        static sf::Texture m_bgLeftTexture;
        static sf::Texture m_bgRightTexture;
        //
        static sf::Texture m_tapeLeftTexture;
        static sf::Texture m_tapeRightTexture;
        static sf::Texture m_tapeMiddleTexture;

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
