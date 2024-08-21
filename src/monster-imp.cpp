// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-imp.hpp
//
#include "monster-imp.hpp"

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

    Imp::Imp(Context & context, const sf::FloatRect & region)
        : Monster(context, { region, "imp", 20 , 0.875f})
    {}

    const sf::FloatRect Imp::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.15f, 0.2f });
            return rect;
        }
    }

    const sf::FloatRect Imp::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };

        const float shiftHoriz{ rect.width * 1.0f };

        if (m_isFacingRight)
        {
            rect.left += shiftHoriz;
        }
        else
        {
            rect.left -= shiftHoriz;
        }

        util::scaleRectInPlace(rect, { 2.0f, 1.1f });

        return rect;
    }

    const Harm Imp::avatarCollide(const sf::FloatRect & avatarRect)
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

    void Imp::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void Imp::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void Imp::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

} // namespace platformer