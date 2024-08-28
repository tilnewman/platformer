#ifndef MONSTER_LITTLE_KNIGHT_HPP_INCLUDED
#define MONSTER_LITTLE_KNIGHT_HPP_INCLUDED
//
// monster-little-knight.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class LittleKnight : public Monster
    {
      public:
        LittleKnight(Context & t_context, const sf::FloatRect & t_region);
        virtual ~LittleKnight() override = default;

        // IMonster functions
        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace bramblefore

#endif // MONSTER_LITTLE_KNIGHT_HPP_INCLUDED
