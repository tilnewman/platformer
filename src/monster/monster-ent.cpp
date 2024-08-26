// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-ent.hpp
//
#include "monster/monster-ent.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace platformer
{

    Ent::Ent(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::Ent, t_region, 0.8f } }
    {}

    sf::FloatRect Ent::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.25f, 0.3f });
            return rect;
        }
    }

    sf::FloatRect Ent::attackCollisionRect() const
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

    void Ent::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void Ent::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void Ent::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

} // namespace platformer