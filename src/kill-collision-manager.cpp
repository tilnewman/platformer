// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// kill-collision-manager.cpp
//
#include "kill-collision-manager.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"

namespace platformer
{

    KillCollisionManager & KillCollisionManager::instance()
    {
        static KillCollisionManager instance;
        return instance;
    }

    KillCollisionManager::KillCollisionManager()
        : m_owners()
    {}

    void KillCollisionManager::addOwner(IKillCollisionOwner & owner) { m_owners.push_back(owner); }

    void KillCollisionManager::removeOwner(IKillCollisionOwner & owner)
    {
        m_owners.erase(
            std::remove_if(
                std::begin(m_owners),
                std::end(m_owners),
                [&](const std::reference_wrapper<IKillCollisionOwner> & refWrapper) {
                    return (&owner == &refWrapper.get());
                }),
            std::end(m_owners));
    }

    bool KillCollisionManager::doesAvatarCollideWithAnyAndDie(
        Context & context, const sf::FloatRect & avatarRect)
    {
        bool didAnyCollide{ false };
        for (auto & ownerRefWrapper : m_owners)
        {
            if (ownerRefWrapper.get().doesAvatarCollideWithAnyAndDie(context, avatarRect))
            {
                didAnyCollide = true;
            }
        }

        return didAnyCollide;
    }

} // namespace platformer
