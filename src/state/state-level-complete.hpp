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

    enum class LevelCompletePhase
    {
        PreDelay,
        StarAnimation,
        CoinAnimation,
        PostDelay
    };

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

    struct CoinBounceAnim
    {
        CoinBounceAnim(
            const Context & t_context,
            const sf::Texture & t_texture,
            const sf::IntRect & t_textureRect,
            const sf::Vector2f & t_position);

        sf::Sprite sprite;
        float elapsed_scale_time_sec;
        float elapsed_frame_time_sec;
        std::size_t frame_index;
        sf::Vector2f velocity;
        bool is_finished;
    };

    //

    class LevelCompleteState : public IState
    {
      public:
        LevelCompleteState();
        virtual ~LevelCompleteState() override = default;

        [[nodiscard]] State which() const final { return State::LevelComplete; }
        void update(const Context & t_context, const float t_elapsedTimeSec) final;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void handleEvent(const Context &, const sf::Event &) final {}
        void onEnter(const Context &) final;
        void onExit(const Context &) final {}

      private:
        void updatePreDelay(const Context & t_context, const float t_elapsedTimeSec);
        void updateStarAnimation(const Context & t_context, const float t_elapsedTimeSec);
        void updateCoinAnimation(const Context & t_context, const float t_elapsedTimeSec);
        void updatePostDelay(const Context & t_context, const float t_elapsedTimeSec);
        [[nodiscard]] std::size_t coinFrameCount() const;
        [[nodiscard]] const sf::IntRect coinTextureRect(const std::size_t frame) const;

      private:
        LevelCompletePhase m_phase;
        float m_elapsedPhaseTimeSec;

        sf::Texture m_knightTexture;
        sf::Sprite m_knightSprite;

        sf::Text m_text;

        sf::Texture m_starDimTexture;
        sf::Texture m_starBrightTexture;
        std::vector<StarAnim> m_starAnims;

        sf::Text m_coinText;
        sf::Texture m_coinTexture;
        std::vector<CoinBounceAnim> m_coinAnims;
        float m_timeCoinEmitElapsedSec;
        int m_coinEmittedCount;
        sf::Vector2f m_coinEmitPosition;
        std::size_t m_coinsFinishedAnimCount;
        sf::Vector2f m_coinTextPosition;
    };

} // namespace bramblefore

#endif // STATE_LEVEL_COMPLETE_HPP_INCLUDED
