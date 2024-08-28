#ifndef COORDINATOR_HPP_INCLUDED
#define COORDINATOR_HPP_INCLUDED
//
// coordinator.hpp
//
#include "avatar/avatar.hpp"
#include "avatar/spells-anim.hpp"
#include "bramblefore/settings.hpp"
#include "item-images.hpp"
#include "map/accents.hpp"
#include "map/level-file-loader.hpp"
#include "map/level-info.hpp"
#include "map/level.hpp"
#include "map/pickups.hpp"
#include "player/player-info-display.hpp"
#include "player/player-info.hpp"
#include "state/state-manager.hpp"
#include "subsystem/background-images.hpp"
#include "subsystem/context.hpp"
#include "subsystem/floating-text.hpp"
#include "subsystem/font.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/random.hpp"
#include "util/sound-player.hpp"
#include "util/stats-display.hpp"

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
        explicit Coordinator(const Settings & t_settings);

        void play();

      private:
        void setup();
        void teardown();
        void gameLoop();
        void handleEvents();
        void handleEvent(const sf::Event & t_event);
        void update(const float t_frameTimeSec);
        void draw();
        void handleSleepUntilEndOfFrame(const float t_actualFrameTimeSec);
        void handleOncePerSecondTasks();
        void setupRenderWindow(sf::VideoMode & t_videoMode);

      private:
        sf::RenderWindow m_window;
        Settings m_settings;
        util::Random m_random;
        util::SoundPlayer m_sfx;
        StateManager m_states;
        FontManager m_fonts;
        std::unique_ptr<Avatar> m_avatarUPtr;
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
        FloatingText m_floatText;

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
