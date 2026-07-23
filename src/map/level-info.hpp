#ifndef LEVEL_INFO_HPP_INCLUDED
#define LEVEL_INFO_HPP_INCLUDED
//
// level-info.hpp
//
#include "bramblefore/settings.hpp"

namespace bramblefore
{

    // There are a lot of "level" classes, but this one is responsible for managing all the
    // information about a level/map that is specific to that level.
    class LevelInfo
    {
      public:
        explicit LevelInfo(const Settings & t_settings)
            : m_playerLives{ t_settings.player_lives_per_level }
        {}

        [[nodiscard]] constexpr int playerLives() const noexcept { return m_playerLives; }

        inline void playerLivesAdjust(const int t_adjustment) noexcept
        {
            m_playerLives += t_adjustment;
        }

      private:
        int m_playerLives;
    };

} // namespace bramblefore

#endif // PLAYER_INFO_HPP_INCLUDED
