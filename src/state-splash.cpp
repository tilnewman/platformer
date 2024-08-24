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
        : m_texture()
        , m_sprite()
        , m_text()
        , m_elpasedTimeSec(0.0f)
    {}

    void SplashState::onEnter(Context & context)
    {
        m_texture.loadFromFile(
            (context.settings.media_path / "image/splash/characters.png").string());

        m_sprite.setTexture(m_texture);

        sf::FloatRect rect{ context.layout.wholeRect() };
        util::scaleRectInPlace(rect, 0.5f);
        util::fitAndCenterInside(m_sprite, rect);
        m_sprite.setPosition(m_sprite.getPosition().x, (context.layout.wholeRect().height * 0.1f));

        m_text = context.font.makeText(
            Font::Default, FontSize::Huge, "Some Damn Game", sf::Color(220, 220, 220));

        util::centerInside(m_text, context.layout.wholeRect());

        m_text.move(0.0f, m_text.getGlobalBounds().height);
    }

    void SplashState::update(Context & context, const float frameTimeSec)
    {
        m_elpasedTimeSec += frameTimeSec;
        if (m_elpasedTimeSec > 3.0f)
        {
            context.state.setChangePending(State::Play);
        }
    }

    void SplashState::draw(Context &, sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_sprite, states);
        target.draw(m_text, states);
    }

    void SplashState::handleEvent(Context & context, const sf::Event & event)
    {
        if (event.type != sf::Event::KeyPressed)
        {
            return;
        }

        context.state.setChangePending(State::Play);
    }

} // namespace platformer
