// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// font.hpp
//
#include "subsystem/font.hpp"

#include "bramblefore/settings.hpp"
#include "util/sfml-util.hpp"

#include <iostream>

namespace bramblefore
{

    FontManager::FontManager()
        : m_defaultFont{}
        , m_fontExtentHuge{}
        , m_fontExtentLarge{}
        , m_fontExtentMedium{}
        , m_fontExtentSmall{}
    {}

    void FontManager::setup(const Settings & t_settings)
    {
        m_defaultFont.loadFromFile((t_settings.media_path / "font/mops-antiqua.ttf").string());
        setupFontExtents(t_settings);
    }

    sf::Text FontManager::makeText(
        const Font t_font,
        const FontSize t_size,
        const std::string & t_string,
        const sf::Color & t_color,
        const sf::Text::Style t_style) const
    {
        sf::Text text(t_string, get(t_font), extent(t_size).char_size);
        text.setFillColor(t_color);
        text.setStyle(t_style);
        util::setOriginToPosition(text);
        return text;
    }

    FontExtent FontManager::extent(const FontSize t_size) const noexcept
    {
        if (FontSize::Huge == t_size)
        {
            return m_fontExtentHuge;
        }
        else if (FontSize::Large == t_size)
        {
            return m_fontExtentLarge;
        }
        else if (FontSize::Medium == t_size)
        {
            return m_fontExtentMedium;
        }
        else
        {
            return m_fontExtentSmall;
        }
    }

    void FontManager::setupFontExtents(const Settings & t_settings)
    {
        // all the magic numbers in this function are based on trial and error
        const float standardRes = std::sqrt(3840.f * 2400.0f);

        const float currentRes = std::sqrt(
            static_cast<float>(t_settings.video_mode.width * t_settings.video_mode.height));

        const float ratioRes = (currentRes / standardRes);

        sf::Text text;
        const std::string widthStr{ "M" };
        const std::string heightStr{ "|g" };

        m_fontExtentHuge.char_size     = static_cast<unsigned>(200.0f * ratioRes);
        text                           = makeText(Font::Default, FontSize::Huge, widthStr);
        m_fontExtentHuge.letter_size.x = text.getGlobalBounds().width;
        text                           = makeText(Font::Default, FontSize::Huge, heightStr);
        m_fontExtentHuge.letter_size.y = text.getGlobalBounds().height;

        m_fontExtentLarge.char_size     = static_cast<unsigned>(90.0f * ratioRes);
        text                            = makeText(Font::Default, FontSize::Large, widthStr);
        m_fontExtentLarge.letter_size.x = text.getGlobalBounds().width;
        text                            = makeText(Font::Default, FontSize::Large, heightStr);
        m_fontExtentLarge.letter_size.y = text.getGlobalBounds().height;

        m_fontExtentMedium.char_size     = static_cast<unsigned>(60.0f * ratioRes);
        text                             = makeText(Font::Default, FontSize::Medium, widthStr);
        m_fontExtentMedium.letter_size.x = text.getGlobalBounds().width;
        text                             = makeText(Font::Default, FontSize::Medium, heightStr);
        m_fontExtentMedium.letter_size.y = text.getGlobalBounds().height;

        m_fontExtentSmall.char_size     = static_cast<unsigned>(40.0f * ratioRes);
        text                            = makeText(Font::Default, FontSize::Small, widthStr);
        m_fontExtentSmall.letter_size.x = text.getGlobalBounds().width;
        text                            = makeText(Font::Default, FontSize::Small, heightStr);
        m_fontExtentSmall.letter_size.y = text.getGlobalBounds().height;
    }

} // namespace bramblefore
