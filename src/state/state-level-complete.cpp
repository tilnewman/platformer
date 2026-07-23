// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-level-complete.hpp
//
#include "state/state-level-complete.hpp"

#include "bramblefore/settings.hpp"
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
        , slider{ 0.5f, 0.5f }
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

    LevelCompleteState::LevelCompleteState()
        : m_knightTexture{}
        , m_knightSprite{ m_knightTexture }
        , m_youSurvivedText{ util::SfmlDefaults::instance().font() }
        , m_elapsedTimeSec{ 0.0f }
        , m_starTexture{}
        , m_starAnims{}
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
        m_knightSprite.move({ 0.0f, -(wholeRect.size.y * 0.1f) });

        // You Survived! text setup
        m_youSurvivedText = t_context.font.makeText(
            Font::Title, FontSize::Huge, "You Survived!", t_context.settings.off_white_color);

        m_youSurvivedText.setPosition(
            { ((wholeRect.size.x * 0.5f) - (m_youSurvivedText.getGlobalBounds().size.x * 0.5f)),
              (util::bottom(m_knightSprite) + (wholeRect.size.y * 0.015f)) });

        // star animations or you-got-no-stars text animation setup
        util::TextureLoader::load(
            m_starTexture,
            (t_context.settings.media_path / "image" / "ui" / "star-yellow.png"),
            true);

        const std::size_t starCount{ static_cast<std::size_t>(t_context.player.mapStarCount()) };
        if (starCount > 0)
        {
            sf::Sprite tempSprite(m_starTexture);
            const float tempScale{ t_context.layout.calScaleBasedOnResolution(t_context, 3.0f) };
            tempSprite.setScale({ tempScale, tempScale });
            const float starWidth{ tempSprite.getGlobalBounds().size.x };

            float starsTotalWidth{ static_cast<float>(starCount) * starWidth };

            const float horizSpacer{ wholeRect.size.x * 0.01f };
            if (starCount > 1)
            {
                starsTotalWidth += (static_cast<float>(starCount - 1) * horizSpacer);
            }

            const float starVertPosition{ util::bottom(m_youSurvivedText) +
                                          (wholeRect.size.y * 0.2f) };

            const sf::Vector2f startPos{ (wholeRect.size.x + starWidth), starVertPosition };

            float initialDelaySec{ 2.0f };
            float starHorizStopPos{ (wholeRect.size.x * 0.5f) - (starsTotalWidth * 0.5f) };
            for (std::size_t i{ 0 }; i < starCount; ++i)
            {
                m_starAnims.emplace_back(
                    m_starTexture, startPos, tempScale, starHorizStopPos, initialDelaySec);

                starHorizStopPos += (starWidth + horizSpacer);
                initialDelaySec += 0.5f;
            }
        }
    }

    void LevelCompleteState::update(const Context & t_context, const float t_frameTimeSec)
    {
        bool areAllStarsFinishedAnimating{ true };
        for (StarAnim & anim : m_starAnims)
        {
            anim.update(t_context, t_frameTimeSec);

            if (!anim.isFinished())
            {
                areAllStarsFinishedAnimating = false;
            }
        }

        if (areAllStarsFinishedAnimating)
        {
            m_elapsedTimeSec += t_frameTimeSec;
            if (m_elapsedTimeSec > 14.0f)
            {
                t_context.map_coord.mapNameAdvance();

                if (t_context.map_coord.mapName().empty())
                {
                    t_context.state.setChangePending(State::Credits);
                }
                else
                {
                    t_context.state.setChangePending(State::Play);
                }
            }
        }
    }

    void LevelCompleteState::draw(
        const Context &, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_target.draw(m_knightSprite, t_states);
        t_target.draw(m_youSurvivedText, t_states);

        for (const StarAnim & anim : m_starAnims)
        {
            t_target.draw(anim.sprite, t_states);
        }
    }

} // namespace bramblefore
