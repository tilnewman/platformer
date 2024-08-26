// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// harm-collision-manager.cpp
//
#include "subsystem/harm-collision-manager.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"

namespace platformer
{

    HarmCollisionManager::HarmCollisionManager()
        : m_owners{}
    {}

    HarmCollisionManager & HarmCollisionManager::instance()
    {
        static HarmCollisionManager instance;
        return instance;
    }

    void HarmCollisionManager::addOwner(IHarmCollisionOwner & t_owner)
    {
        m_owners.push_back(t_owner);
    }

    void HarmCollisionManager::removeOwner(IHarmCollisionOwner & t_owner)
    {
        std::erase_if(
            m_owners, [&](const std::reference_wrapper<IHarmCollisionOwner> & refWrapper) {
                return (&t_owner == &refWrapper.get());
            });
    }

    Harm
        HarmCollisionManager::avatarCollide(Context & t_context, const sf::FloatRect & t_avatarRect)
    {
        for (auto & ownerRefWrapper : m_owners)
        {
            const Harm harm{ ownerRefWrapper.get().avatarCollide(t_context, t_avatarRect) };
            if (harm.isAnyHarmDone())
            {
                return harm;
            }
        }

        return Harm{};
    }

} // namespace platformer
