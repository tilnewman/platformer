// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// font.hpp
//
#include "subsystem/framerate-display.hpp"

#include "bramblefore/settings.hpp"
#include "context.hpp"
#include "subsystem/font.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"

namespace bramblefore
{

    FramerateDisplay::FramerateDisplay()
        : m_updateDisplayPeriodSec{ 1.0f }
        , m_statsDisplaySize{ 500, 200 }
        , m_fpsValues{}
        , m_elapsedTimeSec{ 0.0f }
        , m_statsDisplayUPtr{}
        , m_framerateTextUPtr{}
    {
        m_fpsValues.reserve(60);
    }

    void FramerateDisplay::setup(const Context &)
    {
        m_framerateTextUPtr = std::make_unique<sf::Text>(util::SfmlDefaults::instance().font());
    }

    void FramerateDisplay::handleElapsedFrame(
        const Context & t_context, const float t_elapsedTimeSec)
    {
        m_fpsValues.push_back(static_cast<std::size_t>(1.0f / t_elapsedTimeSec));

        m_elapsedTimeSec += (1.0f / t_context.settings.frame_rate);
        if (m_elapsedTimeSec > m_updateDisplayPeriodSec)
        {
            m_elapsedTimeSec -= m_updateDisplayPeriodSec;

            if (t_context.settings.will_display_fps)
            {
                const auto stats{ util::makeStats(m_fpsValues) };

                *m_framerateTextUPtr = t_context.font.makeText(
                    Font::General,
                    FontSize::Medium,
                    stats.toString(),
                    sf::Color(255, 255, 255, 127));

                m_framerateTextUPtr->setPosition(
                    { (t_context.layout.wholeRect().size.x -
                       m_framerateTextUPtr->getGlobalBounds().size.x),
                      0.0f });
            }

            if (t_context.settings.will_display_fps_graph)
            {
                m_statsDisplayUPtr = std::make_unique<util::GraphDisplay<std::size_t>>(
                    m_fpsValues, m_statsDisplaySize, std::uint8_t(64));
            }

            m_fpsValues.clear();
        }
    }

    void FramerateDisplay::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        if (t_context.settings.will_display_fps_graph && m_statsDisplayUPtr)
        {
            // TODO keep the size as a var
            const sf::Vector2f pos{ (t_context.layout.wholeRect().size.x -
                                     static_cast<float>(m_statsDisplaySize.x)),
                                    0.0f };

            m_statsDisplayUPtr->draw(pos, t_target, t_states);
        }

        if (t_context.settings.will_display_fps)
        {
            t_target.draw(*m_framerateTextUPtr, t_states);
        }
    }

} // namespace bramblefore
