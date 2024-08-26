#ifndef MONSTER_HPP_INCLUDED
#define MONSTER_HPP_INCLUDED
//
// monster.hpp
//
#include "custom-types.hpp"
#include "harm.hpp"
#include "imonster.hpp"
#include "monster-spell-anim.hpp"

#include <filesystem>
#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
} // namespace sf

namespace platformer
{

    struct Context;

    //

    struct MonsterSetupInfo
    {
        MonsterSetupInfo(
            const MonsterType t_monsterType,
            const sf::FloatRect & t_roamRegion,
            const float t_imageHeightRatio,
            const float t_imageScale = 1.0f)
            : type(t_monsterType)
            , region(t_roamRegion)
            , image_height_ratio(t_imageHeightRatio)
            , image_scale(t_imageScale)
        {}

        MonsterType type;
        sf::FloatRect region;
        float image_height_ratio;
        float image_scale;
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

      protected:
        virtual bool animate(); // returns true if animation is finished
        [[nodiscard]] virtual float timePerFrameSec(const MonsterAnim anim) const noexcept;
        virtual void changeStateBeforeSeeingPlayer(Context & context);
        virtual void changeStateAfterSeeingPlayer(Context & context);
        virtual void handleWalking(Context & context, const float frameTimeSec);
        virtual void turnAround();
        virtual void startAttackAnimation(Context &) {}

        [[nodiscard]] virtual float walkSpeed() const       = 0;
        virtual void playAttackSfx(Context & context) const = 0;
        virtual void playHurtSfx(Context & context) const   = 0;
        virtual void playDeathSfx(Context & context) const  = 0;

        void resetAnimation();

        void initialSpriteSetup(
            Context & context, const float imageHeightOffsetRatio, const float imageScale);

        void turnToFacePlayer(Context & context);

      protected:
        MonsterType m_type;
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

} // namespace platformer

#endif // MONSTER_HPP_INCLUDED
