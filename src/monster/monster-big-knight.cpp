// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-big-knight.hpp
//
#include "monster/monster-big-knight.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace platformer
{

    BigKnight::BigKnight(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::BigKnight, t_region, 0.835f } }
    {}

    const sf::FloatRect BigKnight::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.25f, 0.35f });
            return rect;
        }
    }

    const sf::FloatRect BigKnight::attackCollisionRect() const
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

    const Harm BigKnight::avatarCollide(const sf::FloatRect & avatarRect)
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

    void BigKnight::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void BigKnight::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void BigKnight::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

} // namespace platformer