// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-boss-wizard.hpp
//
#include "monster/monster-boss-wizard.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace platformer
{

    BossWizard::BossWizard(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::BossWizard, t_region, 0.75f } }
    {}

    const sf::FloatRect BossWizard::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.25f, 0.35f });

            rect.top += (rect.width * 0.15f);

            if (m_isFacingRight)
            {
                rect.left -= (rect.width * 0.75f);
            }
            else
            {
                rect.left += (rect.width * 0.75f);
            }

            return rect;
        }
    }

    const sf::FloatRect BossWizard::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };

        rect.width *= 1.8f;

        if (m_isFacingRight)
        {
            rect.left += (rect.width * 0.6f);
        }
        else
        {
            rect.left -= (rect.width * 1.0f);
        }

        util::scaleRectInPlace(rect, 1.1f);

        return rect;
    }

    const Harm BossWizard::avatarCollide(const sf::FloatRect & avatarRect)
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

    void BossWizard::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void BossWizard::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void BossWizard::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

    void BossWizard::turnAround()
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