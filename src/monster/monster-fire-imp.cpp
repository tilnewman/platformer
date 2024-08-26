// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-fire-imp.hpp
//
#include "monster/monster-fire-imp.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace platformer
{

    FireImp::FireImp(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::FireImp, t_region, 0.875f } }
    {}

    sf::FloatRect FireImp::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.15f, 0.2f });
            return rect;
        }
    }

    sf::FloatRect FireImp::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };

        const float shiftHoriz{ rect.width * 1.0f };

        if (m_isFacingRight)
        {
            rect.left += shiftHoriz;
        }
        else
        {
            rect.left -= shiftHoriz;
        }

        util::scaleRectInPlace(rect, { 2.0f, 1.1f });

        return rect;
    }

} // namespace platformer