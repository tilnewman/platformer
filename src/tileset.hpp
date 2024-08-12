#ifndef TILESET_HPP_INCLUDED
#define TILESET_HPP_INCLUDED
//
// tileset.hpp
//
#include "tile-image.hpp"
#include "tile-layer.hpp"

#include <memory>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
} // namespace sf

namespace platformer
{

    struct Context;

    //

    struct TileSet
    {
        TileSet();

        void reset();

        sf::Vector2i count;
        sf::Vector2i size;
        std::vector<std::unique_ptr<ITileLayer>> layers;
    };

} // namespace platformer

#endif // TILESET_HPP_INCLUDED
