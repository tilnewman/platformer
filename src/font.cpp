// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// font.hpp
//
#include "font.hpp"

#include "settings.hpp"
#include "sfml-util.hpp"

#include <exception>

namespace platformer
{

    FontManager::FontManager()
        : m_defaultFont()
        , m_fontExtentHuge()
        , m_fontExtentLarge()
        , m_fontExtentMedium()
        , m_fontExtentSmall()
    {}

    void FontManager::setup(const Settings & settings)
    {
        m_defaultFont.loadFromFile((settings.media_path / "font/mops-antiqua.ttf").string());

        setupFontExtents(settings);
    }

    const sf::Font & FontManager::get(const Font) const { return m_defaultFont; }

    const sf::Text FontManager::makeText(
        const Font font,
        const FontSize size,
        const std::string & string,
        const sf::Color & color,
        const sf::Text::Style style) const
    {
        sf::Text text(string, get(font), extent(size).char_size);
        text.setFillColor(color);
        text.setStyle(style);
        util::setOriginToPosition(text);
        return text;
    }

    const sf::Text
        FontManager::makeText(const std::string & text, const TextDetails & details) const
    {
        return makeText(details.font, details.size, text, details.color, details.style);
    }

    const FontExtent FontManager::extent(const FontSize size) const
    {
        if (size == FontSize::Small)
        {
            return m_fontExtentSmall;
        }
        else if (size == FontSize::Medium)
        {
            return m_fontExtentMedium;
        }
        else if (size == FontSize::Large)
        {
            return m_fontExtentLarge;
        }
        else if (size == FontSize::Huge)
        {
            return m_fontExtentHuge;
        }
        else
        {
            throw std::runtime_error(
                "Error:  FontManager::extent() given an unknown FontSize.(  Medium will be used.");
        }
    }

    void FontManager::setupFontExtents(const Settings & settings)
    {
        // all the magic numbers in this function are based on trial and error
        const float standardRes = std::sqrt(3840.f * 2400.0f);

        const float currentRes =
            std::sqrt(static_cast<float>(settings.video_mode.width * settings.video_mode.height));

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

} // namespace platformer
