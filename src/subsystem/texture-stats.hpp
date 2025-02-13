#ifndef TEXTURE_STATS_HPP_INCLUDED
#define TEXTURE_STATS_HPP_INCLUDED
//
// tile-layer.hpp
//
#include <cstddef>

namespace sf
{
    class Texture;
}

namespace bramblefore
{

    // Texture loading is performed many times during the initial setup,
    // so this class cannot be passed around in Context and must be a singleton.
    class TextureStats
    {
      public:
        static TextureStats & instance();

        void process(const sf::Texture & texture);
        void dumpInfo();

      private:
        TextureStats();

      private:
        std::size_t m_count;
        std::size_t m_byteCount;
    };

} // namespace bramblefore

#endif // TEXTURE_STATS_HPP_INCLUDED
