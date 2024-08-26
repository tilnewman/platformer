#ifndef STATE_PLAY_HPP_INCLUDED
#define STATE_PLAY_HPP_INCLUDED
//
// state-play.hpp
//
#include "state/states.hpp"
#include "ui/spell-select-menu.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sf
{
    class RenderTarget;
    class Event;
} // namespace sf

namespace platformer
{

    struct Context;

    //

    class PlayState : public IState
    {
      public:
        PlayState();
        virtual ~PlayState() override = default;

        State which() const final { return State::Play; }
        void update(Context & t_context, const float t_frameTimeSec) final;

        void draw(Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void handleEvent(Context & t_context, const sf::Event & t_event) final;
        void onEnter(Context & t_context) final;
        void onExit(Context & t_context) final;

      private:
        SpellSelectMenu m_spellSelectMenu;
    };

} // namespace platformer

#endif // STATE_PLAY_HPP_INCLUDED
