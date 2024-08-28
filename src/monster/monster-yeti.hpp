#ifndef MONSTER_YETI_HPP_INCLUDED
#define MONSTER_YETI_HPP_INCLUDED
//
// monster-yeti.hpp
//
#include "monster/monster.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    class Yeti : public Monster
    {
      public:
        Yeti(Context & t_context, const sf::FloatRect & t_region);
        virtual ~Yeti() override = default;

        sf::FloatRect collisionRect() const final;
        sf::FloatRect attackCollisionRect() const final;
    };

} // namespace bramblefore

#endif // MONSTER_YETI_HPP_INCLUDED
