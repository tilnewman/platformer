// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// player-info.hpp
//
#include "player-info.hpp"

namespace platformer
{

    PlayerInfo::PlayerInfo()
        : m_health(0)
        , m_healthMax(100)
        , m_coins(0)
    {}

    Health_t PlayerInfo::healthAdjust(const Health_t adjustment)
    {
        m_health += adjustment;

        if (m_health < 0)
        {
            m_health = 0;
        }
        else if (m_health > m_healthMax)
        {
            m_health = m_healthMax;
        }

        return m_health;
    }

    Coin_t PlayerInfo::coinsAdjust(const Coin_t adjustment) 
    {
        m_coins += adjustment;

        if (m_coins < 0)
        {
            m_coins = 0;
        }

        return m_coins;
    }

} // namespace platformer
