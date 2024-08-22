#ifndef COORDINATOR_HPP_INCLUDED
#define COORDINATOR_HPP_INCLUDED
//
// coordinator.hpp
//
#include "accents.hpp"
#include "avatar.hpp"
#include "background-images.hpp"
#include "context.hpp"
#include "font.hpp"
#include "item-images.hpp"
#include "level-file-loader.hpp"
#include "level-info.hpp"
#include "level.hpp"
#include "pickups.hpp"
#include "player-info-display.hpp"
#include "player-info.hpp"
#include "random.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sound-player.hpp"
#include "spells.hpp"
#include "state-manager.hpp"
#include "stats-display.hpp"

#include <memory>
#include <vector>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

namespace platformer
{

    class Coordinator
    {
      public:
        explicit Coordinator(const Settings & settings);

        void play();

      private:
        void setup();
        void teardown();
        void gameLoop();
        void handleEvents();
        void handleEvent(const sf::Event & event);
        void update(const float frameTimeSec);
        void draw();
        void handleSleepUntilEndOfFrame(const float actualFrameTimeSec);
        void handleOncePerSecondTasks();
        void setupRenderWindow(sf::VideoMode & videoMode);

      private:
        sf::RenderWindow m_window;
        Settings m_settings;
        util::Random m_random;
        util::SoundPlayer m_sfx;
        StateManager m_states;
        FontManager m_fonts;
        Avatar m_avatar;
        ScreenLayout m_layout;
        LevelFileLoader m_levelLoader;
        Level m_level;
        BackgroundImages m_backgroundImages;
        PickupAnimations m_pickups;
        AccentAnimations m_accents;
        SpellAnimations m_spells;
        ItemImages m_itemImages;
        LevelInfo m_levelInfo;
        PlayerInfo m_playerInfo;
        PlayerInfoDisplay m_playerInfoDisplay;

        Context m_context;

        // fps stuff
        std::vector<std::size_t> m_fpsValues;
        sf::Clock m_oneSecondClock;
        float m_elapsedTimeSec;
        std::unique_ptr<util::GraphDisplay<std::size_t>> m_statsDisplayUPtr;

        // avatar testing stuff
        sf::Text m_avatarTypeText;
        sf::Text m_avatarAnimText;
        sf::Text m_avatarAnimIndexText;
    };

} // namespace platformer

#endif // COORDINATOR_HPP_INCLUDED
