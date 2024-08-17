#ifndef PLAYER_INFO_HPP_INCLUDED
#define PLAYER_INFO_HPP_INCLUDED
//
// player-info.hpp
//
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
} // namespace sf

namespace platformer
{
    struct Context;

    //

    class PlayerInfoDisplay
    {
      public:
        PlayerInfoDisplay();

        void setup(const Context & context);
        void draw(sf::RenderTarget & target, sf::RenderStates states) const;

      private:
        sf::Texture m_halfFrameTexture;
        sf::Sprite m_halfFrameSprite;

        sf::Texture m_fullFrameTexture;
        sf::Sprite m_fullFrameSprite;

        sf::Sprite m_avatarIconSprite;

        std::vector<sf::Vertex> m_bgFadeVerts;

        sf::Texture m_barFrameTexture;
        sf::Sprite m_healthBarFrameSprite;
        sf::Sprite m_manaBarFrameSprite;
        
        sf::Texture m_barFillLeftTexture;
        sf::Texture m_barFillMiddleTexture;
        sf::Texture m_barFillRightTexture;

        sf::FloatRect m_healthBarRect;
        sf::Sprite m_healthBarLeftSprite;
        sf::Sprite m_healthBarMiddleSprite;
        sf::Sprite m_healthBarRightSprite;

        sf::FloatRect m_manaBarRect;
        sf::Sprite m_manaBarLeftSprite;
        sf::Sprite m_manaBarMiddleSprite;
        sf::Sprite m_manaBarRightSprite;

        sf::Color m_healthColor;
        sf::Color m_manaColor;
    };

} // namespace platformer

#endif // PLAYER_INFO_HPP_INCLUDED
