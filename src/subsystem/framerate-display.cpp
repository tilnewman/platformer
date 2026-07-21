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
        : m_frameCount{ 0 }
        , m_elapsedTimeSec{ 0.0f }
        , m_framerateTextUPtr{}
    {}

    void FramerateDisplay::setup(const Context &)
    {
        m_framerateTextUPtr = std::make_unique<sf::Text>(util::SfmlDefaults::instance().font());
    }

    void FramerateDisplay::update(const Context & t_context, const float t_elapsedTimeSec)
    {
        ++m_frameCount;

        m_elapsedTimeSec += t_elapsedTimeSec;
        if (m_elapsedTimeSec > 1.0f)
        {
            m_elapsedTimeSec -= 1.0f;

            if (t_context.settings.will_display_fps)
            {
                std::string text{ "fps " };
                text += std::to_string(m_frameCount);

                *m_framerateTextUPtr = t_context.font.makeText(
                    Font::General, FontSize::Medium, text, sf::Color(255, 255, 255, 127));

                m_framerateTextUPtr->setPosition({ 50.0f, 0.0f });
            }

            m_frameCount = 0;
        }
    }

    void FramerateDisplay::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        if (t_context.settings.will_display_fps)
        {
            t_target.draw(*m_framerateTextUPtr, t_states);
        }
    }

} // namespace bramblefore
