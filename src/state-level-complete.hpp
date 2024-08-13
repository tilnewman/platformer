#ifndef STATE_LEVEL_COMPLETE_HPP_INCLUDED
#define STATE_LEVEL_COMPLETE_HPP_INCLUDED
//
// state-level-complete.hpp
//
#include "states.hpp"

#include <SFML/Graphics/Text.hpp>

namespace platformer
{

    struct Context;

    //

    class LevelCompleteState : public IState
    {
      public:
        LevelCompleteState();
        virtual ~LevelCompleteState() override = default;

        State which() const final { return State::LevelComplete; }
        void update(Context & context, const float) final;
        void draw(Context & c, sf::RenderTarget & t, sf::RenderStates s) const final;
        void handleEvent(Context &, const sf::Event &) final {}
        void onEnter(Context &) final {}
        void onExit(Context &) final {}

      private:
        sf::Text m_text;
    };

} // namespace platformer

#endif // STATE_LEVEL_COMPLETE_HPP_INCLUDED
