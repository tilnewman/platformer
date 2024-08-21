// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-snake.hpp
//
#include "monster-snake.hpp"

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

    Snake::Snake(Context & context, const sf::FloatRect & region)
        : Monster(context, { region, "snake", 30, 0.86f })
    {}

    const sf::FloatRect Snake::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.3f, 0.25f });
            
            if (m_isFacingRight)
            {
                rect.left -= (rect.width * 0.3f);
            }
            else
            {
                rect.left += (rect.width * 0.3f);
            }
            return rect;
        }
    }

    const sf::FloatRect Snake::attackCollisionRect() const
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

    const Harm Snake::avatarCollide(const sf::FloatRect & avatarRect)
    {
        Harm harm;

        if ((MonsterAnim::Attack == m_anim) && avatarRect.intersects(attackCollisionRect()))
        {
            harm.damage = 16;
            harm.rect   = collisionRect();
            harm.sfx    = "hit-wood";
        }

        return harm;
    }

    void Snake::playAttackSfx(Context & context) const { context.sfx.play("attack-dino"); }

    void Snake::playHurtSfx(Context & context) const { context.sfx.play("hurt-dino"); }

    void Snake::playDeathSfx(Context & context) const { context.sfx.play("death-dino"); }

    void Snake::turnAround()
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