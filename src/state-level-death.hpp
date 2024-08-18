#ifndef STATE_LEVEL_DEATH_HPP_INCLUDED
#define STATE_LEVEL_DEATH_HPP_INCLUDED
//
// state-level-death.hpp
//
#include "states.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace platformer
{

    struct Context;

    //

    class LevelDeathState : public IState
    {
      public:
        LevelDeathState();
        virtual ~LevelDeathState() override = default;

        State which() const final { return State::LevelComplete; }
        void update(Context & context, const float) final;
        void draw(Context & c, sf::RenderTarget & t, sf::RenderStates s) const final;
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

#endif // STATE_LEVEL_DEATH_HPP_INCLUDED
