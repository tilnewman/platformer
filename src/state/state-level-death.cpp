// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-level-death.hpp
//
#include "state/state-level-death.hpp"

#include "bramblefore/settings.hpp"
#include "state/state-manager.hpp"
#include "subsystem/context.hpp"
#include "subsystem/font.hpp"
#include "subsystem/screen-layout.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/sfml-util.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    LevelDeathState::LevelDeathState()
        : m_texture{}
        , m_sprite{}
        , m_text{}
        , m_elapsedTimeSec{ 0.0f }
    {}

    void LevelDeathState::onEnter(Context & t_context)
    {
        const sf::FloatRect wholeRect{ t_context.layout.wholeRect() };

        m_texture.loadFromFile(
            (t_context.settings.media_path / "image/splash/knight-loose.png").string());

        TextureStats::instance().process(m_texture);
        m_texture.setSmooth(true);

        m_sprite.setTexture(m_texture);

        util::fitAndCenterInside(m_sprite, util::scaleRectInPlaceCopy(wholeRect, 0.2f));
        m_sprite.move(0.0f, -(wholeRect.height * 0.1f));

        //

        m_text = t_context.font.makeText(
            Font::Default, FontSize::Huge, "You Died", sf::Color(220, 220, 220));

        m_text.setPosition(
            ((wholeRect.width * 0.5f) - (m_text.getGlobalBounds().width * 0.5f)),
            (util::bottom(m_sprite) + (wholeRect.height * 0.015f)));
    }

    void LevelDeathState::update(Context & t_context, const float t_frameTimeSec)
    {
        m_elapsedTimeSec += t_frameTimeSec;
        if (m_elapsedTimeSec > 4.0f)
        {
            t_context.state.setChangePending(State::Credits);
        }
    }

    void LevelDeathState::draw(
        Context &, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_target.draw(m_sprite, t_states);
        t_target.draw(m_text, t_states);
    }

} // namespace bramblefore
