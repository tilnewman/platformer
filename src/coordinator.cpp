// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// coordinator.hpp
//
#include "coordinator.hpp"

#include "sfml-util.hpp"
#include "texture-stats.hpp"

#include <iostream>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>

namespace platformer
{

    Coordinator::Coordinator(const Settings & settings)
        : m_window()
        , m_settings(settings)
        , m_random()
        , m_sfx(m_random)
        , m_states()
        , m_fonts()
        , m_avatar()
        , m_avatarTextures()
        , m_layout()
        , m_levelLoader()
        , m_level()
        , m_mapTextures()
        , m_backgroundImages()
        , m_pickups()
        , m_accents()
        , m_spells()
        , m_itemImages()
        , m_levelInfo(settings)
        , m_context(
              m_settings,
              m_window,
              m_random,
              m_sfx,
              m_states,
              m_fonts,
              m_avatar,
              m_avatarTextures,
              m_layout,
              m_levelLoader,
              m_level,
              m_mapTextures,
              m_backgroundImages,
              m_pickups,
              m_accents,
              m_spells,
              m_itemImages,
              m_levelInfo)
        , m_fpsValues()
        , m_oneSecondClock()
        , m_elapsedTimeSec(0.0f)
        , m_statsDisplayUPtr()
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

        m_layout.setup(m_window.getSize());
        m_fonts.setup(m_settings);
        m_itemImages.setup(m_settings);
        m_avatarTextures.setup(m_settings);
        m_mapTextures.setup(m_settings);
        m_pickups.setup(m_settings);
        m_accents.setup(m_settings);
        m_spells.setup(m_settings);
        m_avatar.setup(m_context, AvatarType::Assassin);

        m_states.changeTo(m_context, State::Splash);
    }

    void Coordinator::teardown()
    {
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
            m_states.changeTo(m_context, State::Shutdown);
        }
        else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
        {
            m_states.changeTo(m_context, State::Shutdown);
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

        m_window.display();
    }

    void Coordinator::update(const float frameTimeSec)
    {
        m_states.current().update(m_context, frameTimeSec);
    }

    void Coordinator::handleSleepUntilEndOfFrame(const float elapsedTimeSec)
    {
        m_fpsValues.push_back(static_cast<std::size_t>(1.0f / elapsedTimeSec));

        float timeRemainingSec = ((1.0f / m_settings.frame_rate) - elapsedTimeSec);

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
                std::cout << "FPS " << stats << '\n';
            }

            std::sort(std::begin(m_fpsValues), std::end(m_fpsValues));

            m_statsDisplayUPtr = std::make_unique<util::GraphDisplay<std::size_t>>(
                m_fpsValues, sf::Vector2u{ 500, 200 });

            m_fpsValues.clear();
        }
    }

    void Coordinator::setupRenderWindow(sf::VideoMode & videoMode)
    {
        std::cout << "Attempting video mode " << videoMode << "...";
        m_window.create(videoMode, "Platformer", sf::Style::Fullscreen);

        // sometimes the resolution of the window created does not match what was specified
        const unsigned actualWidth  = m_window.getSize().x;
        const unsigned actualHeight = m_window.getSize().y;
        if ((videoMode.width == actualWidth) && (videoMode.height == actualHeight))
        {
            std::cout << "Success." << std::endl;
        }
        else
        {
            std::cout << "Failed"
                      << ".  ";

            videoMode.width  = actualWidth;
            videoMode.height = actualHeight;

            std::cout << "SFML switched to " << videoMode << " instead." << std::endl;
        }
    }

} // namespace platformer
