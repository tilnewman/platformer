#ifndef MONSTER_DINO_HPP_INCLUDED
#define MONSTER_DINO_HPP_INCLUDED
//
// monster-dino.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Dino : public Monster
    {
      public:
        Dino(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Dino() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace bramblefore

#endif // MONSTER_DINO_HPP_INCLUDED
