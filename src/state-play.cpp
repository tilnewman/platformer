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
        context.pickups.update(context, frameTimeSec);
        context.accents.update(context, frameTimeSec);
        context.spells.update(context, frameTimeSec);
    }

    void
        PlayState::draw(Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        context.bg_images.draw(target, states);
        context.level.draw(context, target, states);
        context.pickups.draw(context, target, states);
        context.accents.draw(context, target, states);
        context.avatar.draw(target, states);
        context.spells.draw(context, target, states);
    }

    void PlayState::handleEvent(Context &, const sf::Event &) {}

    void PlayState::onEnter(Context & context) { context.level.load(context); }

    void PlayState::onExit(Context &) {}

} // namespace platformer
