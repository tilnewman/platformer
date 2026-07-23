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

    Coordinator::Coordinator(const Settings & t_setting)
        : m_setting{ t_setting }
        , m_windowUPtr{}
        , m_renderStates{}
        , m_randomUPtr{}
        , m_sfxUPtr{}
        , m_stateUPtr{}
        , m_fontUPtr{}
        , m_avatarUPtr{}
        , m_layoutUPtr{}
        , m_levelLoaderUPtr{}
        , m_levelUPtr{}
        , m_backgroundImageUPtr{}
        , m_pickupUPtr{}
        , m_accentUPtr{}
        , m_spellUPtr{}
        , m_itemImageUPtr{}
        , m_levelInfoUPtr{}
        , m_playerInfoUPtr{}
        , m_playerInfoDisplayUPtr{}
        , m_floatingTextUPtr{}
        , m_mapCoordUPtr{}
        , m_framerateDisplayUPtr{}
        , m_bloodSplatManagerUPtr{}
        , m_contextUPtr{}
    {}

    void Coordinator::setup()
    {
        m_windowUPtr = std::make_unique<sf::RenderWindow>();
        setupRenderWindow(m_setting.video_mode);
        m_windowUPtr->setMouseCursorVisible(false);
        m_windowUPtr->setVerticalSyncEnabled(false);
        m_windowUPtr->setKeyRepeatEnabled(false);

        // don't let SFML sleep between frames because we do this ourselves below
        // see settings.hpp for where the framerate is actually set
        m_windowUPtr->setFramerateLimit(0);

        util::SfmlDefaults::instance().setup();

        m_randomUPtr            = std::make_unique<util::Random>();
        m_sfxUPtr               = std::make_unique<util::SoundPlayer>(*m_randomUPtr);
        m_layoutUPtr            = std::make_unique<ScreenLayout>();
        m_fontUPtr              = std::make_unique<FontManager>();
        m_stateUPtr             = std::make_unique<StateManager>();
        m_avatarUPtr            = std::make_unique<Avatar>();
        m_playerInfoDisplayUPtr = std::make_unique<PlayerInfoDisplay>();
        m_bloodSplatManagerUPtr = std::make_unique<BloodSplatManager>();
        m_levelLoaderUPtr       = std::make_unique<LevelFileLoader>();
        m_levelUPtr             = std::make_unique<Level>();
        m_backgroundImageUPtr   = std::make_unique<BackgroundImages>();
        m_pickupUPtr            = std::make_unique<PickupAnimations>();
        m_accentUPtr            = std::make_unique<AccentAnimations>();
        m_spellUPtr             = std::make_unique<SpellAnimations>();
        m_itemImageUPtr         = std::make_unique<ItemImages>();
        m_levelInfoUPtr         = std::make_unique<LevelInfo>(m_setting);
        m_playerInfoUPtr        = std::make_unique<PlayerInfo>();
        m_floatingTextUPtr      = std::make_unique<FloatingText>();
        m_mapCoordUPtr          = std::make_unique<MapCoordinator>();
        m_framerateDisplayUPtr  = std::make_unique<FramerateDisplay>();

        m_contextUPtr = std::make_unique<Context>(
            m_setting,
            *m_randomUPtr,
            *m_sfxUPtr,
            *m_stateUPtr,
            *m_fontUPtr,
            *m_avatarUPtr,
            *m_layoutUPtr,
            *m_levelLoaderUPtr,
            *m_levelUPtr,
            *m_backgroundImageUPtr,
            *m_pickupUPtr,
            *m_accentUPtr,
            *m_spellUPtr,
            *m_itemImageUPtr,
            *m_levelInfoUPtr,
            *m_playerInfoUPtr,
            *m_playerInfoDisplayUPtr,
            *m_floatingTextUPtr,
            *m_mapCoordUPtr,
            *m_bloodSplatManagerUPtr);

        m_sfxUPtr->mediaPath(m_setting.media_path / "sound");
        m_sfxUPtr->loadAll();
        m_sfxUPtr->willLoop("walk", true);

        AvatarTextureManager::instance().setup(m_setting);
        MonsterTextureManager::instance().setup(m_setting);
        MapTextureManager::instance().setup();
        MonsterSpellTextureManager::instance().setup(m_setting);

        m_layoutUPtr->setup(m_windowUPtr->getSize());
        m_fontUPtr->setup(m_setting);
        m_itemImageUPtr->setup(m_setting);
        m_pickupUPtr->setup(*m_contextUPtr);
        m_accentUPtr->setup(*m_contextUPtr);
        m_spellUPtr->setup(*m_contextUPtr);
        m_framerateDisplayUPtr->setup(*m_contextUPtr);
        m_bloodSplatManagerUPtr->setup(*m_contextUPtr);

        m_stateUPtr->setChangePending(State::Splash);
    }

    void Coordinator::teardown()
    {
        m_framerateDisplayUPtr.reset();
        m_mapCoordUPtr.reset();
        m_playerInfoUPtr.reset();
        m_levelInfoUPtr.reset();
        m_itemImageUPtr.reset();
        m_floatingTextUPtr.reset();
        m_spellUPtr.reset();
        m_accentUPtr.reset();
        m_pickupUPtr.reset();
        m_backgroundImageUPtr.reset();
        m_levelUPtr.reset();
        m_levelLoaderUPtr.reset();
        m_playerInfoDisplayUPtr.reset();
        m_bloodSplatManagerUPtr.reset();
        m_avatarUPtr.reset();
        m_layoutUPtr.reset();
        m_stateUPtr.reset();
        m_fontUPtr.reset();
        m_sfxUPtr.reset();

        m_randomUPtr.reset();

        MonsterSpellTextureManager::instance().teardown();
        MapTextureManager::instance().teardown();
        AvatarTextureManager::instance().teardown();
        MonsterTextureManager::instance().teardown();

        util::SfmlDefaults::instance().teardown();

        m_contextUPtr.reset();

        m_windowUPtr->close();
        m_windowUPtr.reset();

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
        while (m_windowUPtr->isOpen() && (m_stateUPtr->current().which() != State::Shutdown))
        {
            frameClock.restart();

            handleEvents();
            update(1.0f / m_setting.frame_rate);
            draw();

            handleEndOfFrameTasks(frameClock.getElapsedTime().asSeconds());
        }
    }

    void Coordinator::handleEvents()
    {
        while (const std::optional event = m_windowUPtr->pollEvent())
        {
            handleEvent(event.value());
        }
    }

    void Coordinator::handleEvent(const sf::Event & t_event)
    {
        if (t_event.is<sf::Event::Closed>())
        {
            m_stateUPtr->setChangePending(State::Shutdown);
        }
        else if (const auto * const keyPtr = t_event.getIf<sf::Event::KeyPressed>())
        {
            // TODO remove after testing
            if (keyPtr->scancode == sf::Keyboard::Scancode::Escape)
            {
                m_stateUPtr->setChangePending(State::Shutdown);
            }
        }

        m_stateUPtr->current().handleEvent(*m_contextUPtr, t_event);
    }

    void Coordinator::draw()
    {
        // some states depend on the background color being black
        m_windowUPtr->clear(sf::Color::Black);
        m_stateUPtr->current().draw(*m_contextUPtr, *m_windowUPtr, m_renderStates);
        m_framerateDisplayUPtr->draw(*m_contextUPtr, *m_windowUPtr, m_renderStates);
        m_windowUPtr->display();
    }

    void Coordinator::update(const float t_frameTimeSec)
    {
        m_stateUPtr->current().update(*m_contextUPtr, t_frameTimeSec);
        m_stateUPtr->changeIfPending(*m_contextUPtr);
    }

    void Coordinator::handleEndOfFrameTasks(const float t_elapsedTimeSec)
    {
        m_framerateDisplayUPtr->handleElapsedFrame(*m_contextUPtr, t_elapsedTimeSec);

        // sleep until end of frame occurs
        float timeRemainingSec{ (1.0f / m_setting.frame_rate) - t_elapsedTimeSec };

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

        if (!m_setting.video_mode.isValid())
        {
            std::cout << "but that is not suported.  Valid video modes at "
                      << m_setting.video_mode.bitsPerPixel << "bpp:\n"
                      << util::makeSupportedVideoModesString(m_setting.video_mode.bitsPerPixel)
                      << '\n';

            t_videoMode = util::findVideoModeClosestTo(m_setting.video_mode);
            setupRenderWindow(t_videoMode);
            return;
        }

        m_windowUPtr->create(t_videoMode, "Bramblefore", sf::State::Fullscreen);

        // sometimes the resolution of the window created does not match what was specified
        const unsigned actualWidth  = m_windowUPtr->getSize().x;
        const unsigned actualHeight = m_windowUPtr->getSize().y;
        if ((m_setting.video_mode.size.x == actualWidth) &&
            (m_setting.video_mode.size.y == actualHeight))
        {
            std::cout << "Success." << std::endl;
        }
        else
        {
            std::cout << "Failed" << ".  ";

            m_setting.video_mode.size.x = actualWidth;
            m_setting.video_mode.size.y = actualHeight;

            std::cout << "Using " << m_setting.video_mode << " instead." << std::endl;
        }
    }

} // namespace bramblefore
