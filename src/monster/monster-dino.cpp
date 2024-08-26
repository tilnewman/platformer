// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-dino.hpp
//
#include "monster/monster-dino.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace platformer
{

    Dino::Dino(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::Dino, t_region, 0.8f } }
    {}

    const sf::FloatRect Dino::collisionRect() const
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

    const sf::FloatRect Dino::attackCollisionRect() const
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

    const Harm Dino::avatarCollide(const sf::FloatRect & avatarRect)
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

    void Dino::playAttackSfx(Context & context) const { context.sfx.play("attack-dino"); }

    void Dino::playHurtSfx(Context & context) const { context.sfx.play("hurt-dino"); }

    void Dino::playDeathSfx(Context & context) const { context.sfx.play("death-dino"); }

} // namespace platformer