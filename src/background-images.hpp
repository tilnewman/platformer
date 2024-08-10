#ifndef BACKGROUNDIMAGES_HPP_INCLUDED
#define BACKGROUNDIMAGES_HPP_INCLUDED
//
// background-images.hpp
//
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace platformer
{
    struct Context;

    //

    class BackgroundImages
    {
      public:
        BackgroundImages();

        void setup(const Context & context);
        void draw(sf::RenderTarget & target, sf::RenderStates states) const;

      private:
        sf::Texture m_skyTexture;
        sf::Texture m_cloudsBackTexture;
        sf::Texture m_cloudsFrontTexture;
        sf::Texture m_mountainsTexture;
        sf::Texture m_treesTexture;
        sf::Texture m_mistsTexture;

        sf::Sprite m_skySprite;
        sf::Sprite m_cloudsBackSprite;
        sf::Sprite m_cloudsFrontSprite;
        sf::Sprite m_mountainsSprite;
        sf::Sprite m_treesSprite;
        sf::Sprite m_mistsSprite;
    };

} // namespace platformer

#endif // BACKGROUNDIMAGES_HPP_INCLUDED
