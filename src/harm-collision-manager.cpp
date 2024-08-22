// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// harm-collision-manager.cpp
//
#include "harm-collision-manager.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"

namespace platformer
{

    HarmCollisionManager & HarmCollisionManager::instance()
    {
        static HarmCollisionManager instance;
        return instance;
    }

    HarmCollisionManager::HarmCollisionManager()
        : m_owners()
    {}

    void HarmCollisionManager::addOwner(IHarmCollisionOwner & owner) { m_owners.push_back(owner); }

    void HarmCollisionManager::removeOwner(IHarmCollisionOwner & owner)
    {
        m_owners.erase(
            std::remove_if(
                std::begin(m_owners),
                std::end(m_owners),
                [&](const std::reference_wrapper<IHarmCollisionOwner> & refWrapper) {
                    return (&owner == &refWrapper.get());
                }),
            std::end(m_owners));
    }

    Harm HarmCollisionManager::avatarCollide(Context & context, const sf::FloatRect & avatarRect)
    {
        for (auto & ownerRefWrapper : m_owners)
        {
            const Harm harm{ ownerRefWrapper.get().avatarCollide(context, avatarRect) };
            if (harm.isAnyHarmDone())
            {
                return harm;
            }
        }

        return Harm{};
    }

} // namespace platformer
