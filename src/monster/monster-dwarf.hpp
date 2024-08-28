#ifndef MONSTER_DWARF_HPP_INCLUDED
#define MONSTER_DWARF_HPP_INCLUDED
//
// monster-dwarf.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Dwarf : public Monster
    {
      public:
        Dwarf(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Dwarf() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;

      protected:
        void turnAround() final;
    };

} // namespace bramblefore

#endif // MONSTER_BEAR_HPP_INCLUDED
