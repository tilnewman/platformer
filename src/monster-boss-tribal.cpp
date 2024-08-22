// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-boss-tribalt.hpp
//
#include "monster-boss-tribal.hpp"

#include "avatar.hpp"
#include "context.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "sound-player.hpp"

#include <filesystem>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    BossTribal::BossTribal(Context & context, const sf::FloatRect & region)
        : Monster(context, { MonsterType::BossTribal, region, 0.7f })
    {}

    const sf::FloatRect BossTribal::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.25f, 0.35f });

            rect.top += (rect.width * 0.35f);

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

    const sf::FloatRect BossTribal::attackCollisionRect() const
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

    const Harm BossTribal::avatarCollide(const sf::FloatRect & avatarRect)
    {
        Harm harm;

        if ((MonsterAnim::Attack == m_anim) && avatarRect.intersects(attackCollisionRect()))
        {
            harm.damage = 25;
            harm.rect   = collisionRect();
            harm.sfx    = "hit-wood";
        }

        return harm;
    }

    void BossTribal::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void BossTribal::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void BossTribal::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

    void BossTribal::turnAround()
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