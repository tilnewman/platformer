#ifndef STATE_LEVEL_COMPLETE_HPP_INCLUDED
#define STATE_LEVEL_COMPLETE_HPP_INCLUDED
//
// state-level-complete.hpp
//
#include "state/states.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

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
        void update(Context & t_context, const float t_frameTimeSec) final;

        void
            draw(Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const final;
        
        void handleEvent(Context &, const sf::Event &) final {}
        void onEnter(Context &) final;
        void onExit(Context &) final {}

      private:
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Text m_text;
        float m_elapsedTimeSec;
    };

} // namespace platformer

#endif // STATE_LEVEL_COMPLETE_HPP_INCLUDED
