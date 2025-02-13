#ifndef FONT_HPP_INCLUDED
#define FONT_HPP_INCLUDED
//
// font.hpp
//
#include <cstddef>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

namespace bramblefore
{

    struct Context;
    struct Settings;

    //

    enum class Font
    {
        Default // mops-antiqua
    };

    //

    // restricting fonts to a subset of sizes saves vram and runtime
    enum class FontSize
    {
        Huge,
        Large,
        Medium,
        Small
    };

    //

    struct TextDetails
    {
        TextDetails()
            : font{ Font::Default }
            , size{ FontSize::Medium }
            , color{ sf::Color::White }
            , style{ sf::Text::Regular }
        {}

        TextDetails(
            const Font t_font,
            const FontSize t_size,
            const sf::Color & t_color     = sf::Color::White,
            const sf::Text::Style t_style = sf::Text::Regular)
            : font{ t_font }
            , size{ t_size }
            , color{ t_color }
            , style{ t_style }
        {}

        Font font;
        FontSize size;
        sf::Color color;
        sf::Text::Style style;
    };

    //

    struct FontExtent
    {
        unsigned int char_size{ 0 };
        sf::Vector2f letter_size{};
    };

    //

    class FontManager
    {
      public:
        FontManager();

        void setup(const Settings & t_settings);

        [[nodiscard]] inline const sf::Font & get(const Font = Font::Default) const noexcept
        {
            return m_defaultFont;
        }

        [[nodiscard]] sf::Text makeText(
            const Font t_font,
            const FontSize t_size,
            const std::string & t_text,
            const sf::Color & t_color     = sf::Color::White,
            const sf::Text::Style t_style = sf::Text::Regular) const;

        [[nodiscard]] inline sf::Text
            makeText(const std::string & t_text, const TextDetails & t_details) const
        {
            return makeText(
                t_details.font, t_details.size, t_text, t_details.color, t_details.style);
        }

        [[nodiscard]] FontExtent extent(const FontSize t_size) const noexcept;

      private:
        void setupFontExtents(const Settings & t_settings);

      private:
        sf::Font m_defaultFont;
        FontExtent m_fontExtentHuge;
        FontExtent m_fontExtentLarge;
        FontExtent m_fontExtentMedium;
        FontExtent m_fontExtentSmall;
    };

} // namespace bramblefore

#endif // FONT_HPP_INCLUDED
