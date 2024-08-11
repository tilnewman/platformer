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

        float tile_scale   = 1.75f;
        float spell_scale  = 1.0f;
        float avatar_scale = 1.5f;

        float pickups_time_per_frame     = 0.1f;
        float accent_fire_time_per_frame = 0.15f;
        float accent_vine_time_per_frame = 0.75f;
        float spell_time_per_frame       = 0.1f;
    };

} // namespace platformer

#endif // SETTINGS_HPP_INCLUDED
