#ifndef LEVEL_INFO_HPP_INCLUDED
#define LEVEL_INFO_HPP_INCLUDED
//
// level-info.hpp
//
#include "settings.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

namespace platformer
{

    class LevelInfo
    {
      public:
        explicit LevelInfo(const Settings & settings)
            : m_playerLives(settings.player_lives_per_level)
        {}

        inline int playerLives() const { return m_playerLives; }
        inline void playerLivesAdjust(const int adjustment) { m_playerLives += adjustment; }

      private:
        int m_playerLives;
    };

} // namespace platformer

#endif // PLAYER_INFO_HPP_INCLUDED
