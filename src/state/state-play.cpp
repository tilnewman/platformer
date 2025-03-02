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
#include "util/sfml-defaults.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace bramblefore
{

    PlayState::PlayState()
        : m_spellSelectMenu{}
        , m_isPaused{ false }
        , m_pauseText{ util::SfmlDefaults::instance().font() }
        , m_pauseFadeVerts{}
        , m_isQuitting{ false }
        , m_quitWindow{}
    {}

    void PlayState::update(Context & t_context, const float t_frameTimeSec)
    {
        if (m_isPaused || m_isQuitting)
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
                    &m_pauseFadeVerts[0],
                    m_pauseFadeVerts.size(),
                    sf::PrimitiveType::Triangles,
                    t_states);
            }

            t_target.draw(m_pauseText, t_states);
        }
        else if (m_isQuitting)
        {
            m_quitWindow.draw(t_target, t_states);
        }
    }

    void PlayState::handleEvent(Context & t_context, const sf::Event & t_event)
    {
        if (const auto * const keyPtr = t_event.getIf<sf::Event::KeyPressed>())
        {
            // leave the quitting popup
            if (m_isQuitting)
            {
                if (keyPtr->scancode == sf::Keyboard::Scancode::L)
                {
                    m_isQuitting = false;
                    t_context.avatar.triggerDeath(t_context);
                }
                else if (keyPtr->scancode == sf::Keyboard::Scancode::G)
                {
                    m_isQuitting = false;
                    t_context.state.setChangePending(State::Credits);
                }
                else if (keyPtr->scancode == sf::Keyboard::Scancode::N)
                {
                    m_isQuitting = false;
                }

                return;
            }

            // unpause
            if (m_isPaused && (keyPtr->scancode == sf::Keyboard::Scancode::Space))
            {
                m_isPaused = false;
                t_context.sfx.play("pause");
                return;
            }

            if (!m_isPaused && !m_isQuitting && (keyPtr->scancode == sf::Keyboard::Scancode::Space))
            {
                m_isPaused = true;
                t_context.sfx.stop("walk");
                t_context.sfx.play("pause");
            }
            else if (
                !m_isQuitting && !m_isPaused && (keyPtr->scancode == sf::Keyboard::Scancode::Q))
            {
                m_isQuitting = true;
                t_context.sfx.stop("walk");
                t_context.sfx.play("ui-select-thock-slide");
            }
            else if (keyPtr->scancode == sf::Keyboard::Scancode::T)
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
                isWizard(t_context.player.avatarType()) &&
                util::keys::isNumberKey(keyPtr->scancode))
            {
                m_spellSelectMenu.setup(
                    t_context, util::keys::toNumberOpt<std::size_t>(keyPtr->scancode).value());
            }
        }
    }

    void PlayState::onEnter(Context & t_context)
    {
        m_spellSelectMenu.loadTextures(t_context.settings);
        t_context.avatar.setup(t_context);
        t_context.player_display.setup(t_context);
        t_context.level.load(t_context);

        m_pauseText = t_context.font.makeText(
            Font::Title, FontSize::Huge, "PAUSED", sf::Color(220, 220, 220));

        const sf::FloatRect screenRect{ t_context.layout.wholeRect() };
        util::centerInside(m_pauseText, screenRect);

        util::appendTriangleVerts(screenRect, m_pauseFadeVerts, sf::Color(0, 0, 0, 127));

        m_quitWindow.loadTextures(t_context.settings);

        GuiWindowInfo quitWindowInfo;
        quitWindowInfo.border  = GuiWindowBorder::Fancy;
        quitWindowInfo.content = "Restart (l)evel, quit the (g)ame, or (n)evermind?";
        quitWindowInfo.details = TextDetails(Font::General, FontSize::Medium, sf::Color::Black);
        quitWindowInfo.region  = util::scaleRectInPlaceCopy(screenRect, 0.2f);
        quitWindowInfo.title   = "Quit?";
        quitWindowInfo.will_draw_background   = true;
        quitWindowInfo.will_fade_whole_screen = true;

        m_quitWindow.arrange(t_context, quitWindowInfo);
    }

    void PlayState::onExit(Context & t_context) { t_context.level.reset(); }

} // namespace bramblefore
