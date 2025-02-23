// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// player-info.hpp
//
#include "player/player-info.hpp"

#include "avatar/spells-anim.hpp"
#include "player/player-info-display.hpp"
#include "subsystem/context.hpp"
#include "util/check-macros.hpp"
#include "util/sound-player.hpp"

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
        , m_currentSpell{ 0 }
        , m_spells{}
        , m_elapsedTimeSec{ 0.0f }
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
            playerSpell.is_learned = true; // TODO set to false when done showing off
            playerSpell.spell      = spell;
        }

        // spellcasters always have the first spell learned when the game begins
        if (!m_spells.empty())
        {
            m_spells.front().is_learned = true;
        }

        m_healthMax = startingHealth(t_type);
        healthReset(t_context);

        m_manaMax = startingMana(t_type);
        manaReset(t_context);
    }

    void PlayerInfo::update(Context & t_context, const float t_frameTimeSec)
    {
        m_elapsedTimeSec += t_frameTimeSec;
        if (m_elapsedTimeSec > 1.0f)
        {
            m_elapsedTimeSec -= 1.0f;
            manaAdjust(t_context, 1);
        }
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
            // TODO log an error here
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
            // TODO log an error here
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

    void PlayerInfo::setCurentSpellIndex(const std::size_t newSpellIndex)
    {
        M_CHECK(
            (m_currentSpell < m_spells.size()),
            "newSpellIndex=" << newSpellIndex << " but spell list size=" << m_spells.size());

        const PlayerSpell & spell{ m_spells.at(newSpellIndex) };

        M_CHECK(
            spell.is_learned,
            "newSpellIndex=" << newSpellIndex << " but that spell \"" << toName(spell.spell)
                             << "\" is not learned");

        m_currentSpell = newSpellIndex;
    }

    Spell PlayerInfo::currentSpell() const
    {
        M_CHECK(
            (m_currentSpell < m_spells.size()),
            "m_currentSpell=" << m_currentSpell << " but spell list size=" << m_spells.size());

        const PlayerSpell & spell{ m_spells.at(m_currentSpell) };

        M_CHECK(
            spell.is_learned,
            "m_currentSpell=" << m_currentSpell << " but that spell \"" << toName(spell.spell)
                              << "\" is not learned");

        return spell.spell;
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

    void PlayerInfo::castCurrentSpell(Context & t_context, const sf::Vector2f & pos)
    {
        const Spell spell{ t_context.player.currentSpell() };
        const int manaCost{ toManaCost(spell) };

        if (m_mana >= manaCost)
        {
            t_context.spell.add(pos, spell);
            manaAdjust(t_context, -manaCost);
        }
        else
        {
            t_context.sfx.play("ui-reject-3");
        }
    }

} // namespace bramblefore
