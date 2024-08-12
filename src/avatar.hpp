#ifndef AVATAR_HPP_INCLUDED
#define AVATAR_HPP_INCLUDED
//
// avatar.hpp
//
#include "avatar-textures.hpp"

#include <string_view>
#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace platformer
{
    struct Context;
    struct Settings;

    //

    class Avatar
    {
      public:
        Avatar();

        void setup(const Context & context, const AvatarType & type);
        void update(Context & context, const float frameTimeSec);
        void draw(sf::RenderTarget & target, sf::RenderStates states);
        inline void setPosition(const sf::Vector2f & pos) { m_sprite.setPosition(pos); }
        inline AvatarAnim getAnim() const { return m_anim; }
        void cycleType();
        void cycleAnim();

      private:
        const sf::FloatRect collisionRect() const;

      private:
        sf::Sprite m_sprite;
        AvatarType m_type;
        AvatarAnim m_anim;
        float m_elapsedTimeSec;
        std::size_t m_animIndex;
        sf::Vector2f m_velocity;
        bool m_hasLanded;
        bool m_isFacingRight;
    };

} // namespace platformer

#endif // AVATAR_HPP_INCLUDED
