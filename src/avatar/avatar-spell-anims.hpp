#ifndef AVATAR_SPELL_ANIMS_HPP_INCLUDED
#define AVATAR_SPELL_ANIMS_HPP_INCLUDED
//
// avatar-spell-anims.hpp
//
#include "avatar/avatar-textures.hpp"

#include <filesystem>
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

    struct AvatarSpellAnim
    {
        explicit AvatarSpellAnim(
            const AvatarType t_type,
            const sf::Texture & t_texture,
            const bool t_isFirstAnim,
            const bool t_isMovingRight,
            const sf::Vector2f & t_scale,
            const sf::Vector2f & t_position);

        bool is_alive;
        AvatarType type;
        bool is_first_anim;
        std::size_t frame_index;
        float elapsed_time_sec;
        sf::Sprite sprite;
        bool is_moving_right;
    };

    //

    struct AvatarSpellTextures
    {
        // all spell casters have a normal (first) and extra (second) spell attack
        std::vector<sf::Texture> first{};
        std::vector<sf::Texture> second{};
    };

    //

    class AvatarSpellAnimations
    {
      public:
        AvatarSpellAnimations();

        void setup(const Context & context);

        // add() with no remove() because they remove themselves once finished
        void
            add(const sf::Vector2f & t_pos,
                const AvatarType t_type,
                const bool t_isFirstAttack,
                const bool t_isFacingRight);

        void update(const float t_frameTimeSec);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        void move(const float t_amount);
        inline void clear() noexcept { m_anims.clear(); }

      private:
        void loadTextures(
            std::vector<sf::Texture> & t_textures, const std::filesystem::path & t_path) const;

        [[nodiscard]] const std::vector<sf::Texture> &
            getTextures(const AvatarType t_type, const bool t_isFirstAttack) const;

      private:
        AvatarSpellTextures m_druidTextures;
        AvatarSpellTextures m_enchantressTextures;
        AvatarSpellTextures m_witchTextures;
        std::vector<AvatarSpellAnim> m_anims;
        float m_timePerFrameSec;
        sf::Vector2f m_scale;
    };

} // namespace bramblefore

#endif // AVATAR_SPELL_ANIMS_HPP_INCLUDED
