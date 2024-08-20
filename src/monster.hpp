#ifndef MONSTER_HPP_INCLUDED
#define MONSTER_HPP_INCLUDED
//
// monster.hpp
//
#include "custom-types.hpp"
#include "harm.hpp"

#include <filesystem>
#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
} // namespace sf

namespace platformer
{

    struct Context;
    struct Settings;

    //

    enum class MonsterAnim : std::size_t
    {
        Attack = 0,
        Death,
        Hurt,
        Idle,
        Walk,
        Count
    };

    inline constexpr std::string_view toString(const MonsterAnim anim)
    {
        // clang-format off
        switch (anim)
        {
            case MonsterAnim::Attack:      { return "attack"; }
            case MonsterAnim::Death:       { return "death";  }
            case MonsterAnim::Hurt:        { return "hurt";   }
            case MonsterAnim::Idle:        { return "idle";   }
            case MonsterAnim::Walk:        { return "walk";   }
            case MonsterAnim::Count: // intentional fallthrough
            default:  { return "error_MonsterAnin_not_found"; }
        }
        // clang-format on
    }

    inline constexpr bool doesAnimLoop(const MonsterAnim anim)
    {
        return ((anim == MonsterAnim::Walk) || (anim == MonsterAnim::Idle));
    }

    //

    struct AttackInfo
    {
        AttackInfo(const Health_t attackDamage = 0, const sf::FloatRect & avatarAttackRect = {})
            : damage(attackDamage)
            , rect(avatarAttackRect)
        {}

        Health_t damage;
        sf::FloatRect rect;
    };

    //

    struct IMonster
    {
        virtual ~IMonster() = default;

        virtual void update(Context & context, const float frameTimeSec)                     = 0;
        virtual void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const = 0;
        virtual void move(const float amount)                                                = 0;
        virtual const Harm avatarCollide(const sf::FloatRect & avatarRect)                   = 0;
        virtual const sf::FloatRect collisionRect() const                                    = 0;
        virtual const sf::FloatRect attackCollisionRect() const                              = 0;
        virtual bool avatarAttack(Context & context, const AttackInfo & attackInfo)          = 0;
    };

    //

    class Monster : public IMonster
    {
      public:
        explicit Monster(
            Context & context,
            const sf::FloatRect & region,
            const std::string & imageDirName,
            const Health_t health);

        virtual ~Monster() override = default;

        // IMonster functions
        void update(Context & context, const float frameTimeSec) override;
        void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const override;
        void move(const float amount) override;
        bool avatarAttack(Context & context, const AttackInfo & attackInfo) override;

      protected:
        virtual bool animate(); // returns true if animation is finished
        virtual float timePerFrameSec(const MonsterAnim anim) const;
        virtual void changeStateBeforeSeeingPlayer(Context & context);
        virtual void changeStateAfterSeeingPlayer(Context & context);
        virtual void handleWalking(Context & context, const float frameTimeSec);

        virtual float walkSpeed() const                     = 0;
        virtual void playAttackSfx(Context & context) const = 0;
        virtual void playHurtSfx(Context & context) const   = 0;
        virtual void playDeathSfx(Context & context) const  = 0;

        void resetAnimation();
        void loadTextures(const Settings & settings);
        void initialSpriteSetup(Context & context);
        void setTexture(sf::Sprite & s, const MonsterAnim a, const std::size_t frame) const;
        const sf::IntRect textureRect(const MonsterAnim a, const std::size_t frame) const;
        std::size_t frameCount(const MonsterAnim anim) const;
        void turnToFacePlayer(Context & context);

      protected:
        std::string m_imageDirName;
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

        static std::vector<sf::Texture> m_textures;
    };

} // namespace platformer

#endif // MONSTER_HPP_INCLUDED
