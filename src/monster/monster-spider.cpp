// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-spider.hpp
//
#include "monster/monster-spider.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace platformer
{

    Spider::Spider(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::Spider, t_region, 0.8f } }
    {}

    sf::FloatRect Spider::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.5f, 0.35f });
            return rect;
        }
    }

    sf::FloatRect Spider::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };

        const float shiftHoriz{ rect.width * 0.35f };

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

    void Spider::playAttackSfx(Context & context) const { context.sfx.play("attack-spider"); }

    void Spider::playHurtSfx(Context & context) const { context.sfx.play("hurt-spider"); }

    void Spider::playDeathSfx(Context & context) const { context.sfx.play("death-spider"); }

} // namespace platformer