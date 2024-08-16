// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-play.hpp
//
#include "state-play.hpp"

#include "accents.hpp"
#include "avatar.hpp"
#include "background-images.hpp"
#include "context.hpp"
#include "level.hpp"
#include "pickups.hpp"
#include "spells.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace platformer
{

    PlayState::PlayState()
        : m_window()
    {}

    void PlayState::update(Context & context, const float frameTimeSec)
    {
        context.avatar.update(context, frameTimeSec);
        context.level.update(context, frameTimeSec);
        context.pickup.update(context, frameTimeSec);
        context.accent.update(context, frameTimeSec);
        context.spell.update(context, frameTimeSec);
    }

    void
        PlayState::draw(Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        context.bg_image.draw(target, states);
        context.level.draw(context, target, states);
        context.pickup.draw(context, target, states);
        context.accent.draw(context, target, states);
        context.avatar.draw(target, states);
        context.spell.draw(context, target, states);
        m_window.draw(target, states);
    }

    void PlayState::handleEvent(Context & context, const sf::Event & event)
    {
        if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::T))
        {
            context.avatar.changeType();
        }
    }

    void PlayState::onEnter(Context & context)
    {
        context.level.load(context);

        m_window.setup(context.settings);

        GuiWindowInfo info;
        info.title = "Wolf Nipple Chips";

        info.content =
            "These are the most tender best tasting wolf nipples you'll ever find. Salt is "
            "free, red pepper costs extra. Cut you a deal on a dozen?";

        info.details.color = sf::Color(246, 199, 68);
        info.details.size  = FontSize::Small;
        info.inner_size    = { 200.0f, 200.0f };
        info.border        = GuiWindowBorder::Fancy;

        m_window.create(context, info);
    }

    void PlayState::onExit(Context &) {}

} // namespace platformer
