// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-fire-imp.hpp
//
#include "monster/monster-fire-imp.hpp"

#include "avatar/avatar.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "bramblefore/settings.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

#include <filesystem>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    FireImp::FireImp(Context & context, const sf::FloatRect & region)
        : Monster(context, { MonsterType::FireImp, region, 0.875f })
    {}

    const sf::FloatRect FireImp::collisionRect() const
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

    const sf::FloatRect FireImp::attackCollisionRect() const
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

    const Harm FireImp::avatarCollide(const sf::FloatRect & avatarRect)
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

    void FireImp::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void FireImp::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void FireImp::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

} // namespace platformer