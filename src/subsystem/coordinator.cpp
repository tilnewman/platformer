// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// coordinator.hpp
//
#include "subsystem/coordinator.hpp"

#include "avatar/avatar-textures.hpp"
#include "map/map-textures.hpp"
#include "monster/monster-spell-anim.hpp"
#include "monster/monster-textures.hpp"
#include "subsystem/default-texture.hpp"
#include "subsystem/texture-stats.hpp"
#include "ui/gui-window.hpp"
#include "util/sfml-util.hpp"

#include <iostream>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>

namespace bramblefore
{

    Coordinator::Coordinator(const Settings & t_settings)
        : m_window{}
        , m_settings{ t_settings }
        , m_random{}
        , m_sfx{ m_random }
        , m_states{}
        , m_fonts{}
        , m_avatarUPtr{ std::make_unique<Avatar>() }
        , m_layout{}
        , m_levelLoader{}
        , m_level{}
        , m_backgroundImages{}
        , m_pickups{}
        , m_accents{}
        , m_spells{}
        , m_itemImages{}
        , m_levelInfo{ t_settings }
        , m_playerInfo{}
        , m_playerInfoDisplay{}
        , m_floatText{}
        , m_mapCoord{}
        , m_context{ m_settings,
                     m_random,
                     m_sfx,
                     m_states,
                     m_fonts,
                     *m_avatarUPtr,
                     m_layout,
                     m_levelLoader,
                     m_level,
                     m_backgroundImages,
                     m_pickups,
                     m_accents,
                     m_spells,
                     m_itemImages,
                     m_levelInfo,
                     m_playerInfo,
                     m_playerInfoDisplay,
                     m_floatText,
                     m_mapCoord }
        , m_fpsValues{}
        , m_oneSecondClock{}
        , m_elapsedTimeSec{ 0.0f }
        , m_statsDisplayUPtr{}
        , m_avatarTypeText{}
        , m_avatarAnimText{}
        , m_avatarAnimIndexText{}
    {
        m_fpsValues.reserve(128);
    }

    void Coordinator::setup()
    {
        setupRenderWindow(m_settings.video_mode);
        m_window.setMouseCursorVisible(false);

        m_sfx.mediaPath(m_settings.media_path / "sounds");
        m_sfx.loadAll();
        m_sfx.willLoop("walk", true);

        DefaultTexture::instance().setup();
        AvatarTextureManager::instance().setup(m_settings);
        MonsterTextureManager::instance().setup(m_settings);
        MapTextureManager::instance().setup();
        MonsterSpellTextureManager::instance().setup(m_settings);

        m_layout.setup(m_window.getSize());
        m_fonts.setup(m_settings);
        m_itemImages.setup(m_settings);
        m_pickups.setup(m_context);
        m_accents.setup(m_context);
        m_spells.setup(m_context);

        // TODO find a better place for this eventually
        m_playerInfo.setup(m_context, AvatarType::Enchantress);
        m_playerInfo.learnSpell(Spell::TeslaBall);
        m_playerInfo.learnSpell(Spell::Spikes1);

        // TODO remove after testing
        const sf::Color testColor{ 100, 100, 255 };
        m_avatarTypeText      = m_fonts.makeText(Font::Default, FontSize::Large, "", testColor);
        m_avatarAnimText      = m_fonts.makeText(Font::Default, FontSize::Large, "", testColor);
        m_avatarAnimIndexText = m_fonts.makeText(Font::Default, FontSize::Large, "", testColor);
        //
        m_avatarTypeText.setPosition(0.0f, 500.0f);
        m_avatarAnimText.setPosition(0.0f, 550.0f);
        m_avatarAnimIndexText.setPosition(0.0f, 600.0f);

        m_states.setChangePending(State::Splash);
    }

    void Coordinator::teardown()
    {
        m_avatarUPtr.reset();
        MonsterSpellTextureManager::instance().teardown();
        MapTextureManager::instance().teardown();
        AvatarTextureManager::instance().teardown();
        MonsterTextureManager::instance().teardown();
        DefaultTexture::instance().teardown();
        m_window.close();
        TextureStats::instance().dumpInfo();
    }

