// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-level-complete.hpp
//
#include "state/state-level-complete.hpp"

#include "bramblefore/settings.hpp"
#include "state/state-manager.hpp"
#include "subsystem/context.hpp"
#include "subsystem/font.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    LevelCompleteState::LevelCompleteState()
        : m_texture{}
        , m_sprite{ m_texture }
        , m_text{ util::SfmlDefaults::instance().font() }
        , m_elapsedTimeSec{ 0.0f }
    {}

    void LevelCompleteState::onEnter(Context & t_context)
    {
        util::TextureLoader::load(
            m_texture, (t_context.settings.media_path / "image/splash/knight-win.png"), true);

        m_sprite.setTexture(m_texture, true);

        const sf::FloatRect wholeRect{ t_context.layout.wholeRect() };
        util::fitAndCenterInside(m_sprite, util::scaleRectInPlaceCopy(wholeRect, 0.3f));
        m_sprite.move({ 0.0f, -(wholeRect.size.y * 0.1f) });

        //

        m_text = t_context.font.makeText(
            Font::Title, FontSize::Huge, "You Survived!", sf::Color(220, 220, 220));

        m_text.setPosition({ ((wholeRect.size.x * 0.5f) - (m_text.getGlobalBounds().size.x * 0.5f)),
                             (util::bottom(m_sprite) + (wholeRect.size.y * 0.015f)) });
    }

    void LevelCompleteState::update(Context & t_context, const float t_frameTimeSec)
    {
        m_elapsedTimeSec += t_frameTimeSec;
        if (m_elapsedTimeSec > 4.0f)
        {
            t_context.state.setChangePending(State::Credits);
        }
    }

    void LevelCompleteState::draw(
        Context &, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_target.draw(m_sprite, t_states);
        t_target.draw(m_text, t_states);
    }

} // namespace bramblefore
