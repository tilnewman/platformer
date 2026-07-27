// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-djinn.hpp
//
#include "monster/monster-djinn.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"

namespace bramblefore
{

    Djinn::Djinn(const Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::Djinn, t_region, 0.65f } }
    {}

    const sf::FloatRect Djinn::collisionRect() const
    {
        sf::FloatRect rect{ m_sprite.getGlobalBounds() };
        util::scaleRectInPlace(rect, { 0.3f, 0.4f });

        if (m_isFacingRight)
        {
            rect.position.x -= (rect.size.x * 0.35f);
        }
        else
        {
            rect.position.x += (rect.size.x * 0.35f);
        }

        return rect;
    }

    const sf::FloatRect Djinn::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };

        const float shiftHoriz{ rect.size.x * 0.8f };

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

    void Djinn::turnAround()
    {
        m_sprite.scale({ -1.0f, 1.0f });

        const float imageWidthRatio{ 0.85f };
        if (m_isFacingRight)
        {
            m_sprite.move(
                { -(m_sprite.getGlobalBounds().size.x * (1.0f - imageWidthRatio)), 0.0f });
        }
        else
        {
            m_sprite.move({ (m_sprite.getGlobalBounds().size.x * (1.0f - imageWidthRatio)), 0.0f });
        }

        m_isFacingRight = !m_isFacingRight;
    }

    void Djinn::startAttackAnimation(const Context & t_context)
    {
        const sf::FloatRect collRect{ collisionRect() };

        sf::Vector2f pos{ collRect.position };
        pos.y -= (collRect.size.y * 2.0f);

        const float horizOffset{ collRect.size.x * 1.25f };
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