    void Coordinator::play()
    {
        setup();
        gameLoop();
        teardown();
    }

    void Coordinator::gameLoop()
    {
        sf::Clock frameClock;
        while (m_window.isOpen() && m_states.current().which() != State::Shutdown)
        {
            frameClock.restart();

            handleEvents();
            update(1.0f / m_settings.frame_rate);
            draw();

            handleSleepUntilEndOfFrame(frameClock.getElapsedTime().asSeconds());
            handleOncePerSecondTasks();
        }
    }

    void Coordinator::handleEvents()
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            handleEvent(event);
        }
    }

    void Coordinator::handleEvent(const sf::Event & event)
    {
        if (event.type == sf::Event::Closed)
        {
            m_states.setChangePending(State::Shutdown);
        }
        else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
        {
            m_states.setChangePending(State::Shutdown);
        }

        m_states.current().handleEvent(m_context, event);
    }

    void Coordinator::draw()
    {
        sf::RenderStates states;

        m_window.clear(sf::Color::Black);
        m_states.current().draw(m_context, m_window, states);

        if (m_statsDisplayUPtr && m_settings.will_display_fps_graph)
        {
            m_statsDisplayUPtr->draw(m_window, states);
        }

        // TODO remove after testing
        m_avatarTypeText.setString(std::string(toString(m_context.player.avatarType())));
        m_avatarAnimText.setString(std::string(toString(m_avatarUPtr->anim())));
        m_avatarAnimIndexText.setString(std::to_string(m_avatarUPtr->animIndex()));
        m_window.draw(m_avatarTypeText, states);
        m_window.draw(m_avatarAnimText, states);
        m_window.draw(m_avatarAnimIndexText, states);

        m_window.display();
    }

    void Coordinator::update(const float t_frameTimeSec)
    {
        m_states.current().update(m_context, t_frameTimeSec);
        m_states.changeIfPending(m_context);
    }

    void Coordinator::handleSleepUntilEndOfFrame(const float t_elapsedTimeSec)
    {
        m_fpsValues.push_back(static_cast<std::size_t>(1.0f / t_elapsedTimeSec));

        float timeRemainingSec{ (1.0f / m_settings.frame_rate) - t_elapsedTimeSec };

        sf::Clock delayClock;
        while (timeRemainingSec > 0.0f)
        {
            delayClock.restart();
            sf::sleep(sf::microseconds(100));
            timeRemainingSec -= delayClock.getElapsedTime().asSeconds();
        }
    }

    void Coordinator::handleOncePerSecondTasks()
    {
        m_elapsedTimeSec += m_oneSecondClock.restart().asSeconds();
        if (m_elapsedTimeSec > 1.0f)
        {
            m_elapsedTimeSec -= 1.0f;

            if (m_settings.will_display_fps)
            {
                const auto stats = util::makeStats(m_fpsValues);
                std::clog << "\tFPS " << stats << '\n';
            }

            std::ranges::sort(m_fpsValues);

            m_statsDisplayUPtr = std::make_unique<util::GraphDisplay<std::size_t>>(
                m_fpsValues, sf::Vector2u{ 500, 200 });

            m_fpsValues.clear();
        }
    }

    void Coordinator::setupRenderWindow(sf::VideoMode & t_videoMode)
    {
        std::clog << "Attempting video mode " << t_videoMode << "...";
        m_window.create(t_videoMode, "Platformer", sf::Style::Fullscreen);

        // sometimes the resolution of the window created does not match what was specified
        const unsigned actualWidth{ m_window.getSize().x };
        const unsigned actualHeight{ m_window.getSize().y };
        if ((t_videoMode.width == actualWidth) && (t_videoMode.height == actualHeight))
        {
            std::clog << "Success.\n";
        }
        else
        {
            std::clog << "Failed"
                      << ".  ";

            t_videoMode.width  = actualWidth;
            t_videoMode.height = actualHeight;

            std::clog << "SFML switched to " << t_videoMode << " instead.\n";
        }
    }

} // namespace bramblefore
