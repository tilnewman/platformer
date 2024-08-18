#ifndef AVATARTEXTURES_HPP_INCLUDED
#define AVATARTEXTURES_HPP_INCLUDED
//
// avatar-textures.hpp
//
#include <string_view>
#include <vector>

#include <SFML/Graphics/Texture.hpp>

namespace platformer
{
    struct Settings;

    //

    enum class AvatarType : std::size_t
    {
        Assassin = 0,
        BlueKnight,
        Druid,
        Enchantress,
        Ninja,
        RedKnight,
        Rogue,
        Viking,
        Witch,
        Count
    };

    inline constexpr std::string_view toString(const AvatarType type)
    {
        // clang-format off
        switch (type)
        {
            case AvatarType::Assassin:      { return "assassin";    }
            case AvatarType::BlueKnight:    { return "blue-knight"; }
            case AvatarType::Druid:         { return "druid";       }
            case AvatarType::Enchantress:   { return "enchantress"; }
            case AvatarType::Ninja:         { return "ninja";       }
            case AvatarType::RedKnight:     { return "red-knight";  }
            case AvatarType::Rogue:         { return "rogue";       }
            case AvatarType::Viking:        { return "viking";      }
            case AvatarType::Witch:         { return "witch";       }
            case AvatarType::Count:         // intentional fallthrough
            default:                        { return "error_AvatarType_not_found"; }
        }
        // clang-format on
    }

    //

    inline constexpr bool isMale(const AvatarType type)
    {
        // clang-format off
        switch (type)
        {
            case AvatarType::Assassin:      { return true;  }
            case AvatarType::BlueKnight:    { return true;  }
            case AvatarType::Druid:         { return false; }
            case AvatarType::Enchantress:   { return false; }
            case AvatarType::Ninja:         { return true;  }
            case AvatarType::RedKnight:     { return true;  }
            case AvatarType::Rogue:         { return true;  }
            case AvatarType::Viking:        { return true;  }
            case AvatarType::Witch:         { return false; }
            case AvatarType::Count:         // intentional fallthrough
            default:                        { return true;  }
        }
        // clang-format on
    }

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

    inline constexpr std::string_view toString(const AvatarAnim anim)
    {
        // clang-format off
        switch (anim)
        {
            case AvatarAnim::Attack:        { return "attack"; }
            case AvatarAnim::AttackExtra:   { return "attack_extra"; }
            case AvatarAnim::Climb:         { return "climb"; }
            case AvatarAnim::Death:         { return "death"; }
            case AvatarAnim::Hurt:          { return "hurt"; }
            case AvatarAnim::Idle:          { return "idle"; }
            case AvatarAnim::Jump:          { return "jump"; }
            case AvatarAnim::JumpHigh:      { return "jump_high"; }
            case AvatarAnim::Push:          { return "push"; }
            case AvatarAnim::Run:           { return "run"; }
            case AvatarAnim::Walk:          { return "walk"; }
            case AvatarAnim::Count:         // intentional fallthrough
            default:                        { return "error_AvatarAnin_not_found"; }
        }
        // clang-format on
    }

    inline constexpr float timePerFrameSec(const AvatarAnim anim)
    {
        // clang-format off
        switch (anim)
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
            case AvatarAnim::Count:// intentional fallthrough
            default:                        { return 0.0f;  }
        }
        // clang-format on
    }

    inline constexpr bool doesAnimLoop(const AvatarAnim anim)
    {
        if ((anim == AvatarAnim::Climb) || (anim == AvatarAnim::Idle) ||
            (anim == AvatarAnim::Push) || (anim == AvatarAnim::Run) || (anim == AvatarAnim::Walk))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //

    struct AnimTextures
    {
        float time_per_frame_sec{ 0.0f };
        std::vector<sf::Texture> textures{};
    };

    //

    struct AvatarTextures
    {
        sf::Texture icon_texture{};
        sf::Texture default_texture{};
        std::vector<AnimTextures> anim_textures{};
    };

    //

    class AvatarTextureManager
    {
      public:
        AvatarTextureManager();

        void setup(const Settings & settings);

        inline const AnimTextures & get(const AvatarType & type, const AvatarAnim anim) const
        {
            return m_textures.at(static_cast<std::size_t>(type))
                .anim_textures.at(static_cast<std::size_t>(anim));
        }

        inline const sf::Texture & getDefault(const AvatarType & type) const
        {
            return m_textures.at(static_cast<std::size_t>(type)).default_texture;
        }

        inline const sf::Texture & iconTexture(const AvatarType type) const
        {
            return m_textures.at(static_cast<std::size_t>(type)).icon_texture;
        }

      private:
        std::vector<AvatarTextures> m_textures;
    };

} // namespace platformer

#endif // AVATARTEXTURES_HPP_INCLUDED
