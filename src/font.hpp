#ifndef FONT_HPP_INCLUDED
#define FONT_HPP_INCLUDED
//
// font.hpp
//
#include <cstddef>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

namespace platformer
{

    struct Context;
    struct Settings;

    //

    enum class Font
    {
        Default
    };

    //

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
            : font(Font::Default)
            , size(FontSize::Medium)
            , color(sf::Color::White)
            , style(sf::Text::Regular)
        {}

        TextDetails(
            const Font f,
            const FontSize s,
            const sf::Color & c      = sf::Color::White,
            const sf::Text::Style st = sf::Text::Regular)
            : font(f)
            , size(s)
            , color(c)
            , style(st)
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

    class FontManager
    {
      public:
        FontManager();

        void setup(const Settings & settings);

        const sf::Font & get(const Font font = Font::Default) const;

        const sf::Text makeText(
            const Font font,
            const FontSize size,
            const std::string & text,
            const sf::Color & color     = sf::Color::White,
            const sf::Text::Style style = sf::Text::Regular) const;

        const sf::Text makeText(const std::string & text, const TextDetails & details) const;

        const FontExtent extent(const FontSize size) const;

      private:
        void setupFontExtents(const Settings & settings);

      private:
        sf::Font m_defaultFont;

        FontExtent m_fontExtentHuge;
        FontExtent m_fontExtentLarge;
        FontExtent m_fontExtentMedium;
        FontExtent m_fontExtentSmall;
    };

} // namespace platformer

#endif // FONT_HPP_INCLUDED
