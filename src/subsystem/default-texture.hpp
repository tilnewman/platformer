#ifndef DEFAULT_TEXTURE_HPP_INCLUDED
#define DEFAULT_TEXTURE_HPP_INCLUDED
//
// default-texture.hpp
//
#include <SFML/Graphics/Texture.hpp>

#include <memory>

namespace platformer
{

    class DefaultTexture
    {
      public:
        DefaultTexture();

        static DefaultTexture & instance();

        void setup();
        void teardown();

        inline const sf::Texture & get() const { return *m_textureUPtr; }

      private:
        std::unique_ptr<sf::Texture> m_textureUPtr;
    };

} // namespace platformer

#endif // DEFAULT_TEXTURE_HPP_INCLUDED
