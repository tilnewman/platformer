// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-ghost.hpp
//
#include "monster/monster-ghost.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace platformer
{

    Ghost::Ghost(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::Ghost, t_region, 0.7f } }
    {}

    const sf::FloatRect Ghost::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.3f, 0.4f });
            return rect;
        }
    }

    const sf::FloatRect Ghost::attackCollisionRect() const
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

    const Harm Ghost::avatarCollide(const sf::FloatRect & avatarRect)
    {
        Harm harm;

        if ((MonsterAnim::Attack == m_anim) && avatarRect.intersects(attackCollisionRect()))
        {
            harm.damage = attackDamage(m_type);
            harm.rect   = collisionRect();
            harm.sfx    = "hit-wood";
        }

        return harm;
    }

    void Ghost::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void Ghost::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void Ghost::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

} // namespace platformer