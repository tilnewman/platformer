#ifndef COORDINATOR_HPP_INCLUDED
#define COORDINATOR_HPP_INCLUDED
//
// coordinator.hpp
//
#include "avatar.hpp"
#include "context.hpp"
#include "random.hpp"
#include "settings.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
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
        void setupRenderWindow(sf::VideoMode & videoMode);

      private:
        sf::RenderWindow m_window;
        Settings m_settings;
        util::Random m_random;
        AvatarTextureManager m_avatarTextures;
        Avatar m_avatar;

        Context m_context;
    };

} // namespace platformer

#endif // COORDINATOR_HPP_INCLUDED
