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
        void setPosition(const sf::FloatRect & rect);
        inline AvatarAnim getAnim() const { return m_anim; }

      private:
        const sf::FloatRect collisionRect() const;
        void animate(Context & context, const float frameTimeSec);
        void moveMap(Context & context);
        void killIfOutOfBounds(Context & context);
        void preventBacktracking(const Context & context);
        void collisions(Context & context);
        void sideToSideMotion(Context & context, const float frameTimeSec);
        void restartAnim();
        void jumping(Context & context, const float frameTimeSec);

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
