// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-splash.hpp
//
#include "state-splash.hpp"

#include "context.hpp"
#include "font.hpp"
#include "screen-layout.hpp"
#include "sfml-util.hpp"
#include "state-manager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace platformer
{

    SplashState::SplashState()
        : m_text()
        , m_elpasedTimeSec(0.0f)
    {}

    void SplashState::onEnter(Context & context)
    {
        m_text = context.font.makeText(
            Font::Default, FontSize::Huge, "Splash", sf::Color(220, 220, 220));

        util::centerInside(m_text, context.layout.wholeRect());
    }

    void SplashState::update(Context & context, const float frameTimeSec)
    {
        m_elpasedTimeSec += frameTimeSec;
        if (m_elpasedTimeSec > 3.0f)
        {
            context.state.changeTo(context, State::Play);
        }
    }

    void SplashState::draw(Context &, sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_text, states);
    }

    void SplashState::handleEvent(Context & context, const sf::Event & event)
    {
        if (event.type != sf::Event::KeyPressed)
        {
            return;
        }

        context.state.changeTo(context, State::Play);
    }

} // namespace platformer
