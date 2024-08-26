// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-lizard.hpp
//
#include "monster/monster-lizard.hpp"

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

    Lizard::Lizard(Context & context, const sf::FloatRect & region)
        : Monster(context, { MonsterType::Lizard, region, 0.88f })
    {}

    const sf::FloatRect Lizard::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.2f, 0.2f });
            return rect;
        }
    }

    const sf::FloatRect Lizard::attackCollisionRect() const
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

    const Harm Lizard::avatarCollide(const sf::FloatRect & avatarRect)
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

    void Lizard::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void Lizard::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void Lizard::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

} // namespace platformer