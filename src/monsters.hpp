#ifndef MONSTERS_HPP_INCLUDED
#define MONSTERS_HPP_INCLUDED
//
// monsters.hpp
//
#include "harm.hpp"

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

    struct IMonster
    {
        virtual ~IMonster() = default;

        virtual void update(Context & context, const float frameTimeSec)                     = 0;
        virtual void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const = 0;
        virtual void move(const float amount)                                                = 0;
        virtual const Harm avatarCollide(const sf::FloatRect & avatarRect)                   = 0;
    };

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

      private:
        std::vector<std::unique_ptr<IMonster>> m_monsters;
    };

} // namespace platformer

#endif // MONSTERS_HPP_INCLUDED
