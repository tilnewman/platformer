#ifndef LEVEL_INFO_HPP_INCLUDED
#define LEVEL_INFO_HPP_INCLUDED
//
// level-info.hpp
//
#include "bramblefore/settings.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

namespace bramblefore
{

    class LevelInfo
    {
      public:
        explicit LevelInfo(const Settings & t_settings)
            : m_playerLives{ t_settings.player_lives_per_level }
        {}

        [[nodiscard]] inline int playerLives() const noexcept { return m_playerLives; }

        inline void playerLivesAdjust(const int t_adjustment) noexcept
        {
            m_playerLives += t_adjustment;
        }

      private:
        int m_playerLives;
    };

} // namespace bramblefore

#endif // PLAYER_INFO_HPP_INCLUDED
