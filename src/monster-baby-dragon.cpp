// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-baby-dragon.hpp
//
#include "monster-baby-dragon.hpp"

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

    BabyDragon::BabyDragon(Context & context, const sf::FloatRect & region)
        : Monster(context, { region, "baby-dragon", 34, 0.85f })
    {}

    const sf::FloatRect BabyDragon::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.4f, 0.25f });
            //rect.top += (rect.height * 0.4f);
            return rect;
        }
    }

    const sf::FloatRect BabyDragon::attackCollisionRect() const
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

    const Harm BabyDragon::avatarCollide(const sf::FloatRect & avatarRect)
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

    void BabyDragon::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void BabyDragon::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void BabyDragon::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

} // namespace platformer