// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// player-info.hpp
//
#include "player-info.hpp"

#include <algorithm>

namespace platformer
{

    PlayerInfo::PlayerInfo()
        : m_health{ 0 }
        , m_healthMax{ 100 }
        , m_mana{ 0 }
        , m_manaMax{ 100 }
        , m_coins{ 0 }
    {}

    void PlayerInfo::setup(const AvatarType type)
    {
        m_avatarType = type;

        //TODO setup spells table

        //TODO setup initial health and mana max based on type
    }

    Health_t PlayerInfo::healthAdjust(const Health_t adjustment)
    {
        m_health += adjustment;
        m_health = std::clamp(m_health, 0, m_healthMax);
        return m_health;
    }

    Mana_t PlayerInfo::manaAdjust(const Mana_t adjustment)
    {
        m_mana += adjustment;
        m_mana = std::clamp(m_mana, 0, m_manaMax);
        return m_mana;
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
