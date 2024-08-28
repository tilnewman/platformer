// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-manager.hpp
//
#include "monster/monster-manager.hpp"

#include "subsystem/context.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    MonsterManager::MonsterManager()
        : m_monsters{}
    {}

    void MonsterManager::update(Context & t_context, const float t_frameTimeSec)
    {
        for (auto & monsterUPtr : m_monsters)
        {
            monsterUPtr->update(t_context, t_frameTimeSec);
        }
    }

    void MonsterManager::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const auto & monsterUPtr : m_monsters)
        {
            monsterUPtr->draw(t_context, t_target, t_states);
        }
    }

    void MonsterManager::move(const float t_amount)
    {
        for (auto & monsterUPtr : m_monsters)
        {
            monsterUPtr->move(t_amount);
        }
    }

    Harm MonsterManager::avatarCollide(const sf::FloatRect & t_avatarRect)
    {
        for (auto & monsterUPtr : m_monsters)
        {
            const Harm harm{ monsterUPtr->avatarCollide(t_avatarRect) };
            if (harm.isAnyHarmDone())
            {
                return harm;
            }
        }

        return Harm{};
    }

    void MonsterManager::appendCollisionRects(std::vector<sf::FloatRect> & t_rects) const
    {
        for (auto & monsterUPtr : m_monsters)
        {
            t_rects.push_back(monsterUPtr->collisionRect());
        }
    }

    bool MonsterManager::avatarAttack(Context & t_context, const AttackInfo & t_attackInfo)
    {
        for (auto & monsterUPtr : m_monsters)
        {
            if (monsterUPtr->avatarAttack(t_context, t_attackInfo))
            {
                return true;
            }
        }

        return false;
    }

} // namespace bramblefore