#ifndef GOBLIN_TEXTURES_HPP_INCLUDED
#define GOBLIN_TEXTURES_HPP_INCLUDED
//
// goblin-textures.hpp
//
#include <cstddef>
#include <string_view>
#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace platformer
{

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

    class GoblinTextureManager
    {
      public:
        GoblinTextureManager();

        void setup(const Settings & settings);
        std::size_t frameCount(const GoblinAnim anim) const;
        const sf::IntRect rect(const GoblinAnim anim, const std::size_t frame) const;
        void set(sf::Sprite & sprite, const GoblinAnim anim, const std::size_t frame) const;

      private:
        std::vector<sf::Texture> m_textures;
    };

} // namespace platformer

#endif // GOBLIN_TEXTURES_HPP_INCLUDED
