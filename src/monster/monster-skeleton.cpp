// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-skeleton.hpp
//
#include "monster/monster-skeleton.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace platformer
{

    Skeleton::Skeleton(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::Skeleton, t_region, 0.75f } }
    {}

    const sf::FloatRect Skeleton::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.35f, 0.45f });

            if (m_isFacingRight)
            {
                rect.left -= (rect.width * 0.2f);
            }
            else
            {
                rect.left += (rect.width * 0.2f);
            }

            return rect;
        }
    }

    const sf::FloatRect Skeleton::attackCollisionRect() const
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

    const Harm Skeleton::avatarCollide(const sf::FloatRect & avatarRect)
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

    void Skeleton::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void Skeleton::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void Skeleton::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

    void Skeleton::turnAround()
    {
        m_sprite.scale(-1.0f, 1.0f);

        const float imageWidthRatio{ 0.6f };
        if (m_isFacingRight)
        {
            m_sprite.move(-(m_sprite.getGlobalBounds().width * (1.0f - imageWidthRatio)), 0.0f);
        }
        else
        {
            m_sprite.move((m_sprite.getGlobalBounds().width * (1.0f - imageWidthRatio)), 0.0f);
        }

        m_isFacingRight = !m_isFacingRight;
    }

} // namespace platformer