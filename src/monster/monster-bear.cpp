// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-bear.hpp
//
#include "monster/monster-bear.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"

namespace bramblefore
{

    Bear::Bear(const Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::Bear, t_region, 0.6f, 1.2f } }
    {}

    sf::FloatRect Bear::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { { 0.0f, 0.0f }, { 0.0f, 0.0f } };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.6f, 0.4f });
            rect.position.x -= (rect.size.x * 0.2f);
            rect.position.y += (rect.size.y * 0.4f);

            if (!m_isFacingRight)
            {
                rect.position.x += (rect.size.x * 0.3f);
            }

            return rect;
        }
    }

    sf::FloatRect Bear::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };

        const float shiftHoriz{ rect.size.x * 0.4f };

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

    void Bear::turnAround()
    {
        m_sprite.scale({ -1.0f, 1.0f });

        const float imageWidthRatio{ 0.8f };
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

} // namespace bramblefore