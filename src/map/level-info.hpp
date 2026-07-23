#ifndef LEVEL_INFO_HPP_INCLUDED
#define LEVEL_INFO_HPP_INCLUDED
//
// level-info.hpp
//

namespace bramblefore
{
    struct Context;

    // There are a lot of "level" classes, but this one is responsible for managing all the
    // information about a level/map that is specific to that level.
    class LevelInfo
    {
      public:
        explicit LevelInfo();

        [[nodiscard]] constexpr int playerLives() const noexcept { return m_playerLives; }

        constexpr void playerLivesAdjust(const int t_adjustment) noexcept
        {
            m_playerLives += t_adjustment;
        }

        [[nodiscard]] constexpr int coins() const noexcept { return m_coins; }

        constexpr void coinsAdjust(const int t_adjustment) noexcept { m_coins += t_adjustment; }

        [[nodiscard]] constexpr int enemyKillCount() const noexcept { return m_enemyKillCount; } 

        constexpr void enemyKillCountAdjust(const int t_adjustment) noexcept
        {
            m_enemyKillCount += t_adjustment;
        }

        void resetForRespawn();
        void resetForNewLevel(const Context & t_context);

      private:
        int m_playerLives;
        int m_coins;
        int m_enemyKillCount;
    };

} // namespace bramblefore

#endif // PLAYER_INFO_HPP_INCLUDED
