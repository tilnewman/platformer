#ifndef SPELLS_ANIM_HPP_INCLUDED
#define SPELLS_ANIM_HPP_INCLUDED
//
// spells-anim.hpp
//
#include "player-info.hpp"

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

    struct SpellAnim
    {
        bool is_alive{ true };
        Spell spell{ Spell::Count }; // anything works here
        std::size_t frame_index{ 0 };
        float elapsed_time_sec{ 0.0f };
        float time_per_frame_sec{ 0.0f };
        sf::Sprite sprite{};
    };

    //

    struct SpellTextures
    {
        sf::Texture icon_texture;
        std::vector<sf::Texture> textures;
    };

    //

    class SpellAnimations
    {
      public:
        SpellAnimations();

        void setup(const Settings & settings);
        void add(const sf::Vector2f & pos, const Spell spell);
        void update(Context & context, const float frameTimeSec);
        void draw(sf::RenderTarget & target, sf::RenderStates states) const;
        void move(const float amount);

        inline const sf::Texture & iconTexture(const Spell spell) const
        {
            return m_textureSets.at(static_cast<std::size_t>(spell)).icon_texture;
        }

        inline void clear() { m_anims.clear(); }

      private:
        std::vector<SpellTextures> m_textureSets;
        std::vector<SpellAnim> m_anims;
        sf::Vector2f m_scale;
    };

} // namespace platformer

#endif // SPELLS_ANIM_HPP_INCLUDED
