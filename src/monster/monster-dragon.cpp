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
        : Monster{ t_context,
                   { MonsterType::Dragon, t_region, 0.75f, 1.0f, MonsterSpell::DragonFire } }
    {}

    sf::FloatRect Dragon::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { { 0.0f, 0.0f }, { 0.0f, 0.0f } };
        }
        else
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
    }

    sf::FloatRect Dragon::attackCollisionRect() const
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

    void Dragon::startAttackAnimation(Context &)
    {
        const sf::FloatRect collRect{ collisionRect() };

        sf::Vector2f pos;
        pos.y = collRect.position.y;

        if (m_isFacingRight)
        {
            pos.x = util::right(collRect);
        }
        else
        {
            pos.x = collRect.position.x;
        }

        m_animations.add(pos, m_spell, m_isFacingRight);
    }

} // namespace bramblefore