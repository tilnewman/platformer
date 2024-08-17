#ifndef COORDINATOR_HPP_INCLUDED
#define COORDINATOR_HPP_INCLUDED
//
// coordinator.hpp
//
#include "accents.hpp"
#include "avatar-textures.hpp"
#include "avatar.hpp"
#include "background-images.hpp"
#include "context.hpp"
#include "font.hpp"
#include "item-images.hpp"
#include "kill-collision-manager.hpp"
#include "level-file-loader.hpp"
#include "level-info.hpp"
#include "level.hpp"
#include "map-textures.hpp"
#include "pickups.hpp"
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
        Coordinator(const Settings & settings);

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
        AvatarTextureManager m_avatarTextures;
        ScreenLayout m_layout;
        LevelFileLoader m_levelLoader;
        Level m_level;
        MapTextures m_mapTextures;
        BackgroundImages m_backgroundImages;
        PickupAnimations m_pickups;
        AccentAnimations m_accents;
        SpellAnimations m_spells;
        ItemImages m_itemImages;
        LevelInfo m_levelInfo;

        Context m_context;

        // fps stuff
        std::vector<std::size_t> m_fpsValues;
        sf::Clock m_oneSecondClock;
        float m_elapsedTimeSec;
        std::unique_ptr<util::GraphDisplay<std::size_t>> m_statsDisplayUPtr;
    };

} // namespace platformer

#endif // COORDINATOR_HPP_INCLUDED
