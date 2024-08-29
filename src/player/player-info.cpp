// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// player-info.hpp
//
#include "player/player-info.hpp"

#include "player/player-info-display.hpp"
#include "subsystem/context.hpp"
#include "util/check-macros.hpp"

#include <algorithm>

namespace bramblefore
{

    PlayerInfo::PlayerInfo()
        : m_avatarType{ AvatarType::Count } // anything works here
        , m_health{ 0 }
        , m_healthMax{ 0 }
        , m_mana{ 0 }
        , m_manaMax{ 0 }
        , m_coins{ 0 }
        , m_experience{ 0 }
        , m_mapStarCount{ 0 }
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
            t_context.player_display.setManaBar(
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

    Coin_t PlayerInfo::coinAdjust(Context & t_context, const Coin_t t_adjustment)
    {
        m_coins += t_adjustment;

        if (m_coins < 0)
        {
            m_coins = 0;
        }

        t_context.player_display.setCoinCount(m_coins);

        return m_coins;
    }

    Experience_t PlayerInfo::experienceAdjust(const Experience_t adjustment)
    {
        m_experience += adjustment;

        if (m_experience < 0)
        {
            m_experience = 0;
        }

        return m_experience;
    }

    void PlayerInfo::mapStarCollect(Context & t_context)
    {
        ++m_mapStarCount;
        t_context.player_display.setStarCount(m_mapStarCount);
    }

    void PlayerInfo::mapStarReset(Context & t_context)
    {
        m_mapStarCount = 0;
        t_context.player_display.setStarCount(m_mapStarCount);
    }

    void PlayerInfo::learnSpell(const Spell t_spell)
    {
        const auto iter{ std::ranges::find_if(
            m_spells, [&](const PlayerSpell & ps) { return (ps.spell == t_spell); }) };

        M_CHECK(
            (iter != std::end(m_spells)),
            toName(t_spell) << " spell was not found for " << toString(m_avatarType));

        iter->is_learned = true;
    }

} // namespace bramblefore
