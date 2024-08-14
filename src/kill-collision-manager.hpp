#ifndef KILL_COLLISION_MANAGER_HPP_INCLUDED
#define KILL_COLLISION_MANAGER_HPP_INCLUDED
//
// kill-collision-manager.hpp
//
#include <vector>

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{
    struct Context;
    struct Settings;

    //

    struct IKillCollisionOwner
    {
        virtual ~IKillCollisionOwner() = default;

        virtual bool
            doesAvatarCollideWithAnyAndDie(Context & context, const sf::FloatRect & avatarRect) = 0;
    };

    //

    class KillCollisionManager
    {
      public:
        static KillCollisionManager & instance();

        void addOwner(IKillCollisionOwner & owner);
        void removeOwner(IKillCollisionOwner & owner);
        bool doesAvatarCollideWithAnyAndDie(Context & context, const sf::FloatRect & avatarRect);

      private:
        KillCollisionManager();

      private:
        std::vector<std::reference_wrapper<IKillCollisionOwner>> m_owners;
    };

} // namespace platformer

#endif // KILL_COLLISION_MANAGER_HPP_INCLUDED
