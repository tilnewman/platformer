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
#include "keys.hpp"
#include "level.hpp"
#include "monster-manager.hpp"
#include "pickups.hpp"
#include "player-info-display.hpp"
#include "spells-anim.hpp"
#include "state-manager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace platformer
{

    PlayState::PlayState()
        : m_spellSelectMenu{}
    {}

    void PlayState::update(Context & context, const float frameTimeSec)
    {
        context.avatar.update(context, frameTimeSec);

        if (!context.state.isChangePending())
        {
            context.level.update(context, frameTimeSec);
            context.pickup.update(context, frameTimeSec);
            context.accent.update(context, frameTimeSec);
            context.spell.update(context, frameTimeSec);
            m_spellSelectMenu.update(context, frameTimeSec);
        }
    }

    void
        PlayState::draw(Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        context.bg_image.draw(target, states);
        context.level.draw(context, target, states);
        context.pickup.draw(context, target, states);
        context.accent.draw(context, target, states);
        context.avatar.draw(target, states);
        context.spell.draw(target, states);
        context.player_display.draw(target, states);
        m_spellSelectMenu.draw(target, states);
    }

    void PlayState::handleEvent(Context & context, const sf::Event & event)
    {
        if (event.type != sf::Event::KeyPressed)
        {
            return;
        }

        if (event.key.code == sf::Keyboard::T)
        {
            std::size_t temp{ static_cast<std::size_t>(context.player.avatarType()) };

            ++temp;
            if (temp >= static_cast<std::size_t>(AvatarType::Count))
            {
                temp = 0;
            }

            context.player.setup(static_cast<AvatarType>(temp));
            context.avatar.changeType(context);
        }
        else if (
            isSpellCaster(context.player.avatarType()) && util::keys::isNumberKey(event.key.code))
        {
            m_spellSelectMenu.setup(
                context, util::keys::toNumberOpt<std::size_t>(event.key.code).value());
        }
    }

    void PlayState::onEnter(Context & context) { context.level.load(context); }

    void PlayState::onExit(Context &) {}

} // namespace platformer
