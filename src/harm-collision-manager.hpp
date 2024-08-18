#ifndef HARM_COLLISION_MANAGER_HPP_INCLUDED
#define HARM_COLLISION_MANAGER_HPP_INCLUDED
//
// harm-collision-manager.hpp
//
#include "harm.hpp"

#include <vector>

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{
    struct Context;
    struct Settings;

    //

    struct IHarmCollisionOwner
    {
        virtual ~IHarmCollisionOwner() = default;
        virtual const Harm avatarCollide(Context & context, const sf::FloatRect & avatarRect) = 0;
    };

    //

    class HarmCollisionManager
    {
      public:
        static HarmCollisionManager & instance();

        void addOwner(IHarmCollisionOwner & owner);
        void removeOwner(IHarmCollisionOwner & owner);
        const Harm avatarCollide(Context & context, const sf::FloatRect & avatarRect);

      private:
        HarmCollisionManager();

      private:
        std::vector<std::reference_wrapper<IHarmCollisionOwner>> m_owners;
    };

} // namespace platformer

#endif // HARM_COLLISION_MANAGER_HPP_INCLUDED
