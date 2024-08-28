// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-play.hpp
//
#include "state/state-play.hpp"

#include "avatar/avatar.hpp"
#include "avatar/spells-anim.hpp"
#include "map/accents.hpp"
#include "map/level.hpp"
#include "map/pickups.hpp"
#include "monster/monster-manager.hpp"
#include "player/player-info-display.hpp"
#include "state/state-manager.hpp"
#include "subsystem/background-images.hpp"
#include "subsystem/context.hpp"
#include "subsystem/floating-text.hpp"
#include "util/sfml-keys.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace bramblefore
{

    PlayState::PlayState()
        : m_spellSelectMenu{}
    {}

    void PlayState::update(Context & t_context, const float t_frameTimeSec)
    {
        t_context.avatar.update(t_context, t_frameTimeSec);

        if (!t_context.state.isChangePending())
        {
            t_context.level.update(t_context, t_frameTimeSec);
            t_context.pickup.update(t_context, t_frameTimeSec);
            t_context.accent.update(t_context, t_frameTimeSec);
            t_context.spell.update(t_context, t_frameTimeSec);
            t_context.float_text.update(t_context, t_frameTimeSec);
            m_spellSelectMenu.update(t_context, t_frameTimeSec);
        }
    }

    void PlayState::draw(
        Context & t_context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        t_context.bg_image.draw(target, states);
        t_context.level.draw(t_context, target, states);
        t_context.pickup.draw(t_context, target, states);
        t_context.accent.draw(t_context, target, states);
        t_context.avatar.draw(target, states);
        t_context.spell.draw(target, states);
        t_context.float_text.draw(target, states);
        t_context.player_display.draw(target, states);
        m_spellSelectMenu.draw(target, states);
    }

    void PlayState::handleEvent(Context & t_context, const sf::Event & t_event)
    {
        if (t_event.type != sf::Event::KeyPressed)
        {
            return;
        }

        if (t_event.key.code == sf::Keyboard::T)
        {
            std::size_t temp{ static_cast<std::size_t>(t_context.player.avatarType()) };

            ++temp;
            if (temp >= static_cast<std::size_t>(AvatarType::Count))
            {
                temp = 0;
            }

            t_context.player.setup(t_context, static_cast<AvatarType>(temp));
            t_context.avatar.changeType(t_context);
        }
        else if (
            isSpellCaster(t_context.player.avatarType()) &&
            util::keys::isNumberKey(t_event.key.code))
        {
            m_spellSelectMenu.setup(
                t_context, util::keys::toNumberOpt<std::size_t>(t_event.key.code).value());
        }
    }

    void PlayState::onEnter(Context & t_context)
    {
        m_spellSelectMenu.loadTextures(t_context.settings);
        t_context.avatar.setup(t_context);
        t_context.player_display.setup(t_context);
        t_context.level.load(t_context);
    }

    void PlayState::onExit(Context & t_context) { t_context.level.reset(); }

} // namespace bramblefore
