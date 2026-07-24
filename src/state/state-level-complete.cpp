// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-level-complete.hpp
//
#include "state/state-level-complete.hpp"

#include "bramblefore/settings.hpp"
#include "map/level-info.hpp"
#include "player/player-info.hpp"
#include "state/state-manager.hpp"
#include "subsystem/context.hpp"
#include "subsystem/font.hpp"
#include "subsystem/map-coordinator.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    StarAnim::StarAnim(
        const sf::Texture & t_texture,
        const sf::Vector2f & t_startPos,
        const float t_imageScale,
        const float t_horizStopPosition,
        const float t_initialDelaySec)
        : sprite{ t_texture }
        , slider{ 0.65f, 0.5f }
        , elapsed_time_sec{ 0.0f }
        , initial_delay_sec{ t_initialDelaySec }
        , horiz_stop_pos{ t_horizStopPosition }
    {
        sprite.setScale({ t_imageScale, t_imageScale });
        sprite.setPosition(t_startPos);
    }

    void StarAnim::update(const Context & t_context, const float t_elapsedTimeSec)
    {
        elapsed_time_sec += t_elapsedTimeSec;
        if (elapsed_time_sec > initial_delay_sec)
        {
            const float ratio{ slider.update(t_elapsedTimeSec) };
            const sf::FloatRect wholeRect{ t_context.layout.wholeRect() };
            const float horizTravelDistance{ wholeRect.size.x - horiz_stop_pos };

            const float horizPos{ wholeRect.size.x -
                                  util::map(ratio, 0.5f, 1.0f, 0.0f, horizTravelDistance) };

            sprite.setPosition({ horizPos, sprite.getPosition().y });
        }
    }

    //

    CoinBounceAnim::CoinBounceAnim(
        const Context & t_context,
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const sf::Vector2f & t_position)
        : sprite{ t_texture, t_textureRect }
        , elapsed_scale_time_sec{ 0.0f }
        , elapsed_frame_time_sec{ 0.0f }
        , frame_index{ 0 }
        , velocity{ 1.5f, -10.0f }
        , is_finished{ false }
    {
        util::setOriginToCenter(sprite);
        sprite.setPosition(t_position);

        const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.5f) };
        sprite.setScale({ scale, scale });
    }

    //

    LevelCompleteState::LevelCompleteState()
        : m_tileBackground{}
        , m_phase{ LevelCompletePhase::PreDelay }
        , m_elapsedPhaseTimeSec{ 0.0f }
        , m_knightTexture{}
        , m_knightSprite{ m_knightTexture }
        , m_text{ util::SfmlDefaults::instance().font() }
        , m_starDimTexture{}
        , m_starBrightTexture{}
        , m_starAnims{}
        , m_coinText{ util::SfmlDefaults::instance().font() }
        , m_coinTexture{}
        , m_coinAnims{}
        , m_timeCoinEmitElapsedSec{ 0.0f }
        , m_coinEmittedCount{ 0 }
        , m_coinEmitPosition{}
        , m_coinsFinishedAnimCount{ 0 }
        , m_coinTextPosition{}
    {
        m_starAnims.reserve(5);
    }

    void LevelCompleteState::onEnter(const Context & t_context)
    {
        m_tileBackground.setup(t_context);

        // knight win image setup
        util::TextureLoader::load(
            m_knightTexture,
            (t_context.settings.media_path / "image" / "splash" / "knight-win.png"),
            true);

        m_knightSprite.setTexture(m_knightTexture, true);

        const sf::FloatRect wholeRect{ t_context.layout.wholeRect() };
        util::fitAndCenterInside(m_knightSprite, util::scaleRectInPlaceCopy(wholeRect, 0.3f));
        m_knightSprite.move({ 0.0f, -(wholeRect.size.y * 0.25f) });

        // You Survived! text setup
        std::string message{ t_context.map_coord.displayName() };
        message += " Level Complete";

        m_text = t_context.font.makeText(
            Font::Title, FontSize::Huge, message, t_context.settings.off_white_color);

        m_text.setPosition(
            { ((wholeRect.size.x * 0.5f) - (m_text.getGlobalBounds().size.x * 0.5f)),
              (util::bottom(m_knightSprite) + (wholeRect.size.y * 0.01f)) });

        // star animations or you-got-no-stars text animation setup
        util::TextureLoader::load(
            m_starBrightTexture,
            (t_context.settings.media_path / "image" / "ui" / "star-yellow.png"),
            true);

        util::TextureLoader::load(
            m_starDimTexture,
            (t_context.settings.media_path / "image" / "ui" / "star-brown.png"),
            true);

        const float starScale{ t_context.layout.calScaleBasedOnResolution(t_context, 3.0f) };

        const sf::Vector2f starSize = [&]() {
            sf::Sprite tempSprite(m_starBrightTexture);
            tempSprite.setScale({ starScale, starScale });
            return tempSprite.getGlobalBounds().size;
        }();

        const float horizSpacer{ wholeRect.size.x * 0.01f };
        const float starsTotalWidth{ (5.0f * starSize.x) + ((4.0f * horizSpacer)) };

        const float starVertPosition{ util::bottom(m_text) + (wholeRect.size.y * 0.05f) };
        const sf::Vector2f startPos{ (wholeRect.size.x + starSize.x), starVertPosition };

        const std::size_t starCount{ static_cast<std::size_t>(t_context.player.mapStarCount()) };

        float initialDelaySec{ 0.0f };
        float starHorizStopPos{ (wholeRect.size.x * 0.5f) - (starsTotalWidth * 0.5f) };
        for (std::size_t i{ 0 }; i < 5; ++i)
        {
            const sf::Texture & starTextureRef{ (i >= starCount) ? m_starDimTexture
                                                                 : m_starBrightTexture };

            m_starAnims.emplace_back(
                starTextureRef, startPos, starScale, starHorizStopPos, initialDelaySec);

            starHorizStopPos += (starSize.x + horizSpacer);
            initialDelaySec += 0.5f;
        }

        // coin text setup
        m_coinText = t_context.font.makeText(Font::Title, FontSize::Large, "");

        m_coinText.setPosition(
            { ((wholeRect.size.x * 0.5f) - (m_coinText.getGlobalBounds().size.x * 0.5f)),
              (starVertPosition + starSize.y + (wholeRect.size.y * 0.05f)) });

        m_coinTextPosition = m_coinText.getPosition();

        util::TextureLoader::load(
            m_coinTexture, (t_context.settings.media_path / "image" / "anim" / "coin1.png"), true);

        m_coinEmitPosition.x = (wholeRect.size.x * 0.5f);
        m_coinEmitPosition.y = wholeRect.size.y - 50.0f;
    }

    void LevelCompleteState::update(const Context & t_context, const float t_elapsedTimeSec)
    {
        m_tileBackground.update(t_context, t_elapsedTimeSec);

        if (LevelCompletePhase::PreDelay == m_phase)
        {
            updatePreDelay(t_context, t_elapsedTimeSec);
        }
        else if (LevelCompletePhase::StarAnimation == m_phase)
        {
            updateStarAnimation(t_context, t_elapsedTimeSec);
        }
        else if (LevelCompletePhase::CoinAnimation == m_phase)
        {
            updateCoinAnimation(t_context, t_elapsedTimeSec);
        }
        else
        {
            updatePostDelay(t_context, t_elapsedTimeSec);
        }
    }

    void LevelCompleteState::updatePreDelay(const Context &, const float t_elapsedTimeSec)
    {
        m_elapsedPhaseTimeSec += t_elapsedTimeSec;
        if (m_elapsedPhaseTimeSec > 2.0f)
        {
            m_elapsedPhaseTimeSec = 0.0f;
            m_phase               = LevelCompletePhase::StarAnimation;
        }
    }

    void LevelCompleteState::updateStarAnimation(
        const Context & t_context, const float t_elapsedTimeSec)
    {
        bool areAllStarsFinishedAnimating{ true };
        for (StarAnim & anim : m_starAnims)
        {
            if (!anim.isFinished())
            {
                areAllStarsFinishedAnimating = false;
                anim.update(t_context, t_elapsedTimeSec);
            }
        }

        if (areAllStarsFinishedAnimating)
        {
            m_elapsedPhaseTimeSec = 0.0f;
            m_phase               = LevelCompletePhase::CoinAnimation;
        }
    }

    void LevelCompleteState::updateCoinAnimation(
        const Context & t_context, const float t_elapsedTimeSec)
    {
        m_elapsedPhaseTimeSec += t_elapsedTimeSec;
        if ((m_elapsedPhaseTimeSec > 3.0f) && m_coinAnims.empty())
        {
            m_elapsedPhaseTimeSec = 0.0f;
            m_phase               = LevelCompletePhase::PostDelay;
            return;
        }

        const int coinsGathered{ t_context.level_info.coin() };
        const float timePerCoinEmit{ util::map(coinsGathered, 0, 200, 0.2f, 0.01f) };
        m_timeCoinEmitElapsedSec += t_elapsedTimeSec;
        if ((m_timeCoinEmitElapsedSec > timePerCoinEmit) && (m_coinEmittedCount < coinsGathered))
        {
            ++m_coinEmittedCount;
            m_timeCoinEmitElapsedSec -= timePerCoinEmit;

            m_coinAnims.emplace_back(
                t_context, m_coinTexture, coinTextureRect(0), m_coinEmitPosition);
        }

        for (CoinBounceAnim & anim : m_coinAnims)
        {
            const float timePerFrame{ 0.08f };
            anim.elapsed_frame_time_sec += t_elapsedTimeSec;
            if (anim.elapsed_frame_time_sec > timePerFrame)
            {
                anim.elapsed_frame_time_sec -= timePerFrame;

                if (++anim.frame_index >= coinFrameCount())
                {
                    anim.frame_index = 0;
                }

                anim.sprite.setTextureRect(coinTextureRect(anim.frame_index));
            }

            const float gravity{ 10.0f };
            anim.velocity += sf::Vector2f(0.0f, (gravity * t_elapsedTimeSec));
            anim.sprite.move(anim.velocity);

            anim.elapsed_scale_time_sec += t_elapsedTimeSec;
            const float duration{ 1.5f };

            const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.5f) +
                               util::map(anim.elapsed_scale_time_sec, 0.0f, duration, 0.0f, 3.0f) };

            anim.sprite.setScale({ scale, scale });

            if (anim.elapsed_scale_time_sec > duration)
            {
                anim.is_finished = true;

                t_context.sfx.play("pickup");

                std::string coinMessage{ std::to_string(++m_coinsFinishedAnimCount) };
                coinMessage += " Coins Gathered";

                m_coinText = t_context.font.makeText(
                    Font::Title,
                    FontSize::Large,
                    coinMessage,
                    sf::Color(236, 218, 95),
                    sf::Text::Style::Bold);

                const sf::FloatRect wholeRect{ t_context.layout.wholeRect() };

                m_coinText.setPosition(
                    { ((wholeRect.size.x * 0.5f) - (m_coinText.getGlobalBounds().size.x * 0.5f)),
                      m_coinTextPosition.y });
            }
        }

        std::erase_if(m_coinAnims, [](const CoinBounceAnim & a) { return a.is_finished; });
    }

    void
        LevelCompleteState::updatePostDelay(const Context & t_context, const float t_elapsedTimeSec)
    {
        m_elapsedPhaseTimeSec += t_elapsedTimeSec;
        if (m_elapsedPhaseTimeSec > 6.0f)
        {
            t_context.map_coord.advance();
            t_context.level_info.resetForNewLevel(t_context);

            if (t_context.map_coord.filename().empty())
            {
                t_context.state.setChangePending(State::Credits);
            }
            else
            {
                t_context.state.setChangePending(State::Play);
            }
        }
    }

    void LevelCompleteState::draw(
        const Context &, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_target.draw(m_tileBackground, t_states);
        t_target.draw(m_knightSprite, t_states);
        t_target.draw(m_text, t_states);

        for (const StarAnim & anim : m_starAnims)
        {
            t_target.draw(anim.sprite, t_states);
        }

        if ((LevelCompletePhase::CoinAnimation == m_phase) ||
            (LevelCompletePhase::PostDelay == m_phase))
        {
            t_target.draw(m_coinText, t_states);
        }

        for (const CoinBounceAnim & anim : m_coinAnims)
        {
            t_target.draw(anim.sprite, t_states);
        }
    }

    std::size_t LevelCompleteState::coinFrameCount() const
    {
        if (m_coinTexture.getSize().y > 0)
        {
            return static_cast<std::size_t>(m_coinTexture.getSize().x / m_coinTexture.getSize().y);
        }
        else
        {
            return 0;
        }
    }

    const sf::IntRect LevelCompleteState::coinTextureRect(const std::size_t t_frame) const
    {
        sf::IntRect rect;
        rect.size.x     = static_cast<int>(m_coinTexture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(t_frame) * rect.size.x);

        return rect;
    }

} // namespace bramblefore
