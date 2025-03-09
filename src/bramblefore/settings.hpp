#ifndef SETTINGS_HPP_INCLUDED
#define SETTINGS_HPP_INCLUDED
//
// settings.hpp
//
#include <filesystem>

#include <SFML/Window/VideoMode.hpp>

namespace bramblefore
{

    struct Settings
    {
        // see main.cpp for where this is really set
        std::filesystem::path media_path = { "./media" };

        int player_lives_per_level = 3;

        float death_delay_sec = 4.0f;
        float idle_delay_sec  = 10.0f;

        sf::VideoMode video_mode    = sf::VideoMode(sf::Vector2u{ 1920u, 1080u }, 32u );
        float frame_rate            = 60; // change at your own risk
        bool will_log_fps           = false;
        bool will_display_fps_graph = true;

        float tile_scale    = 1.75f;
        float spell_scale   = 1.0f;
        float avatar_scale  = 1.5f;
        float monster_scale = 1.5f;

        float gravity_acc      = 25.0f;
        float walk_speed_limit = 2.5f;
        float run_speed_limit  = 3.5f;
        float walk_acc         = 12.0f;
        float run_acc          = 18.0f;
        float jump_acc         = 600.0f;
        float high_jump_acc    = 750.0f;
        float ladder_speed     = 60.0f;
    };

} // namespace bramblefore

#endif // SETTINGS_HPP_INCLUDED
