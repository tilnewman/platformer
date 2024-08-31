#ifndef STATES_HPP_INCLUDED
#define STATES_HPP_INCLUDED
//
// states.hpp
//
#include <string_view>

#include <SFML/Graphics/RenderStates.hpp>

namespace sf
{
    class RenderTarget;
    class Event;
} // namespace sf

namespace bramblefore
{

    struct Context;

    //

    enum class State
    {
        Startup,
        CharacterSelect,
        Splash,
        Play,
        LevelComplete,
        LevelDeath,
        Shutdown
    };

    [[nodiscard]] inline constexpr std::string_view toString(const State state) noexcept
    {
        // clang-format off
        switch (state)
        {
            case State::Startup:        { return "Startup";         }
            case State::CharacterSelect:{ return "CharacterSelect"; }
            case State::Splash:         { return "Splash";          }
            case State::Play:           { return "Play";            }
            case State::LevelComplete:  { return "LevelComplete";   }
            case State::LevelDeath:     { return "LevelDeath";      }
            case State::Shutdown:       { return "Shutdown";        }
            default:     { return "Error_toString()_unknown_State"; }
        }
        // clang-format on
    }

    //

    struct IState
    {
        virtual ~IState() = default;

        [[nodiscard]] virtual State which() const                                      = 0;
        virtual void update(Context & context, const float frameTimeSec)               = 0;
        virtual void draw(Context & c, sf::RenderTarget & t, sf::RenderStates s) const = 0;
        virtual void handleEvent(Context & context, const sf::Event & event)           = 0;
        virtual void onEnter(Context & context)                                        = 0;
        virtual void onExit(Context & context)                                         = 0;
    };

    //

    struct StartupState : public IState
    {
        virtual ~StartupState() override = default;

        [[nodiscard]] State which() const final { return State::Startup; }
        void update(Context &, const float) final {}
        void draw(Context &, sf::RenderTarget &, sf::RenderStates) const final {}
        void handleEvent(Context &, const sf::Event &) final {}
        void onEnter(Context &) final {}
        void onExit(Context &) final {}
    };

    //

    struct ShutdownState : public IState
    {
        virtual ~ShutdownState() override = default;

        [[nodiscard]] State which() const final { return State::Shutdown; }
        void update(Context &, const float) final {}
        void draw(Context &, sf::RenderTarget &, sf::RenderStates) const final {}
        void handleEvent(Context &, const sf::Event &) final {}
        void onEnter(Context &) final {}
        void onExit(Context &) final {}
    };

} // namespace bramblefore

#endif // STATES_HPP_INCLUDED
