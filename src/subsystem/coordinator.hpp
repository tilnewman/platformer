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
#include "subsystem/blood-splat.hpp"
#include "subsystem/context.hpp"
#include "subsystem/floating-text.hpp"
#include "subsystem/font.hpp"
#include "subsystem/framerate-display.hpp"
#include "subsystem/map-coordinator.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/random.hpp"
#include "util/sound-player.hpp"

#include <memory>
#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

namespace bramblefore
{

    class Coordinator
    {
      public:
        explicit Coordinator(const Settings & t_setting);

        void play();

      private:
        void setup();
        void teardown();
        void gameLoop();
        void handleEvents();
        void handleEvent(const sf::Event & t_event);
        void update(const float t_frameTimeSec);
        void draw();
        void handleEndOfFrameTasks(const float t_actualFrameTimeSec);
        void setupRenderWindow(sf::VideoMode & t_videoMode);

      private:
        Settings m_setting;
        sf::RenderStates m_renderStates;

        std::unique_ptr<sf::RenderWindow> m_windowUPtr;
        std::unique_ptr<util::Random> m_randomUPtr;
        std::unique_ptr<util::SoundPlayer> m_sfxUPtr;
        std::unique_ptr<StateManager> m_stateUPtr;
        std::unique_ptr<FontManager> m_fontUPtr;
        std::unique_ptr<Avatar> m_avatarUPtr;
        std::unique_ptr<ScreenLayout> m_layoutUPtr;
        std::unique_ptr<LevelFileLoader> m_levelLoaderUPtr;
        std::unique_ptr<Level> m_levelUPtr;
        std::unique_ptr<BackgroundImages> m_backgroundImageUPtr;
        std::unique_ptr<PickupAnimations> m_pickupUPtr;
        std::unique_ptr<AccentAnimations> m_accentUPtr;
        std::unique_ptr<SpellAnimations> m_spellUPtr;
        std::unique_ptr<ItemImages> m_itemImageUPtr;
        std::unique_ptr<LevelInfo> m_levelInfoUPtr;
        std::unique_ptr<PlayerInfo> m_playerInfoUPtr;
        std::unique_ptr<PlayerInfoDisplay> m_playerInfoDisplayUPtr;
        std::unique_ptr<FloatingText> m_floatingTextUPtr;
        std::unique_ptr<MapCoordinator> m_mapCoordUPtr;
        std::unique_ptr<FramerateDisplay> m_framerateDisplayUPtr;
        std::unique_ptr<BloodSplatManager> m_bloodSplatManagerUPtr;

        std::unique_ptr<Context> m_contextUPtr;
    };

} // namespace bramblefore

#endif // COORDINATOR_HPP_INCLUDED
