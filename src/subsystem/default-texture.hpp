#ifndef DEFAULT_TEXTURE_HPP_INCLUDED
#define DEFAULT_TEXTURE_HPP_INCLUDED
//
// default-texture.hpp
//
#include <SFML/Graphics/Texture.hpp>

#include <memory>

namespace bramblefore
{

    // when you have to have a texture but an error occurs, use this one instead of crashing
    class DefaultTexture
    {
      public:
        DefaultTexture();

        static DefaultTexture & instance();

        void setup();
        void teardown();

        inline const sf::Texture & get() const { return *m_textureUPtr; }

      private:
        // um...why is there a pointer to this texture?
        std::unique_ptr<sf::Texture> m_textureUPtr;
    };

} // namespace bramblefore

#endif // DEFAULT_TEXTURE_HPP_INCLUDED
