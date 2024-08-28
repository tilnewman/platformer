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
        std::size_t ref_count{ 0 };
        std::vector<sf::Texture> textures{};
    };

    //

    class MonsterSpellTextureManager
    {
      public:
        MonsterSpellTextureManager();

        static MonsterSpellTextureManager & instance();

        void setup(const Settings & t_settings);
        void teardown();

        void acquire(Context & t_context, const MonsterSpell t_spell);
        void release(const MonsterSpell t_spell);

        void
            set(sf::Sprite & t_sprite, const MonsterSpell t_spell, const std::size_t t_frame) const;

        [[nodiscard]] std::size_t frameCount(const MonsterSpell t_spell) const;

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

        void
            add(const sf::Vector2f & t_pos, const MonsterSpell t_spell, const bool t_isFacingRight);

        void update(const float t_frameTimeSec);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        void move(const float t_amount);
        inline void clear() noexcept { m_anims.clear(); }

      private:
        std::vector<MonsterSpellAnim> m_anims;
        float m_timePerFrameSec;
    };

} // namespace platformer

#endif // MONSTER_SPELL_ANIM_HPP_INCLUDED
