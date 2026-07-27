#ifndef MOSQUITO_TEXTURES_HPP_INCLUDED
#define MOSQUITO_TEXTURES_HPP_INCLUDED
//
// mosquito-textures.hpp
//
#include "monster/imonster.hpp"

#include <vector>

#include <SFML/Graphics/Texture.hpp>

namespace bramblefore
{
    struct Context;

    //

    class MosquitoTextureManager
    {
      private:
        MosquitoTextureManager();

      public:
        static MosquitoTextureManager & instance();

        void acquire(const Context & t_context);
        void release();

        const std::vector<std::vector<sf::Texture>> & textures() const { return m_textures; }
      
    private:
        std::size_t m_refCount;
        std::vector<std::vector<sf::Texture>> m_textures;
    };

} // namespace bramblefore

#endif // MOSQUITO_TEXTURES_HPP_INCLUDED
