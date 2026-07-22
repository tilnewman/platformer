#ifndef BLOOD_SPLAT_HPP_INCLUDED
#define BLOOD_SPLAT_HPP_INCLUDED
//
// blood-splat.hpp
//
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sf
{
    class RenderTarget;
} // namespace sf

namespace bramblefore
{

    struct Context;

    //

    struct BloodSplatAnim
    {
        BloodSplatAnim(
            const Context & t_context,
            const sf::Texture & t_texture,
            const sf::IntRect & t_textureRect,
            const sf::Vector2f & t_position);

        bool is_alive;
        sf::Sprite sprite;
        float elapsed_sec;
        float duration_sec;
    };

    //

    class BloodSplatManager
    {
      public:
        BloodSplatManager();

        void setup(const Context & t_context);
        void update(const float t_elapsedTimeSec);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        void add(const Context & t_context, const sf::Vector2f & t_position);
        void move(const sf::Vector2f & t_move);
        constexpr void clear() noexcept { m_anims.clear(); }

        private:
        [[nodiscard]] const sf::IntRect getRandomTextureRect(const Context & t_context);

      private:
        sf::Texture m_texture;
        std::vector<BloodSplatAnim> m_anims;
    };

} // namespace bramblefore

#endif // BLOOD_SPLAT_HPP_INCLUDED
