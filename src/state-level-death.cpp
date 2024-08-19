// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-level-death.hpp
//
#include "state-level-death.hpp"

#include "context.hpp"
#include "font.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "state-manager.hpp"
#include "texture-stats.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    LevelDeathState::LevelDeathState()
        : m_texture()
        , m_sprite()
        , m_text()
        , m_elapsedTimeSec(0.0f)
    {}

    void LevelDeathState::onEnter(Context & context)
    {
        const sf::FloatRect wholeRect{ context.layout.wholeRect() };

        m_texture.loadFromFile(
            (context.settings.media_path / "image/splash/knight-loose.png").string());

        TextureStats::instance().process(m_texture);
        m_texture.setSmooth(true);

        m_sprite.setTexture(m_texture);

        util::fitAndCenterInside(m_sprite, util::scaleRectInPlaceCopy(wholeRect, 0.2f));
        m_sprite.move(0.0f, -(wholeRect.height * 0.1f));

        //

        m_text = context.font.makeText(
            Font::Default, FontSize::Huge, "You Died", sf::Color(220, 220, 220));

        m_text.setPosition(
            ((wholeRect.width * 0.5f) - (m_text.getGlobalBounds().width * 0.5f)),
            (util::bottom(m_sprite) + (wholeRect.height * 0.015f)));
    }

    void LevelDeathState::update(Context & context, const float frameTimeSec)
    {
        m_elapsedTimeSec += frameTimeSec;
        if (m_elapsedTimeSec > 4.0f)
        {
            context.state.changeTo(context, State::Shutdown);
        }
    }

    void LevelDeathState::draw(Context &, sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_sprite, states);
        target.draw(m_text, states);
    }

} // namespace platformer
