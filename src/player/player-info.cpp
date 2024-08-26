// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// player-info.hpp
//
#include "player/player-info.hpp"

#include <algorithm>
#include <iostream>

namespace platformer
{

    PlayerInfo::PlayerInfo()
        : m_avatarType{ AvatarType::Count } // anything works here
        , m_health{ 0 }
        , m_healthMax{ 100 }
        , m_mana{ 0 }
        , m_manaMax{ 100 }
        , m_coins{ 0 }
        , m_spells{}
    {}

    void PlayerInfo::setup(const AvatarType t_type)
    {
        m_avatarType = t_type;

        m_spells.clear();
        for (const Spell spell : makeSpellSet(t_type))
        {
            PlayerSpell & playerSpell{ m_spells.emplace_back() };
            playerSpell.cost       = toManaCost(spell);
            playerSpell.damage     = toDamage(spell);
            playerSpell.is_learned = false;
            playerSpell.spell      = spell;
        }

        // TODO setup initial health and mana max based on type
    }

    void PlayerInfo::learnSpell(const Spell t_spell)
    {
        const auto iter{ std::find_if(
            std::begin(m_spells), std::end(m_spells), [&](const PlayerSpell & ps) {
                return (ps.spell == t_spell);
            }) };

        if (iter == std::end(m_spells))
        {
            std::cout << "Error:  PlayerInfo::learnSpell(" << toName(t_spell) << ") failed for "
                      << toString(m_avatarType) << ".\n";
        }
        else
        {
            iter->is_learned = true;
        }
    }

} // namespace platformer
