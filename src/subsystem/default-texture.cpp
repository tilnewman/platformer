// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// default-texture.hpp
//
#include "subsystem/default-texture.hpp"

#include <SFML/Graphics/Image.hpp>

namespace bramblefore
{

    DefaultTexture::DefaultTexture()
        : m_textureUPtr(std::make_unique<sf::Texture>())
    {}

    DefaultTexture & DefaultTexture::instance()
    {
        static DefaultTexture defaultTexture;
        return defaultTexture;
    }

    void DefaultTexture::setup()
    {
        // SFML uses an all white texture for error cases so we use a different color
        sf::Image image;
        image.create(64, 64, sf::Color::Magenta);
        m_textureUPtr->loadFromImage(image);
    }

    void DefaultTexture::teardown() { m_textureUPtr.reset(); }

} // namespace bramblefore