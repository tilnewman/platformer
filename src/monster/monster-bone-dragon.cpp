// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-bone-dragon.hpp
//
#include "monster/monster-bone-dragon.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"

namespace bramblefore
{

    BoneDragon::BoneDragon(const Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::BoneDragon, t_region, 0.87f } }
    {}

    sf::FloatRect BoneDragon::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { { 0.0f, 0.0f }, { 0.0f, 0.0f } };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.3f, 0.25f });
            return rect;
        }
    }

    sf::FloatRect BoneDragon::attackCollisionRect() const
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

} // namespace bramblefore