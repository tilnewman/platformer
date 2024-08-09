#ifndef SETTINGS_HPP_INCLUDED
#define SETTINGS_HPP_INCLUDED
//
// settings.hpp
//
#include <filesystem>

#include <SFML/Window/VideoMode.hpp>

namespace platformer
{

    struct Settings
    {
        // see main.cpp for where this is really set
        std::filesystem::path media_path = { "./media" };

        sf::VideoMode video_mode = { 1920, 1080, 32 };

        // frames per second
        float frame_rate            = 60;
        bool will_display_fps       = false;
        bool will_display_fps_graph = false;
    };

} // namespace platformer

#endif // SETTINGS_HPP_INCLUDED
