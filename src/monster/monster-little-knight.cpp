// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-little-knight.hpp
//
#include "monster/monster-little-knight.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace platformer
{

    LittleKnight::LittleKnight(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::LittleKnight, t_region, 0.85f } }
    {}

    sf::FloatRect LittleKnight::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.2f, 0.2f });
            rect.top += (rect.height * 0.2f);
            return rect;
        }
    }

    sf::FloatRect LittleKnight::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };

        const float shiftHoriz{ rect.width * 0.7f };

        if (m_isFacingRight)
        {
            rect.left += shiftHoriz;
        }
        else
        {
            rect.left -= shiftHoriz;
        }

        util::scaleRectInPlace(rect, 1.1f);

        return rect;
    }

} // namespace platformer