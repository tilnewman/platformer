#ifndef STATE_SPLASH_HPP_INCLUDED
#define STATE_SPLASH_HPP_INCLUDED
//
// state-splash.hpp
//
#include "states.hpp"

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

        State which() const final { return State::Splash; }
        void update(Context & context, const float) final;
        void draw(Context & c, sf::RenderTarget & t, sf::RenderStates s) const final;
        void handleEvent(Context & context, const sf::Event & event) final;
        void onEnter(Context & context) final;
        void onExit(Context &) final {}

      private:
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Text m_text;
        float m_elpasedTimeSec;
    };

} // namespace platformer

#endif // STATE_SPLASH_HPP_INCLUDED
