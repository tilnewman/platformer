#ifndef MONSTER_MANAGER_HPP_INCLUDED
#define MONSTER_MANAGER_HPP_INCLUDED
//
// monster-manager.hpp
//
#include "harm.hpp"
#include "monster.hpp"

#include <memory>
#include <vector>

#include <SFML/Graphics/Rect.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
} // namespace sf

namespace platformer
{

    struct Context;

    //

    class MonsterManager
    {
      public:
        MonsterManager();

        inline void add(std::unique_ptr<IMonster> m) { m_monsters.push_back(std::move(m)); }
        inline void clear() { return m_monsters.clear(); }

        void update(Context & context, const float frameTimeSec);
        void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const;
        void move(const float amount);
        const Harm avatarCollide(const sf::FloatRect & avatarRect);
        void appendCollisionRects(std::vector<sf::FloatRect> & rects) const;
        void avatarAttack(Context & context, const AttackInfo & attackInfo);

      private:
        std::vector<std::unique_ptr<IMonster>> m_monsters;
    };

} // namespace platformer

#endif // MONSTER_MANAGER_HPP_INCLUDED
