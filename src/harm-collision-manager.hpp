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
        [[nodiscard]] virtual Harm
            avatarCollide(Context & t_context, const sf::FloatRect & t_avatarRect) = 0;
    };

    //

    class HarmCollisionManager
    {
      public:
        static HarmCollisionManager & instance();

        void addOwner(IHarmCollisionOwner & t_owner);
        void removeOwner(IHarmCollisionOwner & t_owner);
        [[nodiscard]] Harm avatarCollide(Context & t_context, const sf::FloatRect & t_avatarRect);

      private:
        HarmCollisionManager();

      private:
        std::vector<std::reference_wrapper<IHarmCollisionOwner>> m_owners;
    };

} // namespace platformer

#endif // HARM_COLLISION_MANAGER_HPP_INCLUDED
