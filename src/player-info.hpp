#ifndef PLAYER_INFO_HPP_INCLUDED
#define PLAYER_INFO_HPP_INCLUDED
//
// player-info.hpp
//
#include "custom-types.hpp"

namespace platformer
{

    class PlayerInfo
    {
      public:
        PlayerInfo();

        Health_t health() const { return m_health; }
        Health_t healthAdjust(const Health_t adjustment);

        Coin_t coins() const { return m_coins; }
        Coin_t coinsAdjust(const Coin_t adjustment);

      private:
        Health_t m_health;
        Health_t m_healthMax;
        Coin_t m_coins;
    };

} // namespace platformer

#endif // PLAYER_INFO_HPP_INCLUDED
