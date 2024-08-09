#ifndef AVATAR_HPP_INCLUDED
#define AVATAR_HPP_INCLUDED
//
// avatar.hpp
//
#include <string_view>
#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace platformer
{
    struct Context;
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

    enum class AvatarAnim : std::size_t
    {
        Attack = 0,
        AttackExtra,
        Climb,
        Death,
        Fire,      // optional
        FireExtra, // optional
        Hurt,
        Idle,
        Jump,
        JumpHigh,
        Push,
        Run,
        RunAttack,
        Walk,
        WalkAttack,
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
            case AvatarAnim::Fire:          { return "fire"; }
            case AvatarAnim::FireExtra:     { return "fire_extra"; }
            case AvatarAnim::Hurt:          { return "hurt"; }
            case AvatarAnim::Idle:          { return "idle"; }
            case AvatarAnim::Jump:          { return "jump"; }
            case AvatarAnim::JumpHigh:      { return "jump_high"; }
            case AvatarAnim::Push:          { return "push"; }
            case AvatarAnim::Run:           { return "run"; }
            case AvatarAnim::RunAttack:     { return "run_attack"; }
            case AvatarAnim::Walk:          { return "walk"; }
            case AvatarAnim::WalkAttack:    { return "walk_attack"; }
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
            case AvatarAnim::Attack:        { return 0.1f;  }
            case AvatarAnim::AttackExtra:   { return 0.1f;  }
            case AvatarAnim::Climb:         { return 0.2f;  }
            case AvatarAnim::Death:         { return 0.1f;  }
            case AvatarAnim::Fire:          { return 0.1f;  }
            case AvatarAnim::FireExtra:     { return 0.1f;  }
            case AvatarAnim::Hurt:          { return 0.2f;  }
            case AvatarAnim::Idle:          { return 0.1f;  }
            case AvatarAnim::Jump:          { return 0.15f; }
            case AvatarAnim::JumpHigh:      { return 0.15f; }
            case AvatarAnim::Push:          { return 0.2f;  }
            case AvatarAnim::Run:           { return 0.1f;  }
            case AvatarAnim::RunAttack:     { return 0.1f;  }
            case AvatarAnim::Walk:          { return 0.12f; }
            case AvatarAnim::WalkAttack:    { return 0.12f; }
            case AvatarAnim::Count:         // intentional fallthrough
            default:                        { return 0.0f;  }
        }
        // clang-format on
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

      private:
        std::vector<AvatarTextures> m_textures;
    };

    //

    class Avatar
    {
      public:
        Avatar();

        void setup(const Context & context, const AvatarType & type);
        void update(Context & context, const float frameTimeSec);
        void draw(sf::RenderTarget & target, sf::RenderStates states);

      private:
        sf::Sprite m_sprite;
        AvatarType m_type;
        AvatarAnim m_anim;
        float m_elapsedTimeSec;
        std::size_t m_animIndex;
    };

} // namespace platformer

#endif // AVATAR_HPP_INCLUDED
