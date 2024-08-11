#ifndef TILESET_HPP_INCLUDED
#define TILESET_HPP_INCLUDED
//
// tileset.hpp
//
#include <cassert>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

namespace platformer
{

    enum class TileImage
    {
        CastleGround,
        CastleObject,
        Dungeon1Ground,
        Dungeon2Ground,
        Dungeon2Object,
        ForestGround,
        ForestTrees,
        MountainsGround,
        MountainsObject
    };

    inline constexpr std::string_view toString(const TileImage image)
    {
        // clang-format off
        switch (image)
        {
            case TileImage::CastleGround:   { return "castle-ground";           }
            case TileImage::CastleObject:   { return "castle-object";           }
            case TileImage::Dungeon1Ground: { return "dungeon1-ground";         }
            case TileImage::Dungeon2Ground: { return "dungeon2-ground";         }
            case TileImage::Dungeon2Object: { return "dungeon2-object";         }
            case TileImage::ForestGround:   { return "forest-ground";           }
            case TileImage::ForestTrees:    { return "forest-trees";            }
            case TileImage::MountainsGround:{ return "mountains-ground";        } 
            case TileImage::MountainsObject:{ return "mountains-object";        }
            default:                        { return "error_TileImage_unknown"; }
        }
        // clang-format on
    }

    inline std::ostream & operator<<(std::ostream & os, const TileImage image)
    {
        os << toString(image);
        return os;
    }

    inline const std::string toFilename(const TileImage image)
    {
        std::string filename{ toString(image) };
        filename += ".tsx";
        return filename;
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
