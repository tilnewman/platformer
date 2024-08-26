// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-medusa.hpp
//
#include "monster/monster-medusa.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace platformer
{

    Medusa::Medusa(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::Medusa, t_region, 0.75f, 1.1f } }
    {}

    const sf::FloatRect Medusa::collisionRect() const
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

    const sf::FloatRect Medusa::attackCollisionRect() const
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

    const Harm Medusa::avatarCollide(const sf::FloatRect & avatarRect)
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

    void Medusa::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void Medusa::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void Medusa::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

} // namespace platformer