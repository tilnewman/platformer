// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-boss-tribal.hpp
//
#include "monster/monster-boss-tribal.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace bramblefore
{

    BossTribal::BossTribal(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::BossTribal, t_region, 0.7f } }
    {}

    sf::FloatRect BossTribal::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { { 0.0f, 0.0f }, { 0.0f, 0.0f } };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.25f, 0.35f });

            rect.position.y += (rect.size.x * 0.35f);

            if (m_isFacingRight)
            {
                rect.position.x -= (rect.size.x * 0.75f);
            }
            else
            {
                rect.position.x += (rect.size.x * 0.75f);
            }

            return rect;
        }
    }

    sf::FloatRect BossTribal::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };

        rect.size.x *= 1.8f;

        if (m_isFacingRight)
        {
            rect.position.x += (rect.size.x * 0.6f);
        }
        else
        {
            rect.position.x -= (rect.size.x * 1.0f);
        }

        util::scaleRectInPlace(rect, 1.1f);

        return rect;
    }

    void BossTribal::turnAround()
    {
        m_sprite.scale({ -1.0f, 1.0f });

        const float imageWidthRatio{ 0.6f };
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