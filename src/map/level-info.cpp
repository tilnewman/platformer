//
// level-info.cpp
//
#include "level-info.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"

namespace bramblefore
{

    LevelInfo::LevelInfo()
        : m_playerLives{ 0 }
        , m_coins{ 0 }
        , m_enemyKillCount{ 0 }
    {}

    void LevelInfo::resetForRespawn()
    {
        m_coins          = 0;
        m_enemyKillCount = 0;
    }

    void LevelInfo::resetForNewLevel(const Context & t_context)
    {
        m_playerLives = t_context.settings.player_lives_per_level;
        resetForRespawn();
    }

} // namespace bramblefore
