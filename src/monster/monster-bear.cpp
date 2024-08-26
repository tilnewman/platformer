// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-bear.hpp
//
#include "monster/monster-bear.hpp"

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

    Bear::Bear(Context & context, const sf::FloatRect & region)
        : Monster(context, { MonsterType::Bear, region, 0.6f, 1.2f })
    {}

    const sf::FloatRect Bear::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.6f, 0.4f });
            rect.left -= (rect.width * 0.2f);
            rect.top += (rect.height * 0.4f);

            if (!m_isFacingRight)
            {
                rect.left += (rect.width * 0.3f);
            }

            return rect;
        }
    }

    const sf::FloatRect Bear::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };

        const float shiftHoriz{ rect.width * 0.4f };

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

    const Harm Bear::avatarCollide(const sf::FloatRect & avatarRect)
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

    void Bear::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void Bear::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void Bear::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

    void Bear::turnAround()
    {
        m_sprite.scale(-1.0f, 1.0f);

        const float imageWidthRatio{ 0.8f };
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