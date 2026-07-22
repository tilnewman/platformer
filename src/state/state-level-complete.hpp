#ifndef STATE_LEVEL_COMPLETE_HPP_INCLUDED
#define STATE_LEVEL_COMPLETE_HPP_INCLUDED
//
// state-level-complete.hpp
//
#include "state/states.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace bramblefore
{

    struct Context;

    //

    class LevelCompleteState : public IState
    {
      public:
        LevelCompleteState();
        virtual ~LevelCompleteState() override = default;

        [[nodiscard]] State which() const final { return State::LevelComplete; }
        void update(const Context & t_context, const float t_frameTimeSec) final;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void handleEvent(const Context &, const sf::Event &) final {}
        void onEnter(const Context &) final;
        void onExit(const Context &) final {}

      private:
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Text m_text;
        float m_elapsedTimeSec;
    };

} // namespace bramblefore

#endif // STATE_LEVEL_COMPLETE_HPP_INCLUDED
