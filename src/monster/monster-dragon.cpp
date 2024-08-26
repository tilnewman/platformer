// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-dragon.hpp
//
#include "monster/monster-dragon.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace platformer
{

    Dragon::Dragon(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::Dragon, t_region, 0.75f } }
    {}

    const sf::FloatRect Dragon::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.4f, 0.25f });
            rect.top += (rect.height * 0.5f);

            if (m_isFacingRight)
            {
                rect.left += (rect.width * 0.2f);
            }
            else
            {
                rect.left -= (rect.width * 0.2f);
            }

            return rect;
        }
    }

    const sf::FloatRect Dragon::attackCollisionRect() const
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

    const Harm Dragon::avatarCollide(const sf::FloatRect & avatarRect)
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

    void Dragon::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void Dragon::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void Dragon::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

    void Dragon::startAttackAnimation(Context &)
    {
        const sf::FloatRect collRect{ collisionRect() };

        sf::Vector2f pos;
        pos.y = collRect.top;

        if (m_isFacingRight)
        {
            pos.x = util::right(collRect);
        }
        else
        {
            pos.x = collRect.left;
        }

        m_animations.add(pos, MonsterSpell::DragonFire, m_isFacingRight);
    }

} // namespace platformer