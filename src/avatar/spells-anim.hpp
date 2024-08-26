#ifndef SPELLS_ANIM_HPP_INCLUDED
#define SPELLS_ANIM_HPP_INCLUDED
//
// spells-anim.hpp
//
#include "player/player-info.hpp"

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

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

    struct SpellAnim
    {
        bool is_alive{ true };
        Spell spell{ Spell::Comet }; // anything works here
        std::size_t frame_index{ 0 };
        float elapsed_time_sec{ 0.0f };
        float time_per_frame_sec{ 0.0f };
        sf::Sprite sprite{};
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

        void setup(const Settings & t_settings);
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

} // namespace platformer

#endif // SPELLS_ANIM_HPP_INCLUDED
