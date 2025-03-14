// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-credits.hpp
//
#include "state/state-credits.hpp"

#include "bramblefore/settings.hpp"
#include "monster/monster-manager.hpp"
#include "player/player-info-display.hpp"
#include "state/state-manager.hpp"
#include "subsystem/background-images.hpp"
#include "subsystem/context.hpp"
#include "subsystem/floating-text.hpp"
#include "subsystem/font.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace bramblefore
{

    CreditAnim::CreditAnim(
        const Context & t_context,
        const std::string & t_imageFilePath,
        const float t_imageScale,
        const std::string & t_name,
        const std::string & t_description,
        const float t_vertPos)
        : texture{}
        , sprite{ texture }
        , name{ t_context.font.makeText(
              Font::Title, FontSize::Large, t_name, t_context.settings.off_white_color) }
        , description{ t_context.font.makeText(
              Font::General, FontSize::Small, t_description, sf::Color(160, 160, 160)) }
    {
        util::TextureLoader::load(texture, t_imageFilePath, true);

        sprite.setTexture(texture, true);

        const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, t_imageScale) };
        sprite.scale({ scale, scale });

        const sf::FloatRect screenRect{ t_context.layout.wholeRect() };

        sprite.setPosition(
            { (util::center(screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f)), t_vertPos });

        //

        // this is the vertical empty space between images and lines of text
        const float vertPad{ screenRect.size.y * 0.025f };

        name.setPosition({ (util::center(screenRect).x - (name.getGlobalBounds().size.x * 0.5f)),
                           (util::bottom(sprite) + vertPad) });

        description.setPosition(
            { (util::center(screenRect).x - (description.getGlobalBounds().size.x * 0.5f)),
              (util::bottom(name) + (vertPad * 0.35f)) });
    }

    void CreditAnim::move(const float t_amount)
    {
        sprite.move({ 0.0f, t_amount });
        name.move({ 0.0f, t_amount });
        description.move({ 0.0f, t_amount });
    }

    float CreditAnim::bottom() const { return util::bottom(description); }

    void CreditAnim::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_target.draw(sprite, t_states);
        t_target.draw(name, t_states);
        t_target.draw(description, t_states);
    }

    //

    CreditsState::CreditsState()
        : m_titleText{ util::SfmlDefaults::instance().font() }
        , m_credits{}
    {}

    void CreditsState::update(Context & t_context, const float t_frameTimeSec)
    {
        const float speed{ -35.0f };
        const float moveAmount{ speed * t_frameTimeSec };

        m_titleText.move({ 0.0f, moveAmount });

        for (CreditAnim & anim : m_credits)
        {
            anim.move(moveAmount);
        }

        if (m_credits.back().bottom() < 50.0f)
        {
            t_context.state.setChangePending(State::Shutdown);
        }
    }

    void CreditsState::draw(Context &, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_target.draw(m_titleText, t_states);

        for (const CreditAnim & anim : m_credits)
        {
            anim.draw(t_target, t_states);
        }
    }

    void CreditsState::handleEvent(Context & t_context, const sf::Event & t_event)
    {
        if (t_event.is<sf::Event::KeyPressed>())
        {
            t_context.state.setChangePending(State::Shutdown);
        }
    }

    void CreditsState::onEnter(Context & t_context)
    {
        const sf::FloatRect screenRect{ t_context.layout.wholeRect() };
        const float vertPad{ screenRect.size.y * 0.15f };

        m_titleText = t_context.font.makeText(
            Font::Title, FontSize::Huge, "Credits", t_context.settings.off_white_color);

        m_titleText.setPosition(
            { (util::center(screenRect).x - (m_titleText.getGlobalBounds().size.x * 0.5f)),
              screenRect.size.y });

        m_credits.reserve(16);

        m_credits.emplace_back(
            t_context,
            (t_context.settings.media_path / "image/credits/cpp.png").string(),
            0.25f,
            "Ziesche Til Newman",
            "Software, C++, SFML, CMake",
            (util::bottom(m_titleText) + vertPad));

        m_credits.emplace_back(
            t_context,
            (t_context.settings.media_path / "image/credits/sfml.png").string(),
            0.6f,
            "Simple Fast Multimedia Library",
            "Thanks to Laurent Gomila for this amazing library!",
            m_credits.back().bottom() + vertPad);

        m_credits.emplace_back(
            t_context,
            (t_context.settings.media_path / "image/credits/freesound.png").string(),
            0.75f,
            "www.freesound.org Website",
            "My secret weapon in the hunt for sound effects.",
            m_credits.back().bottom() + vertPad);

        m_credits.emplace_back(
            t_context,
            (t_context.settings.media_path / "image/credits/craftpix.png").string(),
            0.9f,
            "www.craftpix.net Website",
            "www.craftpix.net/product/fantasy-platformer-game-kit-pixel-art/",
            m_credits.back().bottom() + vertPad);

        m_credits.emplace_back(
            t_context,
            (t_context.settings.media_path / "image/credits/font.png").string(),
            0.35f,
            "Mops Antiqua Font",
            "Created by Uwe Borchert, with FontForge 2.0, SIL Open Font License v1.1",
            m_credits.back().bottom() + vertPad);
    }

    void CreditsState::onExit(Context &) {}

} // namespace bramblefore
