#ifndef ATTACK_ANIMATIONS_HPP_INCLUDED
#define ATTACK_ANIMATIONS_HPP_INCLUDED
//
// attack-animations.hpp
//
#include "monster/imonster.hpp"

#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace bramblefore
{
    struct Context;

    //

    struct AttackAnimTexturePack
    {
        std::size_t frameCount() const;
        const sf::IntRect textureRect(const std::size_t t_frame) const;

        std::size_t ref_count{ 0 };
        sf::Texture texture{};
    };

    //

    class AttackAnimTextureManager
    {
      private:
        AttackAnimTextureManager();

      public:
        static AttackAnimTextureManager & instance();

        void setup();
        void teardown();

        void acquire(const Context & t_context, const MonsterType t_type);
        void release(const MonsterType t_type);

        const std::vector<AttackAnimTexturePack> & texturePacks() const { return m_texturePacks; }

      private:
        std::vector<AttackAnimTexturePack> m_texturePacks;
    };

    //

    struct AttackAnimPack
    {
        AttackAnimPack(
            const MonsterType t_monster,
            const sf::Texture & t_texture,
            const sf::IntRect & t_textureRect,
            const float t_scale,
            const bool t_isFacingRight,
            const sf::Vector2f & t_position);

        bool is_alive;
        bool is_facing_right;
        float elapsed_time_sec;
        std::size_t frame_index;
        MonsterType monster;
        sf::Sprite sprite;
    };

    //

    class AttackAnimManager
    {
      public:
        AttackAnimManager();

        void update(const Context & t_context, const float t_elapsedTimeSec);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        void move(const sf::Vector2f & t_move);

        void
            add(const Context & t_context,
                const MonsterType t_type,
                const sf::Vector2f & t_position,
                const bool t_isFacingRight);

        constexpr void clear() noexcept { m_animPacks.clear(); }

      private:
        std::vector<AttackAnimPack> m_animPacks;
    };

} // namespace bramblefore

#endif // ATTACK_ANIMATIONS_HPP_INCLUDED
