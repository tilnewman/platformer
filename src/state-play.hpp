#ifndef STATE_PLAY_HPP_INCLUDED
#define STATE_PLAY_HPP_INCLUDED
//
// state-play.hpp
//
#include "player-info.hpp"
#include "states.hpp"

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
        void update(Context & context, const float) final;
        void draw(Context & c, sf::RenderTarget & t, sf::RenderStates s) const final;
        void handleEvent(Context & context, const sf::Event & event) final;
        void onEnter(Context & context) final;
        void onExit(Context & context) final;

      private:
        PlayerInfoDisplay m_playerInfoDisplay;
    };

} // namespace platformer

#endif // STATE_PLAY_HPP_INCLUDED
