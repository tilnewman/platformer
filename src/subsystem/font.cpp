// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// font.hpp
//
#include "subsystem/font.hpp"

#include "bramblefore/settings.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-util.hpp"

#include <iostream>

namespace bramblefore
{

    TextDetails::TextDetails()
        : font{ Font::General }
        , size{ FontSize::Medium }
        , color{ sf::Color::White }
        , style{ sf::Text::Regular }
    {}

    TextDetails::TextDetails(
        const Font t_font,
        const FontSize t_size,
        const sf::Color & t_color ,
        const sf::Text::Style t_style)
        : font{ t_font }
        , size{ t_size }
        , color{ t_color }
        , style{ t_style }
    {}

    //

    FontManager::FontManager()
        : m_titleFont{}
        , m_generalFont{}
        , m_titleExtents{}
        , m_generalExtents{}
    {}

    void FontManager::setup(const Settings & t_settings)
    {
        M_CHECK(
            m_titleFont.openFromFile((t_settings.media_path / "font/mops-antiqua.ttf").string()),
            "file not found");

        setupFontExtents(t_settings, Font::Title, m_titleExtents);

        M_CHECK(
            m_generalFont.openFromFile((t_settings.media_path / "font/et-bembo.ttf").string()),
            "file not found");

        setupFontExtents(t_settings, Font::General, m_generalExtents);
    }

    sf::Text FontManager::makeText(
        const Font t_font,
        const FontSize t_size,
        const std::string & t_string,
        const sf::Color & t_color,
        const sf::Text::Style t_style) const
    {
        sf::Text text(get(t_font), t_string, extent(t_font, t_size).char_size);
        text.setFillColor(t_color);
        text.setStyle(t_style);
        util::setOriginToPosition(text);

        return text;
    }

    FontExtent FontManager::extent(const Font t_font, const FontSize t_size) const noexcept
    {
        if (t_font == Font::Title)
        {
            if (FontSize::Huge == t_size)
            {
                return m_titleExtents.huge;
            }
            else if (FontSize::Large == t_size)
            {
                return m_titleExtents.large;
            }
            else if (FontSize::Medium == t_size)
            {
                return m_titleExtents.medium;
            }
            else
            {
                return m_titleExtents.small;
            }
        }
        else
        {
            if (FontSize::Huge == t_size)
            {
                return m_generalExtents.huge;
            }
            else if (FontSize::Large == t_size)
            {
                return m_generalExtents.large;
            }
            else if (FontSize::Medium == t_size)
            {
                return m_generalExtents.medium;
            }
            else
            {
                return m_generalExtents.small;
            }
        }
    }

    void FontManager::setupFontExtents(
        const Settings & t_settings, const Font t_font, FontExtentSet & t_extentSet)
    {
        // All the magic numbers in this function are based on trial and error.
        // There was nothing magical about it...
        const float standardRes{ std::sqrt(3840.f * 2400.0f) };

        const float currentRes{ std::sqrt(
            static_cast<float>(t_settings.video_mode.size.x * t_settings.video_mode.size.y)) };

        const float ratioRes{ currentRes / standardRes };

        const std::string widthStr{ "M" };
        const std::string heightStr{ "|g" };

        t_extentSet.huge.char_size = static_cast<unsigned>(200.0f * ratioRes);

        t_extentSet.huge.letter_size.x =
            makeText(t_font, FontSize::Huge, widthStr).getGlobalBounds().size.x;

        t_extentSet.huge.letter_size.y =
            makeText(t_font, FontSize::Huge, heightStr).getGlobalBounds().size.y;

        t_extentSet.large.char_size = static_cast<unsigned>(90.0f * ratioRes);

        t_extentSet.large.letter_size.x =
            makeText(t_font, FontSize::Large, widthStr).getGlobalBounds().size.x;

        t_extentSet.large.letter_size.y =
            makeText(t_font, FontSize::Large, heightStr).getGlobalBounds().size.y;

        t_extentSet.medium.char_size = static_cast<unsigned>(60.0f * ratioRes);

        t_extentSet.medium.letter_size.x =
            makeText(t_font, FontSize::Medium, widthStr).getGlobalBounds().size.x;

        t_extentSet.medium.letter_size.y =
            makeText(t_font, FontSize::Medium, heightStr).getGlobalBounds().size.y;

        t_extentSet.small.char_size = static_cast<unsigned>(40.0f * ratioRes);

        t_extentSet.small.letter_size.x =
            makeText(t_font, FontSize::Small, widthStr).getGlobalBounds().size.x;

        t_extentSet.small.letter_size.y =
            makeText(t_font, FontSize::Small, heightStr).getGlobalBounds().size.y;
    }

} // namespace bramblefore
