#ifndef AVATAR_SPELL_ANIMS_HPP_INCLUDED
#define AVATAR_SPELL_ANIMS_HPP_INCLUDED
//
// avatar-spell-anims.hpp
//
#include "avatar-textures.hpp"

#include <vector>
#include <filesystem>

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

    struct AvatarSpellAnim
    {
        bool is_alive{ true };
        AvatarType type{ AvatarType::Count }; // anything works here
        bool is_first_anim{ true };
        std::size_t frame_index{ 0 };
        float elapsed_time_sec{ 0.0f };
        sf::Sprite sprite{};
        bool is_moving_right{true};
    };

    //

    struct AvatarSpellTextures
    {
        std::vector<sf::Texture> first{};
        std::vector<sf::Texture> second{};
    };

    //

    class AvatarSpellAnimations
    {
      public:
        AvatarSpellAnimations();

        void setup(const Settings & settings);

        void
            add(const sf::Vector2f & pos,
                const AvatarType type,
                const bool isFirstAttack,
                const bool isFacingRight);

        void update(const float frameTimeSec);
        void draw(sf::RenderTarget & t, sf::RenderStates s) const;
        void move(const float amount);
        inline void clear() { m_anims.clear(); }

      private:
        void loadTextures(
            std::vector<sf::Texture> & textures, const std::filesystem::path & path) const;

        const std::vector<sf::Texture> &
            getTextures(const AvatarType type, const bool isFirstAttack) const;

      private:
        AvatarSpellTextures m_druidTextures;
        AvatarSpellTextures m_enchantressTextures;
        AvatarSpellTextures m_witchTextures;
        std::vector<AvatarSpellAnim> m_anims;
        float m_timePerFrameSec;
        sf::Vector2f m_scale;
    };

} // namespace platformer

#endif // AVATAR_SPELL_ANIMS_HPP_INCLUDED
