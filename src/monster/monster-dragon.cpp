// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-dragon.hpp
//
#include "monster/monster-dragon.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"

namespace bramblefore
{

    Dragon::Dragon(const Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::Dragon, t_region, 0.75f, 1.0f } }
    {}

    const sf::FloatRect Dragon::collisionRect() const
    {
        sf::FloatRect rect{ m_sprite.getGlobalBounds() };
        util::scaleRectInPlace(rect, { 0.4f, 0.25f });
        rect.position.y += (rect.size.y * 0.5f);

        if (m_isFacingRight)
        {
            rect.position.x += (rect.size.x * 0.2f);
        }
        else
        {
            rect.position.x -= (rect.size.x * 0.2f);
        }

        return rect;
    }

    const sf::FloatRect Dragon::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };

        const float shiftHoriz{ rect.size.x * 0.6f };

        if (m_isFacingRight)
        {
            rect.position.x += shiftHoriz;
        }
        else
        {
            rect.position.x -= shiftHoriz;
        }

        util::scaleRectInPlace(rect, 1.1f);

        return rect;
    }

    void Dragon::startAttackAnimation(const Context & t_context)
    {
        const sf::FloatRect collRect{ collisionRect() };

        sf::Vector2f pos{ collRect.position };
        pos.y -= (collRect.size.y * 0.75f);

        const float horizOffset{ collRect.size.x * 0.35f };
        if (m_isFacingRight)
        {
            pos.x = util::right(collRect);
            pos.x -= horizOffset;
        }
        else
        {
            pos.x += horizOffset;
        }

        m_attackAnims.add(t_context, m_type, pos, m_isFacingRight);
    }

} // namespace bramblefore