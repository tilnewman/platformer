#ifndef SPELLS_ANIM_HPP_INCLUDED
#define SPELLS_ANIM_HPP_INCLUDED
//
// spells-anim.hpp
//
#include "player/player-info.hpp"

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RenderTarget;
} // namespace sf

namespace bramblefore
{
    struct Context;
    struct Settings;

    //

    struct SpellAnim
    {
        explicit SpellAnim(
            const Spell t_spell,
            const sf::Texture & t_texture,
            const float t_timePerFrameSec,
            const sf::Vector2f & t_scale,
            const sf::Vector2f & t_position);

        bool is_alive;
        Spell spell;
        std::size_t frame_index;
        float elapsed_time_sec;
        float time_per_frame_sec;
        sf::Sprite sprite;
    };

    //

    struct SpellTextures
    {
        sf::Texture icon_texture{};
        std::vector<sf::Texture> textures{};
    };

    //

    class SpellAnimations
    {
      public:
        SpellAnimations();

        void setup(const Context & t_context);
        void add(const sf::Vector2f & t_pos, const Spell t_spell);
        void update(Context & t_context, const float t_frameTimeSec);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        void move(const float t_amount);

        [[nodiscard]] inline const sf::Texture & iconTexture(const Spell spell) const
        {
            return m_textureSets.at(static_cast<std::size_t>(spell)).icon_texture;
        }

        inline void clear() noexcept { m_anims.clear(); }

      private:
        std::vector<SpellTextures> m_textureSets;
        std::vector<SpellAnim> m_anims;
        sf::Vector2f m_scale;
    };

} // namespace bramblefore

#endif // SPELLS_ANIM_HPP_INCLUDED
