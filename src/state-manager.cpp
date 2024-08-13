// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-manager.hpp
//
#include "state-manager.hpp"

#include "state-level-complete.hpp"
#include "state-play.hpp"
#include "state-splash.hpp"

#include <exception>

namespace platformer
{

    StateManager::StateManager()
        : m_currentStateUPtr(factory(State::Startup))
    {}

    void StateManager::changeTo(Context & context, const State newState)
    {
        m_currentStateUPtr->onExit(context);
        m_currentStateUPtr = factory(newState);
        m_currentStateUPtr->onEnter(context);
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
            case State::Shutdown:       { return std::make_unique<ShutdownState>();      }
            default:                    
            { 
                throw std::runtime_error("StateManager::factory() given an unknown State.");
            }

        }
        // clang-format on
    }

} // namespace platformer
