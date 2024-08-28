// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-splash.hpp
//
#include "state/state-splash.hpp"

#include "bramblefore/settings.hpp"
#include "state/state-manager.hpp"
#include "subsystem/context.hpp"
#include "subsystem/font.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/sfml-util.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace bramblefore
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

        sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };
        util::scaleRectInPlace(wholeScreenRect, 0.4f);
        util::fitAndCenterInside(m_sprite, wholeScreenRect);

        m_sprite.setPosition(
            m_sprite.getPosition().x, (t_context.layout.wholeRect().height * 0.15f));

        m_text = t_context.font.makeText(
            Font::Default, FontSize::Huge, "Bramblefore", sf::Color(220, 220, 220));

        util::centerInside(m_text, t_context.layout.wholeRect());
        m_text.setPosition(m_text.getPosition().x, util::bottom(m_sprite));
    }

    void SplashState::update(Context & t_context, const float t_frameTimeSec)
    {
        m_elpasedTimeSec += t_frameTimeSec;
        if (m_elpasedTimeSec > 4.0f)
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

} // namespace bramblefore
