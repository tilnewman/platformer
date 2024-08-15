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

    PlayState::PlayState() {}

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
    }

    void PlayState::handleEvent(Context & context, const sf::Event & event)
    {
        if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::T))
        {
            context.avatar.changeType();
        }
    }

    void PlayState::onEnter(Context & context) { context.level.load(context); }

    void PlayState::onExit(Context &) {}

} // namespace platformer
