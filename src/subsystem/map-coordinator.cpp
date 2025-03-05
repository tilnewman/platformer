// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// map-coordinator.hpp
//
#include "subsystem/map-coordinator.hpp"

#include "avatar/spells-anim.hpp"
#include "map/accents.hpp"
#include "map/level.hpp"
#include "map/pickups.hpp"
#include "player/player-info.hpp"
#include "state/state-manager.hpp"
#include "subsystem/background-images.hpp"
#include "subsystem/context.hpp"
#include "subsystem/floating-text.hpp"
#include "util/sound-player.hpp"

namespace bramblefore
{

    MapCoordinator::MapCoordinator()
        : m_mapNames{ "forest-1.json",
                      "mountains-1.json",
                      "dungeon1-1.json",
                      "cave-1.json",
                      "castle-1.json" }
        , m_mapNameIter{ std::begin(m_mapNames) }
    {}

    void MapCoordinator::respawn(Context & t_context)
    {
        t_context.accent.clear();
        t_context.pickup.clear();
        t_context.spell.clear();
        t_context.float_text.clear();

        t_context.player.healthReset(t_context);
        t_context.player.manaReset(t_context);
        t_context.player.mapStarReset(t_context);

        const std::string nextLevelFilename{ mapName() };
        if (nextLevelFilename.empty())
        {
            t_context.state.setChangePending(State::Credits);
        }
        else
        {
            t_context.level.load(t_context, nextLevelFilename);
            t_context.sfx.play("spawn");
        }
    }

    void MapCoordinator::deathBeforeDelay(Context & t_context)
    {
        t_context.sfx.play("death-avatar");
        // t_context.map_stats.has_player_died = true;
        // t_context.map_stats.enemy_killed    = 0;
        // t_context.map_stats.coin_collected  = 0;
    }

    void MapCoordinator::deathAfterDelay(Context & t_context)
    {
        t_context.state.setChangePending(State::LevelDeath);
    }

    std::string MapCoordinator::mapName() const
    {
        if (m_mapNameIter != std::end(m_mapNames))
        {
            return *m_mapNameIter;
        }
        else
        {
            return "";
        }
    }

    void MapCoordinator::mapNameAdvance()
    {
        if (m_mapNameIter != std::end(m_mapNames))
        {
            ++m_mapNameIter;
        }
    }

} // namespace bramblefore
