#ifndef MONSTER_SPELL_ANIM_HPP_INCLUDED
#define MONSTER_SPELL_ANIM_HPP_INCLUDED
//
// monster-spell-anim.hpp
//
#include <string_view>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace platformer
{
    struct Context;
    struct Settings;

    //

    enum class MonsterSpell : std::size_t
    {
        BossBlade = 0,
        BossFire,
        BossLightning,
        DragonFire,
        Count
    };

    inline constexpr std::string_view toString(const MonsterSpell t_spell)
    {
        // clang-format off
        switch (t_spell)
        {
            case MonsterSpell::BossBlade:     { return "boss-blade";      }
            case MonsterSpell::BossFire:      { return "boss-fire";       }
            case MonsterSpell::BossLightning: { return "boss-lightning";  }
            case MonsterSpell::DragonFire:    { return "dragon-fire";     }
            case MonsterSpell::Count:         [[fallthrough]];
            default:               { return "Error_unknown_MonsterSpell"; }
        }
        // clang-format on
    }

    //

    struct MonsterSpellTextures
    {
        std::vector<sf::Texture> textures{};
    };

    //

    class MonsterSpellTextureManager
    {
      public:
        MonsterSpellTextureManager();

        void setup(const Settings & t_settings);

        void
            set(sf::Sprite & t_sprite, const MonsterSpell t_spell, const std::size_t t_frame) const;

        [[nodiscard]] inline std::size_t getFrameCount(const MonsterSpell spell) const
        {
            return m_textureSets.at(static_cast<std::size_t>(spell)).textures.size();
        }

      private:
        std::vector<MonsterSpellTextures> m_textureSets;
    };

    //

    struct MonsterSpellAnim
    {
        bool is_alive{ true };
        MonsterSpell spell{ MonsterSpell::Count }; // anything works here
        std::size_t frame_index{ 0 };
        float elapsed_time_sec{ 0.0f };
        bool is_moving_right{ true };
        sf::Sprite sprite{};
    };

    //

    class MonsterSpellAnimations
    {
      public:
        MonsterSpellAnimations();

        void setup(const Settings & settings);
        void add(const sf::Vector2f & pos, const MonsterSpell spell, const bool isFacingRight);
        void update(const float frameTimeSec);
        void draw(sf::RenderTarget & targt, sf::RenderStates states) const;
        void move(const float amount);
        inline void clear() { m_anims.clear(); }

      private:
        std::vector<MonsterSpellAnim> m_anims;
        float m_timePerFrameSec;
        static MonsterSpellTextureManager m_textureManager;
    };

} // namespace platformer

#endif // MONSTER_SPELL_ANIM_HPP_INCLUDED
