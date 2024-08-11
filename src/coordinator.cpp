// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// coordinator.hpp
//
#include "coordinator.hpp"

#include "map-textures.hpp"
#include "sfml-util.hpp"
#include "tileset.hpp"

#include <iostream>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>

namespace platformer
{

    Coordinator::Coordinator(const Settings & settings)
        : m_window()
        , m_settings(settings)
        , m_random()
        , m_avatarTextures()
        , m_layout()
        , m_levelLoader()
        , m_level()
        , m_mapTextures()
        , m_backgroundImages()
        , m_pickups()
        , m_accents()
        , m_spells()
        , m_context(
              m_settings,
              m_window,
              m_random,
              m_avatarTextures,
              m_layout,
              m_levelLoader,
              m_level,
              m_mapTextures,
              m_backgroundImages,
              m_pickups,
              m_accents,
              m_spells)
    {}

    void Coordinator::setup()
    {
        setupRenderWindow(m_settings.video_mode);
        std::cout << "video mode: " << m_settings.video_mode << std::endl;

        m_layout.setup(m_window.getSize());
        m_avatarTextures.setup(m_settings);
        m_mapTextures.setup(m_settings);
        m_pickups.setup(m_settings);
        m_accents.setup(m_settings);
        m_spells.setup(m_settings);

        m_level.load(m_context);
    }

    void Coordinator::teardown() { m_window.close(); }

    void Coordinator::play()
    {
        setup();
        gameLoop();
        teardown();
    }

    void Coordinator::gameLoop()
    {
        sf::Clock frameClock;
        while (m_window.isOpen())
        {
            frameClock.restart();

            handleEvents();
            update(1.0f / m_settings.frame_rate);
            draw();

            handleSleepUntilEndOfFrame(frameClock.getElapsedTime().asSeconds());
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
            // TODO
        }
        else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
        {
            // TEMP TODO REMOVE
            m_window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            m_backgroundImages.move(-4.0f);
        }
    }

    void Coordinator::draw()
    {
        sf::RenderStates states;

        m_window.clear(sf::Color::Black);

        m_backgroundImages.draw(m_window, states);

        for (TileLayer & layer : m_level.tiles.layers)
        {
            states.texture = &m_context.map_textures.get(layer.image).texture;
            m_window.draw(&layer.visibleVerts[0], layer.visibleVerts.size(), sf::Quads, states);
            states.texture = nullptr;
        }

        m_pickups.draw(m_context, m_window, states);
        m_accents.draw(m_context, m_window, states);
        m_spells.draw(m_context, m_window, states);

        m_window.display();
    }

    void Coordinator::update(const float frameTimeSec)
    {
        m_pickups.update(m_context, frameTimeSec);
        m_accents.update(m_context, frameTimeSec);
        m_spells.update(m_context, frameTimeSec);
    }

    void Coordinator::handleSleepUntilEndOfFrame(const float elapsedTimeSec)
    {
        float timeRemainingSec = ((1.0f / m_settings.frame_rate) - elapsedTimeSec);

        sf::Clock delayClock;
        while (timeRemainingSec > 0.0f)
        {
            delayClock.restart();
            sf::sleep(sf::microseconds(100));
            timeRemainingSec -= delayClock.getElapsedTime().asSeconds();
        }
    }

    void Coordinator::setupRenderWindow(sf::VideoMode & videoMode)
    {
        std::cout << "Attempting video mode " << videoMode << "...";

        if (!videoMode.isValid())
        {
            std::cout << "but that is an invalid mode.  Valid video modes at "
                      << sf::VideoMode::getDesktopMode().bitsPerPixel << "bpp:" << std::endl
                      << util::makeSupportedVideoModesString(true) << std::endl;

            videoMode = util::findVideoModeClosestTo(videoMode);
            setupRenderWindow(videoMode);
            return;
        }

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

            std::cout << "Using " << videoMode << " instead." << std::endl;
        }
    }

} // namespace platformer
