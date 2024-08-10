#ifndef TILESET_HPP_INCLUDED
#define TILESET_HPP_INCLUDED
//
// tileset.hpp
//
#include <cassert>
#include <ostream>
#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

namespace platformer
{

    enum class TileImage
    {
        ForestGround = 0,
        ForestTrees
    };

    inline constexpr std::string_view toString(const TileImage image)
    {
        // clang-format off
        switch (image)
        {
            case TileImage::ForestGround:   { return "ground";   }
            case TileImage::ForestTrees:    { return "object-1"; }
            default:                        { return "error_TileImage_unknown"; }
        }
        // clang-format on
    }

    //

    struct TileLayer
    {
        TileLayer()
            : image(TileImage::ForestGround) // any works here
            , indexes()
            , verts()
            , visibleVerts()
        {
            indexes.reserve(10'000);
            verts.reserve(10'000);
            visibleVerts.reserve(10'000);
        }

        TileImage image;
        std::vector<int> indexes;
        std::vector<sf::Vertex> verts;
        std::vector<sf::Vertex> visibleVerts;
    };

    //

    struct TileSet
    {
        TileSet()
            : count(0, 0)
            , size(0, 0)
            , layers()
        {
            layers.reserve(16);
        }

        void reset()
        {
            layers.clear();
            count = { 0, 0 };
            size  = { 0, 0 };
        }

        sf::Vector2i count;
        sf::Vector2i size;
        std::vector<TileLayer> layers;
    };

} // namespace platformer

#endif // TILESET_HPP_INCLUDED
