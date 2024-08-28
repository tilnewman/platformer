// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// player-info.hpp
//
#include "player/player-info.hpp"

#include "player/player-info-display.hpp"
#include "subsystem/context.hpp"

#include <algorithm>
#include <iostream>

namespace bramblefore
{

    PlayerInfo::PlayerInfo()
        : m_avatarType{ AvatarType::Count } // anything works here
        , m_health{ 0 }
        , m_healthMax{ 0 }
        , m_mana{ 0 }
        , m_manaMax{ 0 }
        , m_coins{ 0 }
        , m_spells{}
    {}

    void PlayerInfo::setup(Context & t_context, const AvatarType t_type)
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

        m_healthMax = startingHealth(t_type);
        healthReset(t_context);

        m_manaMax = startingMana(t_type);
        manaReset(t_context);
    }

    Health_t PlayerInfo::healthAdjust(Context & t_context, const Health_t t_adjustment)
    {
        m_health = std::clamp((m_health + t_adjustment), 0, m_healthMax);

        if (m_healthMax > 0)
        {
            t_context.player_display.setHealthBar(
                static_cast<float>(t_context.player.health()) /
                static_cast<float>(t_context.player.healthMax()));
        }

        return m_health;
    }

    void PlayerInfo::healthReset(Context & t_context)
    {
        m_health = 0;
        healthAdjust(t_context, m_healthMax);
    }

    Mana_t PlayerInfo::manaAdjust(Context & t_context, const Mana_t t_adjustment)
    {
        m_mana = std::clamp((m_mana + t_adjustment), 0, m_manaMax);

        if (m_manaMax > 0)
        {
            t_context.player_display.setHealthBar(
                static_cast<float>(t_context.player.mana()) /
                static_cast<float>(t_context.player.manaMax()));
        }

        return m_mana;
    }

    void PlayerInfo::manaReset(Context & t_context)
    {
        m_mana = 0;
        manaAdjust(t_context, m_manaMax);
    }

    Coin_t PlayerInfo::coinsAdjust(Context & t_context, const Coin_t t_adjustment)
    {
        m_coins += t_adjustment;

        if (m_coins < 0)
        {
            m_coins = 0;
        }

        t_context.player_display.setCoinCount(m_coins);

        return m_coins;
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

} // namespace bramblefore
