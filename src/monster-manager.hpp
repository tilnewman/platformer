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

        inline void add(std::unique_ptr<IMonster> monster)
        {
            m_monsters.push_back(std::move(monster));
        }

        inline void clear() { return m_monsters.clear(); }

        void update(Context & t_context, const float t_frameTimeSec);

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const;

        void move(const float t_amount);
        const Harm avatarCollide(const sf::FloatRect & t_avatarRect);
        void appendCollisionRects(std::vector<sf::FloatRect> & t_rects) const;
        bool avatarAttack(Context & t_context, const AttackInfo & t_attackInfo);

      private:
        std::vector<std::unique_ptr<IMonster>> m_monsters;
    };

} // namespace platformer

#endif // MONSTER_MANAGER_HPP_INCLUDED
