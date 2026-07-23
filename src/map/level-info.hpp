#ifndef LEVEL_INFO_HPP_INCLUDED
#define LEVEL_INFO_HPP_INCLUDED
//
// level-info.hpp
//
#include "player/custom-types.hpp"

namespace bramblefore
{
    struct Context;

    // There are a lot of "level" classes, but this one is responsible for managing all the
    // information about a level/map that is specific to that level.  So the coins tracked here are
    // not how many the player has, but how manby they have gathered on the current level only.
    class LevelInfo
    {
      public:
        explicit LevelInfo();

        [[nodiscard]] constexpr int playerLives() const noexcept { return m_playerLives; }

        constexpr void playerLivesAdjust(const int t_adjustment) noexcept
        {
            m_playerLives += t_adjustment;
        }

        [[nodiscard]] constexpr int coin() const noexcept { return m_coins; }

        constexpr void coinAdjust(const Coin_t t_adjustment) noexcept { m_coins += t_adjustment; }

        [[nodiscard]] constexpr Coin_t enemyKillCount() const noexcept { return m_enemyKillCount; }

        constexpr void enemyKillCountAdjust(const int t_adjustment) noexcept
        {
            m_enemyKillCount += t_adjustment;
        }

        void resetForRespawn();
        void resetForNewLevel(const Context & t_context);

      private:
        int m_playerLives;
        Coin_t m_coins;
        int m_enemyKillCount;
    };

} // namespace bramblefore

#endif // PLAYER_INFO_HPP_INCLUDED
