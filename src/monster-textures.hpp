#ifndef MONSTER_TEXTURES_HPP_INCLUDED
#define MONSTER_TEXTURES_HPP_INCLUDED
//
// monster-textures.hpp
//
#include "imonster.hpp"

#include <vector>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace platformer
{
    struct Context;
    struct Settings;

    //

    struct MonsterTextures
    {
        std::size_t ref_count{ 0 };
        std::vector<sf::Texture> textures{};
    };

    //

    class MonsterTextureManager
    {
      public:
        MonsterTextureManager();

        static MonsterTextureManager & instance();

        void setup(const Settings & settings);

        void acquire(Context & context, const MonsterType type);
        void release(const MonsterType type);

        void setTexture(
            sf::Sprite & sprite,
            const MonsterType type,
            const MonsterAnim a,
            const std::size_t frame) const;

        std::size_t frameCount(const MonsterType type, const MonsterAnim anim) const;
       
        private:
        const sf::Texture & getTexture(const MonsterType type, const MonsterAnim anim) const;
        sf::IntRect getTextureRect(const MonsterType type, const MonsterAnim anim, const std::size_t frame) const;
        
      private:
        std::vector<MonsterTextures> m_textureSets;
    };

} // namespace platformer

#endif // MONSTER_TEXTURES_HPP_INCLUDED
