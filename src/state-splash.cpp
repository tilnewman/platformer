// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-splash.hpp
//
#include "state-splash.hpp"

#include "context.hpp"
#include "font.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "state-manager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace platformer
{

    SplashState::SplashState()
        : m_texture{}
        , m_sprite{}
        , m_text{}
        , m_elpasedTimeSec{ 0.0f }
    {}

    void SplashState::onEnter(Context & t_context)
    {
        m_texture.loadFromFile(
            (t_context.settings.media_path / "image/splash/characters.png").string());

        m_sprite.setTexture(m_texture);

        sf::FloatRect rect{ t_context.layout.wholeRect() };
        util::scaleRectInPlace(rect, 0.5f);
        util::fitAndCenterInside(m_sprite, rect);

        m_sprite.setPosition(
            m_sprite.getPosition().x, (t_context.layout.wholeRect().height * 0.1f));

        m_text = t_context.font.makeText(
            Font::Default, FontSize::Huge, "Bramblefore", sf::Color(220, 220, 220));

        util::centerInside(m_text, t_context.layout.wholeRect());

        m_text.move(0.0f, m_text.getGlobalBounds().height);
    }

    void SplashState::update(Context & t_context, const float t_frameTimeSec)
    {
        m_elpasedTimeSec += t_frameTimeSec;
        if (m_elpasedTimeSec > 3.0f)
        {
            t_context.state.setChangePending(State::Play);
        }
    }

    void SplashState::draw(Context &, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_target.draw(m_sprite, t_states);
        t_target.draw(m_text, t_states);
    }

    void SplashState::handleEvent(Context & t_context, const sf::Event & t_event)
    {
        if (t_event.type != sf::Event::KeyPressed)
        {
            return;
        }

        t_context.state.setChangePending(State::Play);
    }

} // namespace platformer
