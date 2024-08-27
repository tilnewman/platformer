#ifndef AVATAR_TEXTURES_HPP_INCLUDED
#define AVATAR_TEXTURES_HPP_INCLUDED
//
// avatar-textures.hpp
//
#include "player/player-info.hpp"

#include <string_view>
#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace platformer
{

    struct Context;
    struct Settings;

    //

    enum class AvatarAnim : std::size_t
    {
        Attack = 0,
        AttackExtra,
        Climb,
        Death,
        Hurt,
        Idle,
        Jump,
        JumpHigh,
        Push,
        Run,
        Walk,
        Count
    };

    inline constexpr std::string_view toString(const AvatarAnim t_anim)
    {
        // clang-format off
        switch (t_anim)
        {
            case AvatarAnim::Attack:        { return "attack";       }
            case AvatarAnim::AttackExtra:   { return "attack_extra"; }
            case AvatarAnim::Climb:         { return "climb";        }
            case AvatarAnim::Death:         { return "death";        }
            case AvatarAnim::Hurt:          { return "hurt";         }
            case AvatarAnim::Idle:          { return "idle";         }
            case AvatarAnim::Jump:          { return "jump";         }
            case AvatarAnim::JumpHigh:      { return "jump_high";    }
            case AvatarAnim::Push:          { return "push";         }
            case AvatarAnim::Run:           { return "run";          }
            case AvatarAnim::Walk:          { return "walk";         }
            case AvatarAnim::Count:         [[fallthrough]];
            default:          { return "error_AvatarAnin_not_found"; }
        }
        // clang-format on
    }

    inline constexpr float avatarTimePerFrameSec(const AvatarAnim t_anim)
    {
        // clang-format off
        switch (t_anim)
        {
            case AvatarAnim::Attack:        { return 0.08f; }
            case AvatarAnim::AttackExtra:   { return 0.08f; }
            case AvatarAnim::Climb:         { return 0.2f;  }
            case AvatarAnim::Death:         { return 0.1f;  }
            case AvatarAnim::Hurt:          { return 0.1f;  }
            case AvatarAnim::Idle:          { return 0.1f;  }
            case AvatarAnim::Jump:          { return 0.1f;  }
            case AvatarAnim::JumpHigh:      { return 0.1f;  }
            case AvatarAnim::Push:          { return 0.2f;  }
            case AvatarAnim::Run:           { return 0.1f;  }
            case AvatarAnim::Walk:          { return 0.1f;  }
            case AvatarAnim::Count:         [[fallthrough]];
            default:                        { return 0.0f;  }
        }
        // clang-format on
    }

    inline constexpr bool doesAnimLoop(const AvatarAnim t_anim)
    {
        return (
            (t_anim == AvatarAnim::Climb) || (t_anim == AvatarAnim::Idle) ||
            (t_anim == AvatarAnim::Push) || (t_anim == AvatarAnim::Run) ||
            (t_anim == AvatarAnim::Walk));
    }

    //

    struct AnimTextures
    {
        std::vector<sf::Texture> textures{};
    };

    //

    struct AvatarTextureSet
    {
        sf::Texture icon{};
        sf::Texture defalt{};
        std::size_t ref_count{ 0 };
        std::vector<AnimTextures> anims{};
    };

    //

    class AvatarTextureManager
    {
      public:
        AvatarTextureManager();

        static AvatarTextureManager & instance();

        void setup(const Settings & t_settings);

        void acquire(const Context & t_context, const AvatarType t_type);
        void release(const AvatarType t_type);

        void
            set(sf::Sprite & t_sprite,
                const AvatarType t_type,
                const AvatarAnim t_anim,
                const std::size_t t_frame) const;

        [[nodiscard]] std::size_t
            frameCount(const AvatarType t_type, const AvatarAnim t_anim) const;

        [[nodiscard]] const sf::Texture & getDefault(const AvatarType t_type) const;
        [[nodiscard]] const sf::Texture & getIcon(const AvatarType t_type) const;

      private:
        std::vector<AvatarTextureSet> m_textureSets;
    };

} // namespace platformer

#endif // AVATAR_TEXTURES_HPP_INCLUDED
