// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monsters.hpp
//
#include "monsters.hpp"

#include "context.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace platformer
{

    MonsterManager::MonsterManager()
        : m_monsters()
    {}

    void MonsterManager::update(Context & context, const float frameTimeSec)
    {
        for (auto & monsterUPtr : m_monsters)
        {
            monsterUPtr->update(context, frameTimeSec);
        }
    }

    void MonsterManager::draw(
        const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const auto & monsterUPtr : m_monsters)
        {
            monsterUPtr->draw(context, target, states);
        }
    }

    void MonsterManager::move(const float amount) 
    {
        for (auto & monsterUPtr : m_monsters)
        {
            monsterUPtr->move(amount);
        }
    }

} // namespace platformer