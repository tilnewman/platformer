// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// goblin.hpp
//
#include "goblin.hpp"

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

    Goblin::Goblin(Context & context, const sf::FloatRect & region)
        : Monster(context, region, "goblin")
    {}

    const sf::FloatRect Goblin::collisionRect() const
    {
        sf::FloatRect rect{ m_sprite.getGlobalBounds() };
        util::scaleRectInPlace(rect, 0.35f);
        return rect;
    }

    const sf::FloatRect Goblin::attackCollisionRect() const
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

    const Harm Goblin::avatarCollide(const sf::FloatRect & avatarRect)
    {
        Harm harm;

        if ((MonsterAnim::Attack == m_anim) && avatarRect.intersects(attackCollisionRect()))
        {
            harm.damage = 10;
            harm.rect   = collisionRect();
            harm.sfx    = "wood-hit";
        }

        return harm;
    }

    //const std::string Goblin::imageDirName() const { return "goblin"; }

    void Goblin::playAttackSfx(Context & context) const { context.sfx.play("swipe"); }

} // namespace platformer