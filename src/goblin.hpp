#ifndef GOBLIN_HPP_INCLUDED
#define GOBLIN_HPP_INCLUDED
//
// goblin.hpp
//
#include "monsters.hpp"

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

    enum class GoblinAnim : std::size_t
    {
        Attack = 0,
        Death,
        Hurt,
        Idle,
        Walk,
        Count
    };

    inline constexpr std::string_view toString(const GoblinAnim anim)
    {
        // clang-format off
        switch (anim)
        {
            case GoblinAnim::Attack:        { return "attack"; }
            case GoblinAnim::Death:         { return "death";  }
            case GoblinAnim::Hurt:          { return "hurt";   }
            case GoblinAnim::Idle:          { return "idle";   }
            case GoblinAnim::Walk:          { return "walk";   }
            case GoblinAnim::Count:   // intentional fallthrough
            default:    { return "error_GoblinAnin_not_found"; }
        }
        // clang-format on
    }

    inline constexpr float timePerFrameSec(const GoblinAnim anim)
    {
        // clang-format off
        switch (anim)
        {
            case GoblinAnim::Attack:        { return 0.1f;   }
            case GoblinAnim::Death:         { return 0.15f;  }
            case GoblinAnim::Hurt:          { return 0.175f; }
            case GoblinAnim::Idle:          { return 0.15f;  }
            case GoblinAnim::Walk:          { return 0.1f;   }
            case GoblinAnim::Count: // intentional fallthrough
            default:                        { return 0.0f;   }
        }
        // clang-format on
    }

    inline constexpr bool doesAnimLoop(const GoblinAnim anim)
    {
        return ((anim == GoblinAnim::Walk) || (anim == GoblinAnim::Idle));
    }

    //

    class Goblin : public IMonster
    {
      public:
        Goblin(Context & context, const sf::FloatRect & region);
        virtual ~Goblin() override = default;

        void update(Context & context, const float frameTimeSec) override;
        void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const override;
        void move(const float amount) override;

      private:
        bool animate(const float frameTimeSec); // returns true if animation is finished
        void changeState(Context & context);
        const sf::FloatRect collision() const;

        void loadTextures(const Settings & settings);
        std::size_t frameCount(const GoblinAnim anim) const;
        const sf::IntRect textureRect(const GoblinAnim anim, const std::size_t frame) const;
        void setTexture(sf::Sprite & sprite, const GoblinAnim anim, const std::size_t frame) const;

      private:
        sf::FloatRect m_region;
        GoblinAnim m_anim;
        std::size_t m_animFrame;
        sf::Sprite m_sprite;
        float m_elapsedTimeSec;
        bool m_isFacingRight;
        float m_stateElapsedTimeSec;
        float m_stateTimeUntilChangeSec;

        static std::vector<sf::Texture> m_textures;
    };

} // namespace platformer

#endif // GOBLIN_HPP_INCLUDED
