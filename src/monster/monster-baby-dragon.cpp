// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-baby-dragon.hpp
//
#include "monster/monster-baby-dragon.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace platformer
{

    BabyDragon::BabyDragon(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::BabyDragon, t_region, 0.85f } }
    {}

    sf::FloatRect BabyDragon::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.4f, 0.25f });
            return rect;
        }
    }

    sf::FloatRect BabyDragon::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };

        const float shiftHoriz{ rect.width * 0.6f };

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

    void BabyDragon::playAttackSfx(Context & t_context) const { t_context.sfx.play("attack-ent"); }

    void BabyDragon::playHurtSfx(Context & t_context) const { t_context.sfx.play("hurt-ent"); }

    void BabyDragon::playDeathSfx(Context & t_context) const { t_context.sfx.play("death-ent"); }

} // namespace platformer