#ifndef MONSTER_HPP_INCLUDED
#define MONSTER_HPP_INCLUDED
//
// monster.hpp
//
#include "monster/imonster.hpp"
#include "monster/monster-spell-anim.hpp"
#include "player/custom-types.hpp"
#include "subsystem/harm.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
    class RenderTarget;
} // namespace sf

namespace bramblefore
{

    struct Context;

    //

    struct MonsterSetupInfo
    {
        MonsterSetupInfo(
            const MonsterType t_monsterType,
            const sf::FloatRect & t_roamRegion,
            const float t_imageHeightRatio,
            const float t_imageScale   = 1.0f,
            const MonsterSpell t_spell = MonsterSpell::Count)
            : type(t_monsterType)
            , region(t_roamRegion)
            , image_height_ratio(t_imageHeightRatio)
            , image_scale(t_imageScale)
            , spell(t_spell)
        {}

        MonsterType type;
        sf::FloatRect region;
        float image_height_ratio;
        float image_scale;
        MonsterSpell spell;
    };

    //

    class Monster : public IMonster
    {
      public:
        Monster(Context & t_context, const MonsterSetupInfo & t_setupInfo);
        virtual ~Monster() override;

        // IMonster functions
        void update(Context & t_context, const float t_frameTimeSec) override;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const override;

        void move(const float t_amount) override;
        bool avatarAttack(Context & t_context, const AttackInfo & t_attackInfo) override;

        Harm avatarCollide(const sf::FloatRect & t_avatarRect) override;

      protected:
        bool animate(); // returns true if animation is finished
        [[nodiscard]] float timePerFrameSec(const MonsterAnim t_anim) const noexcept;
        void handleWalking(Context & t_context, const float t_frameTimeSec);
        void handleDying(Context & t_context);

        virtual void changeStateBeforeSeeingPlayer(Context & t_context);
        virtual void changeStateAfterSeeingPlayer(Context & t_context);
        virtual void turnAround();
        virtual void startAttackAnimation(Context &) {}

        void resetAnimation();

        void initialSpriteSetup(
            Context & t_context, const float t_imageHeightOffsetRatio, const float t_imageScale);

        void turnToFacePlayer(Context & t_context);

      protected:
        MonsterType m_type;
        MonsterSpell m_spell;
        sf::FloatRect m_region;
        MonsterAnim m_anim;
        std::size_t m_animFrame;
        sf::Sprite m_sprite;
        float m_elapsedTimeSec;
        bool m_isFacingRight;
        float m_stateElapsedTimeSec;
        float m_stateTimeUntilChangeSec;
        bool m_hasSpottedPlayer;
        Health_t m_health;
        bool m_isAlive;
        MonsterSpellAnimations m_animations;
    };

} // namespace bramblefore

#endif // MONSTER_HPP_INCLUDED
