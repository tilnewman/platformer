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

    CoinBounceAnim::CoinBounceAnim(const sf::Texture & t_texture)
        : sprite{ t_texture }
        , elapsed_time_sec{ 0.0f }
        , frame_index{ 0 }
        , velocity{ 0.0f, 0.0f }
        , is_finished{ false }
    {}

    //

    LevelCompleteState::LevelCompleteState()
        : m_phase{ LevelCompletePhase::PreDelay }
        , m_elapsedTimeSec{ 0.0f }
        , m_knightTexture{}
        , m_knightSprite{ m_knightTexture }
        , m_text{ util::SfmlDefaults::instance().font() }
        , m_starDimTexture{}
        , m_starBrightTexture{}
        , m_starAnims{}
        , m_coinText{ util::SfmlDefaults::instance().font() }
        , m_coinTexture{}
        , m_coinAnims{}
    {
        m_starAnims.reserve(5);
    }

    void LevelCompleteState::onEnter(const Context & t_context)
    {
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
        std::string coinMessage{ std::to_string(t_context.level_info.coin()) };
        coinMessage += " Coins Gathered!";

        m_coinText = t_context.font.makeText(
            Font::Title,
            FontSize::Large,
            coinMessage,
            t_context.settings.off_white_color,
            sf::Text::Style::Bold);

        m_coinText.setPosition(
            { ((wholeRect.size.x * 0.5f) - (m_coinText.getGlobalBounds().size.x * 0.5f)),
              (starVertPosition + starSize.y + (wholeRect.size.y * 0.05f)) });

        util::TextureLoader::load(
            m_coinTexture, (t_context.settings.media_path / "image" / "anim" / "coin1.png"), true);
    }

    void LevelCompleteState::update(const Context & t_context, const float t_elapsedTimeSec)
    {
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
        m_elapsedTimeSec += t_elapsedTimeSec;
        if (m_elapsedTimeSec > 2.0f)
        {
            m_elapsedTimeSec = 0.0f;
            m_phase          = LevelCompletePhase::StarAnimation;
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
            m_elapsedTimeSec = 0.0f;
            m_phase          = LevelCompletePhase::CoinAnimation;
        }
    }

    void LevelCompleteState::updateCoinAnimation(const Context &, const float t_elapsedTimeSec)
    {
        m_elapsedTimeSec += t_elapsedTimeSec;
        if (m_elapsedTimeSec > 3.0f)
        {
            m_elapsedTimeSec = 0.0f;
            m_phase          = LevelCompletePhase::PostDelay;
        }
    }

    void
        LevelCompleteState::updatePostDelay(const Context & t_context, const float t_elapsedTimeSec)
    {
        m_elapsedTimeSec += t_elapsedTimeSec;
        if (m_elapsedTimeSec > 6.0f)
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
