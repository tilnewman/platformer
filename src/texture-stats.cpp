// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// texture-stats.cpp
//
#include "texture-stats.hpp"

#include <iostream>
#include <sstream>

#include <SFML/Graphics/Texture.hpp>

namespace platformer
{

    TextureStats::TextureStats()
        : m_count(0)
        , m_byteCount(0)
    {}

    void TextureStats::process(const sf::Texture & texture)
    {
        ++m_count;
        m_byteCount += static_cast<std::size_t>(texture.getSize().x * texture.getSize().y * 4u);
    }

    void TextureStats::dumpInfo()
    {
        std::ostringstream ss;
        ss.imbue(std::locale("")); // this is only to put commas in the big numbers
        ss << m_count << " textures loaded for a total of " << m_byteCount << "bytes.";
        std::cout << ss.str() << '\n';
    }

} // namespace platformer
