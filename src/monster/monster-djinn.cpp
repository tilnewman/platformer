// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-djinn.hpp
//
#include "monster/monster-djinn.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace platformer
{

    Djinn::Djinn(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::Djinn, t_region, 0.65f } }
    {}

    const sf::FloatRect Djinn::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.3f, 0.4f });

            if (m_isFacingRight)
            {
                rect.left -= (rect.width * 0.35f);
            }
            else
            {
                rect.left += (rect.width * 0.35f);
            }

            return rect;
        }
    }

    const sf::FloatRect Djinn::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };

        const float shiftHoriz{ rect.width * 0.8f };

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

    const Harm Djinn::avatarCollide(const sf::FloatRect & avatarRect)
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

    void Djinn::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void Djinn::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void Djinn::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

    void Djinn::turnAround()
    {
        m_sprite.scale(-1.0f, 1.0f);

        const float imageWidthRatio{ 0.85f };
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