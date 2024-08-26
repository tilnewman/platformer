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

namespace platformer
{

    class TextureStats
    {
      public:
        static TextureStats & instance()
        {
            static TextureStats signleton;
            return signleton;
        }

        void process(const sf::Texture & texture);
        void dumpInfo();

      private:
        TextureStats();

      private:
        std::size_t m_count;
        std::size_t m_byteCount;
    };

} // namespace platformer

#endif // TEXTURE_STATS_HPP_INCLUDED
