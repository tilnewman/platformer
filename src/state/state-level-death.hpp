#ifndef STATE_LEVEL_DEATH_HPP_INCLUDED
#define STATE_LEVEL_DEATH_HPP_INCLUDED
//
// state-level-death.hpp
//
#include "state/states.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace bramblefore
{

    struct Context;

    //

    class LevelDeathState : public IState
    {
      public:
        LevelDeathState();
        virtual ~LevelDeathState() override = default;

        [[nodiscard]] State which() const final { return State::LevelDeath; }
        void update(Context & t_context, const float t_frameTimeSec) final;

        void draw(Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void handleEvent(Context &, const sf::Event &) final {}
        void onEnter(Context &) final;
        void onExit(Context &) final {}

      private:
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Text m_text;
        float m_elapsedTimeSec;
    };

} // namespace bramblefore

#endif // STATE_LEVEL_DEATH_HPP_INCLUDED
