#ifndef MONSTER_TEXTURES_HPP_INCLUDED
#define MONSTER_TEXTURES_HPP_INCLUDED
//
// monster-textures.hpp
//
#include "monster/imonster.hpp"

#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace bramblefore
{
    struct Context;
    struct Settings;

    //

    struct MonsterTextures
    {
        std::size_t ref_count{ 0 };
        std::vector<sf::Texture> textures{};
        sf::Texture icon_texture{};
    };

    //

    class MonsterTextureManager
    {
      public:
        MonsterTextureManager();

        static MonsterTextureManager & instance();

        void setup(const Settings & t_settings);
        void teardown();

        void acquire(const Context & t_context, const MonsterType t_type);
        void release(const MonsterType t_type);

        void setTexture(
            sf::Sprite & t_sprite,
            const MonsterType t_type,
            const MonsterAnim t_anim,
            const std::size_t t_frame) const;

        void setIconTexture(sf::Sprite & t_sprite, const MonsterType t_type) const;

        [[nodiscard]] std::size_t frameCount(const MonsterType t_type, const MonsterAnim t_anim) const;

      private:
        [[nodiscard]] const sf::Texture &
            getTexture(const MonsterType t_type, const MonsterAnim t_anim) const;

        [[nodiscard]] sf::IntRect getTextureRect(
            const MonsterType t_type, const MonsterAnim t_anim, const std::size_t t_frame) const;

      private:
        std::vector<MonsterTextures> m_textureSets;
    };

} // namespace bramblefore

#endif // MONSTER_TEXTURES_HPP_INCLUDED
