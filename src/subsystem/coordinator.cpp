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
#include "ui/gui-window.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

#include <iostream>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>

namespace bramblefore
{

    Coordinator::Coordinator(const Settings & t_settings)
        : m_renderStates{}
        , m_window{}
        , m_settings{ t_settings }
        , m_random{}
        , m_sfx{ m_random }
        , m_states{}
        , m_fonts{}
        , m_avatarUPtr{}
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
        , m_playerInfoDisplayUPtr{}
        , m_floatText{}
        , m_mapCoord{}
        , m_framerateDisplay{}
        , m_bloodSplatManagerUPtr{}
        , m_contextUPtr{}
    {}

    void Coordinator::setup()
    {
        setupRenderWindow(m_settings.video_mode);
        m_window.setMouseCursorVisible(false);
        m_window.setVerticalSyncEnabled(false);
        m_window.setKeyRepeatEnabled(false);

        // don't let SFML sleep between frames because we do this ourselves below
        // see settings.hpp for where the framerate is actually set
        m_window.setFramerateLimit(0);

        util::SfmlDefaults::instance().setup();

        m_sfx.mediaPath(m_settings.media_path / "sound");
        m_sfx.loadAll();
        m_sfx.willLoop("walk", true);

        m_avatarUPtr            = std::make_unique<Avatar>();
        m_playerInfoDisplayUPtr = std::make_unique<PlayerInfoDisplay>();
        m_bloodSplatManagerUPtr = std::make_unique<BloodSplatManager>();

        m_contextUPtr = std::make_unique<Context>(
            m_settings,
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
            *m_playerInfoDisplayUPtr,
            m_floatText,
            m_mapCoord,
            *m_bloodSplatManagerUPtr);

        AvatarTextureManager::instance().setup(m_settings);
        MonsterTextureManager::instance().setup(m_settings);
        MapTextureManager::instance().setup();
        MonsterSpellTextureManager::instance().setup(m_settings);

        m_layout.setup(m_window.getSize());
        m_fonts.setup(m_settings);
        m_itemImages.setup(m_settings);
        m_pickups.setup(*m_contextUPtr);
        m_accents.setup(*m_contextUPtr);
        m_spells.setup(*m_contextUPtr);
        m_framerateDisplay.setup(*m_contextUPtr);
        m_bloodSplatManagerUPtr->setup(*m_contextUPtr);

        m_states.setChangePending(State::Splash);
    }

    void Coordinator::teardown()
    {
        m_playerInfoDisplayUPtr.reset();
        m_bloodSplatManagerUPtr.reset();
        m_avatarUPtr.reset();

        MonsterSpellTextureManager::instance().teardown();
        MapTextureManager::instance().teardown();
        AvatarTextureManager::instance().teardown();
        MonsterTextureManager::instance().teardown();
        
        util::SfmlDefaults::instance().teardown();

        m_contextUPtr.reset();

        m_window.close();

        // util::TextureLoader::dumpInfo();
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
        while (m_window.isOpen() && (m_states.current().which() != State::Shutdown))
        {
            frameClock.restart();

            handleEvents();
            update(1.0f / m_settings.frame_rate);
            draw();

            handleEndOfFrameTasks(frameClock.getElapsedTime().asSeconds());
        }
    }

    void Coordinator::handleEvents()
    {
        while (const std::optional event = m_window.pollEvent())
        {
            handleEvent(event.value());
        }
    }

    void Coordinator::handleEvent(const sf::Event & t_event)
    {
        if (t_event.is<sf::Event::Closed>())
        {
            m_states.setChangePending(State::Shutdown);
        }
        else if (const auto * const keyPtr = t_event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPtr->scancode == sf::Keyboard::Scancode::Escape)
            {
                m_states.setChangePending(State::Shutdown);
            }
        }

        m_states.current().handleEvent(*m_contextUPtr, t_event);
    }

    void Coordinator::draw()
    {
        m_window.clear(sf::Color::Black); // some states depend on the background color being black
        m_states.current().draw(*m_contextUPtr, m_window, m_renderStates);
        m_framerateDisplay.draw(*m_contextUPtr, m_window, m_renderStates);
        m_window.display();
    }

    void Coordinator::update(const float t_frameTimeSec)
    {
        m_states.current().update(*m_contextUPtr, t_frameTimeSec);
        m_states.changeIfPending(*m_contextUPtr);
    }

    void Coordinator::handleEndOfFrameTasks(const float t_elapsedTimeSec)
    {
        m_framerateDisplay.handleElapsedFrame(*m_contextUPtr, t_elapsedTimeSec);

        // sleep until end of frame occurs
        float timeRemainingSec{ (1.0f / m_settings.frame_rate) - t_elapsedTimeSec };

        sf::Clock delayClock;
        while (timeRemainingSec > 0.0f)
        {
            delayClock.restart();
            sf::sleep(sf::microseconds(100));
            timeRemainingSec -= delayClock.getElapsedTime().asSeconds();
        }
    }

    void Coordinator::setupRenderWindow(sf::VideoMode & t_videoMode)
    {
        std::cout << "Attempting video mode " << t_videoMode << "...";

        if (!m_settings.video_mode.isValid())
        {
            std::cout << "but that is not suported.  Valid video modes at "
                      << m_settings.video_mode.bitsPerPixel << "bpp:\n"
                      << util::makeSupportedVideoModesString(m_settings.video_mode.bitsPerPixel)
                      << '\n';

            t_videoMode = util::findVideoModeClosestTo(m_settings.video_mode);
            setupRenderWindow(t_videoMode);
            return;
        }

        m_window.create(t_videoMode, "Bramblefore", sf::State::Fullscreen);

        // sometimes the resolution of the window created does not match what was specified
        const unsigned actualWidth  = m_window.getSize().x;
        const unsigned actualHeight = m_window.getSize().y;
        if ((m_settings.video_mode.size.x == actualWidth) &&
            (m_settings.video_mode.size.y == actualHeight))
        {
            std::cout << "Success." << std::endl;
        }
        else
        {
            std::cout << "Failed" << ".  ";

            m_settings.video_mode.size.x = actualWidth;
            m_settings.video_mode.size.y = actualHeight;

            std::cout << "Using " << m_settings.video_mode << " instead." << std::endl;
        }
    }

} // namespace bramblefore
