// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-demon.hpp
//
#include "monster/monster-demon.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace bramblefore
{

    Demon::Demon(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::Demon, t_region, 0.75f } }
    {}

    sf::FloatRect Demon::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { { 0.0f, 0.0f }, { 0.0f, 0.0f } };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.25f, 0.35f });
            rect.position.y += (rect.size.y * 0.2f);
            rect.position.x -= (rect.size.x * 0.4f);

            if (!m_isFacingRight)
            {
                rect.position.x += (rect.size.x * 0.8f);
            }

            return rect;
        }
    }

    sf::FloatRect Demon::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };

        const float shiftHoriz{ rect.size.x * 0.75f };

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

    void Demon::turnAround()
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