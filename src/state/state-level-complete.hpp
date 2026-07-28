#ifndef STATE_LEVEL_COMPLETE_HPP_INCLUDED
#define STATE_LEVEL_COMPLETE_HPP_INCLUDED
//
// state-level-complete.hpp
//
#include "monster/imonster.hpp"
#include "state/states.hpp"
#include "ui/tile-background.hpp"
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
        MonsterAnimation,
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

    struct CoinAnim
    {
        CoinAnim(
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

    class CoinAnimationManager
    {
      public:
        CoinAnimationManager();

        void setup(
            const Context & t_context,
            const sf::Vector2f & t_starPosition,
            const sf::Vector2f & t_starSize);

        void update(const Context & t_context, const float t_elapsedTimeSec);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        [[nodiscard]] constexpr bool areAllFinished() noexcept { return m_coinAnims.empty(); }

      private:
        [[nodiscard]] std::size_t coinFrameCount() const;
        [[nodiscard]] const sf::IntRect coinTextureRect(const std::size_t frame) const;

      private:
        sf::Text m_coinText;
        sf::Texture m_coinTexture;
        std::vector<CoinAnim> m_coinAnims;
        float m_timeCoinEmitElapsedSec;
        int m_coinEmittedCount;
        sf::Vector2f m_coinEmitPosition;
        std::size_t m_coinsFinishedAnimCount;
        sf::Vector2f m_coinTextPosition;
    };

    //

    struct MonsterAnimation
    {
        MonsterAnimation(
            const sf::Texture & t_monsterTexture,
            const sf::Texture & t_graveTexture,
            const float t_scale,
            const sf::Vector2f & t_position,
            const float t_initialDelaySec,
            const float t_horizStopPos,
            const bool t_didSurvive);

        sf::Sprite monster_sprite;
        sf::Sprite grave_sprite;
        util::SliderRatio<float> slider;
        float elapsed_time_sec;
        float initial_delay_sec;
        float horiz_stop_pos;
        bool is_moving;
        bool did_survive;
    };

    //

    class MonsterAnimationManager
    {
      public:
        MonsterAnimationManager();

        void setup(const Context & t_context);
        bool update(const Context & t_context, const float t_elapsedTimeSec);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        void drawText(sf::RenderTarget & t_target, sf::RenderStates t_states) const;

      private:
        std::vector<MonsterAnimation> m_anims;
        std::vector<sf::Texture> m_textures;
        sf::Texture m_graveTexture;
        sf::Text m_text;
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
        void updateMonsterAnimation(const Context & t_context, const float t_elapsedTimeSec);
        void updatePostDelay(const Context & t_context, const float t_elapsedTimeSec);

      private:
        TileBackground m_tileBackground;

        LevelCompletePhase m_phase;
        float m_elapsedPhaseTimeSec;

        sf::Texture m_knightTexture;
        sf::Sprite m_knightSprite;

        sf::Text m_text;

        sf::Texture m_starDimTexture;
        sf::Texture m_starBrightTexture;
        std::vector<StarAnim> m_starAnims;

        CoinAnimationManager m_coinAnimations;
        MonsterAnimationManager m_monsterAnimations;
    };

} // namespace bramblefore

#endif // STATE_LEVEL_COMPLETE_HPP_INCLUDED
