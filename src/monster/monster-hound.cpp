// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// monster-hound.hpp
//
#include "monster/monster-hound.hpp"

#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

namespace platformer
{

    Hound::Hound(Context & t_context, const sf::FloatRect & t_region)
        : Monster{ t_context, { MonsterType::Hound, t_region, 0.755f, 0.7f } }
    {}

    sf::FloatRect Hound::collisionRect() const
    {
        if (MonsterAnim::Death == m_anim)
        {
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        }
        else
        {
            sf::FloatRect rect{ m_sprite.getGlobalBounds() };
            util::scaleRectInPlace(rect, { 0.4f, 0.25f });
            rect.top += (rect.height * 0.4f);
            return rect;
        }
    }

    sf::FloatRect Hound::attackCollisionRect() const
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

    void Hound::playAttackSfx(Context & context) const { context.sfx.play("attack-ent"); }

    void Hound::playHurtSfx(Context & context) const { context.sfx.play("hurt-ent"); }

    void Hound::playDeathSfx(Context & context) const { context.sfx.play("death-ent"); }

} // namespace platformer