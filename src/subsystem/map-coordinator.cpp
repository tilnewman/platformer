// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// map-coordinator.hpp
//
#include "subsystem/map-coordinator.hpp"

#include "avatar/spells-anim.hpp"
#include "map/accents.hpp"
#include "map/level-info.hpp"
#include "map/level.hpp"
#include "map/pickups.hpp"
#include "player/player-info.hpp"
#include "state/state-manager.hpp"
#include "subsystem/context.hpp"
#include "subsystem/floating-text.hpp"
#include "util/sound-player.hpp"

#include <iterator>

namespace bramblefore
{

    MapCoordinator::MapCoordinator()
        : m_names{ { "castle-1.json", "Wightstone Castle" },
                   { "dungeon1-1.json", "Goblin Dungeon" },
                   { "forest-1.json", "Woodsland" },
                   { "mountains-1.json", "Lake Mountain" },
                   { "cave-1.json", "Lava Cavern" } }
        , m_namesIter{ std::begin(m_names) }
    {}

    void MapCoordinator::respawn(const Context & t_context)
    {
        t_context.level_info.resetForRespawn();

        t_context.accent.clear();
        t_context.pickup.clear();
        t_context.spell.clear();
        t_context.float_text.clear();

        t_context.player.healthReset(t_context);
        t_context.player.manaReset(t_context);
        t_context.player.mapStarReset(t_context);

        const std::string levelFilename{ filename() };
        if (levelFilename.empty())
        {
            t_context.state.setChangePending(State::Credits);
        }
        else
        {
            t_context.level.load(t_context, levelFilename);
            t_context.sfx.play("spawn");
        }
    }

    void MapCoordinator::deathBeforeDelay(const Context & t_context)
    {
        t_context.sfx.play("death-avatar");
        // t_context.level_info.has_player_died = true;
        // t_context.level_info.enemy_killed    = 0;
        // t_context.level_info.coin_collected  = 0;
    }

    void MapCoordinator::deathAfterDelay(const Context & t_context)
    {
        t_context.state.setChangePending(State::LevelDeath);
    }

    const std::string MapCoordinator::filename() const
    {
        if (m_namesIter == std::end(m_names))
        {
            return "";
        }
        else
        {
            return m_namesIter->file;
        }
    }

    const std::string MapCoordinator::displayName() const
    {
        if (m_namesIter == std::end(m_names))
        {
            return "";
        }
        else
        {
            return m_namesIter->display;
        }
    }

    void MapCoordinator::advance()
    {
        if (m_namesIter != std::end(m_names))
        {
            ++m_namesIter;
        }
    }

} // namespace bramblefore
