#ifndef TILE_IMAGE_HPP_INCLUDED
#define TILE_IMAGE_HPP_INCLUDED
//
// tile-image.hpp
//
#include <ostream>
#include <string>
#include <string_view>

namespace bramblefore
{

    enum class TileImage : std::size_t
    {
        CastleGround = 0,
        CastleObject,
        Dungeon1Ground,
        CaveGround,
        CaveObject,
        ForestGround,
        ForestTrees,
        MountainsGround,
        MountainsObject,
        Count
    };

    inline constexpr std::string_view toString(const TileImage image)
    {
        // clang-format off
        switch (image)
        {
            case TileImage::CastleGround:    { return "castle-ground";      }
            case TileImage::CastleObject:    { return "castle-object";      }
            case TileImage::Dungeon1Ground:  { return "dungeon1-ground";    }
            case TileImage::CaveGround:      { return "cave-ground";        }
            case TileImage::CaveObject:      { return "cave-object";        }
            case TileImage::ForestGround:    { return "forest-ground";      }
            case TileImage::ForestTrees:     { return "forest-trees";       }
            case TileImage::MountainsGround: { return "mountains-ground";   } 
            case TileImage::MountainsObject: { return "mountains-object";   }
            case TileImage::Count:           [[fallthrough]];
            default:                    { return "Error_TileImage_unknown"; }
        }
        // clang-format on
    }

    inline std::ostream & operator<<(std::ostream & os, const TileImage image)
    {
        os << toString(image);
        return os;
    }

    inline std::string toMapFilename(const TileImage image)
    {
        std::string filename{ toString(image) };
        filename += ".tsx";
        return filename;
    }

    inline std::string toImageFilename(const TileImage image)
    {
        std::string filename{ toString(image) };
        filename += ".png";
        return filename;
    }

} // namespace bramblefore

#endif // TILE_IMAGE_HPP_INCLUDED
