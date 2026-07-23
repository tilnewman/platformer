#ifndef STATE_LEVEL_COMPLETE_HPP_INCLUDED
#define STATE_LEVEL_COMPLETE_HPP_INCLUDED
//
// state-level-complete.hpp
//
#include "state/states.hpp"
#include "util/sliders.hpp"

#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace bramblefore
{

    struct Context;

    //

    struct StarAnim
    {
        StarAnim(
            const sf::Texture & t_texture,
            const sf::Vector2f & t_startPos,
            const float t_imageScale,
            const float t_horizStopPosition,
            const float t_initialDelaySec);

        void update(const Context & t_context, const float t_elapsedTimeSec);
        [[nodiscard]] constexpr bool isFinished() const noexcept { return !slider.isMoving(); }

        sf::Sprite sprite;
        util::SliderRatio<float> slider;
        float elapsed_time_sec;
        float initial_delay_sec;
        float horiz_stop_pos;
    };

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
        sf::Texture m_knightTexture;
        sf::Sprite m_knightSprite;
        sf::Text m_youSurvivedText;
        float m_elapsedTimeSec;
        sf::Texture m_starTexture;
        std::vector<StarAnim> m_starAnims;
    };

} // namespace bramblefore

#endif // STATE_LEVEL_COMPLETE_HPP_INCLUDED
