#ifndef MONSTER_HOUND_HPP_INCLUDED
#define MONSTER_HOUND_HPP_INCLUDED
//
// monster-hound.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Hound : public Monster
    {
      public:
        Hound(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Hound() override = default;

        // IMonster functions
        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace bramblefore

#endif // MONSTER_HOUND_HPP_INCLUDED
