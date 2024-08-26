#ifndef STATE_SPLASH_HPP_INCLUDED
#define STATE_SPLASH_HPP_INCLUDED
//
// state-splash.hpp
//
#include "state/states.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace platformer
{

    struct Context;

    //

    class SplashState : public IState
    {
      public:
        SplashState();
        virtual ~SplashState() override = default;

        [[nodiscard]] State which() const final { return State::Splash; }
        void update(Context & t_context, const float t_frameTimeSec) final;

        void draw(Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void handleEvent(Context & t_context, const sf::Event & t_event) final;
        void onEnter(Context & t_context) final;
        void onExit(Context &) final {}

      private:
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Text m_text;
        float m_elpasedTimeSec;
    };

} // namespace platformer

#endif // STATE_SPLASH_HPP_INCLUDED
