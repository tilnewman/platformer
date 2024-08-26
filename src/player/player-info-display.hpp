#ifndef PLAYER_INFO_DISPLAY_HPP_INCLUDED
#define PLAYER_INFO_DISPLAY_HPP_INCLUDED
//
// player-info-display.hpp
//
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
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

        void setup(const Context & t_context);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        void setHealthBar(const float t_ratio);
        void setManaBar(const float t_ratio);
        void setCoinCount(const int t_count);
        void setStarCount(const int t_count);

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

        float m_barFillMax;

        bool m_willDrawHealthBarRight;
        bool m_willDrawHealthBarLeft;
        bool m_willDrawManaBarRight;
        bool m_willDrawManaBarLeft;

        sf::Texture m_coinTexture;
        sf::Sprite m_coinSprite;
        sf::Text m_coinText;

        sf::Texture m_starBrownTexture;
        sf::Sprite m_starBrown1Sprite;
        sf::Sprite m_starBrown2Sprite;
        sf::Sprite m_starBrown3Sprite;
        sf::Sprite m_starBrown4Sprite;
        sf::Sprite m_starBrown5Sprite;

        sf::Texture m_starYellowTexture;
        sf::Sprite m_starYellow1Sprite;
        sf::Sprite m_starYellow2Sprite;
        sf::Sprite m_starYellow3Sprite;
        sf::Sprite m_starYellow4Sprite;
        sf::Sprite m_starYellow5Sprite;
    };

} // namespace platformer

#endif // PLAYER_INFO_DISPLAY_HPP_INCLUDED
