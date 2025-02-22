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
#include "subsystem/font.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/sfml-keys.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace bramblefore
{

    PlayState::PlayState()
        : m_spellSelectMenu{}
        , m_isPaused{ false }
        , m_pauseText{}
        , m_pauseFadeVerts{}
    {}

    void PlayState::update(Context & t_context, const float t_frameTimeSec)
    {
        if (m_isPaused)
        {
            return;
        }

        t_context.avatar.update(t_context, t_frameTimeSec);

        if (!t_context.state.isChangePending())
        {
            t_context.player.update(t_context, t_frameTimeSec);
            t_context.level.update(t_context, t_frameTimeSec);
            t_context.pickup.update(t_context, t_frameTimeSec);
            t_context.accent.update(t_context, t_frameTimeSec);
            t_context.spell.update(t_context, t_frameTimeSec);
            t_context.float_text.update(t_context, t_frameTimeSec);
            m_spellSelectMenu.update(t_context, t_frameTimeSec);
        }
    }

    void PlayState::draw(
        Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_context.bg_image.draw(t_target, t_states);
        t_context.level.draw(t_context, t_target, t_states);
        t_context.pickup.draw(t_context, t_target, t_states);
        t_context.accent.draw(t_context, t_target, t_states);
        t_context.avatar.draw(t_target, t_states);
        t_context.spell.draw(t_target, t_states);
        t_context.float_text.draw(t_target, t_states);
        t_context.player_display.draw(t_target, t_states);
        m_spellSelectMenu.draw(t_target, t_states);

        if (m_isPaused)
        {
            if (!m_pauseFadeVerts.empty())
            {
                t_target.draw(
                    &m_pauseFadeVerts[0], m_pauseFadeVerts.size(), sf::Triangles, t_states);
            }

            t_target.draw(m_pauseText, t_states);
        }
    }

    void PlayState::handleEvent(Context & t_context, const sf::Event & t_event)
    {
        if (t_event.type != sf::Event::KeyPressed)
        {
            return;
        }

        if (m_isPaused)
        {
            m_isPaused = false;
            t_context.sfx.play("pause");
            return;
        }

        if (t_event.key.code == sf::Keyboard::Space)
        {
            m_isPaused = true;
            t_context.sfx.play("pause");
            t_context.sfx.stop("walk");
        }
        else if (t_event.key.code == sf::Keyboard::T)
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
            isWizard(t_context.player.avatarType()) && util::keys::isNumberKey(t_event.key.code))
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

        m_pauseText = t_context.font.makeText(
            Font::Default, FontSize::Huge, "PAUSED", sf::Color(220, 220, 220));

        const sf::FloatRect screenRect{ t_context.layout.wholeRect() };
        util::centerInside(m_pauseText, screenRect);

        util::appendTriangleVerts(screenRect, m_pauseFadeVerts, sf::Color(0, 0, 0, 127));
    }

    void PlayState::onExit(Context & t_context) { t_context.level.reset(); }

} // namespace bramblefore
