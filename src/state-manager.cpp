// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-manager.hpp
//
#include "state-manager.hpp"

#include "state-level-complete.hpp"
#include "state-level-death.hpp"
#include "state-play.hpp"
#include "state-splash.hpp"

#include <exception>

namespace platformer
{

    StateManager::StateManager()
        : m_currentStateUPtr{ factory(State::Startup) }
        , m_changePendingOpt{ std::nullopt }
    {}

    void StateManager::setChangePending(const State t_state) noexcept
    {
        m_changePendingOpt = t_state;
    }

    void StateManager::changeIfPending(Context & t_context)
    {
        if (!m_changePendingOpt)
        {
            return;
        }

        m_currentStateUPtr->onExit(t_context);

        m_currentStateUPtr = factory(m_changePendingOpt.value());
        m_changePendingOpt = std::nullopt;

        m_currentStateUPtr->onEnter(t_context);
    }

    std::unique_ptr<IState> StateManager::factory(const State state)
    {
        // clang-format off
        switch (state)
        {
            case State::Startup:        { return std::make_unique<StartupState>();       }
            case State::Splash:         { return std::make_unique<SplashState>();        }
            case State::Play:           { return std::make_unique<PlayState>();          }
            case State::LevelComplete:  { return std::make_unique<LevelCompleteState>(); }
            case State::LevelDeath:     { return std::make_unique<LevelDeathState>();    }
            case State::Shutdown:       { return std::make_unique<ShutdownState>();      }
            default:                    
            { 
                throw std::runtime_error("StateManager::factory() given an unknown State.");
            }

        }
        // clang-format on
    }

} // namespace platformer
