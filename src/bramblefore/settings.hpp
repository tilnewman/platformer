#ifndef SETTINGS_HPP_INCLUDED
#define SETTINGS_HPP_INCLUDED
//
// settings.hpp
//
#include <filesystem>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/VideoMode.hpp>

namespace bramblefore
{

    struct Settings
    {
        // see main.cpp for where this is really set
        std::filesystem::path media_path{ "./media" };

        int player_lives_per_level = 3;

        float death_delay_sec = 4.0f;
        float idle_delay_sec  = 10.0f;

        sf::VideoMode video_mode{ { 1920u, 1080u }, 32u };

        unsigned frame_rate         = 60u;
        bool will_display_fps       = true;
        bool will_display_fps_graph = false;

        // These are related, so if you half or double one of these then you should also do that to
        // the others, except for very minor tweaks.
        float map_scale     = 1.75f;
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

        // this magic number brought to you by zTn 2021-8-2
        float avatar_collision_tolerance = 25.0f;

        // these values are used to create the avatar collision rect
        sf::Vector2f avatar_collision_scale{ 0.25f, 0.35f };
        float avatar_collision_facing_offset_ratio = 0.15f;
        bool avatar_collision_display              = false;

        // Allow moving side-to-side at a reduced speed while in the air.
        // It sounds wrong but feels so right. What the hell, mario did it.
        float avatar_jump_horiz_move_divisor = 3.0f;

        sf::Vector2f avatar_hurt_recoil_speed{ 2.25f, 3.5f };

        sf::Color off_white_color{ 220, 220, 220 };
    };

} // namespace bramblefore

#endif // SETTINGS_HPP_INCLUDED
