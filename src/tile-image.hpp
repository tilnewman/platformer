#ifndef TILEIMAGE_HPP_INCLUDED
#define TILEIMAGE_HPP_INCLUDED
//
// tile-image.hpp
//
#include <ostream>
#include <string>
#include <string_view>

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

} // namespace platformer

#endif // TILEIMAGE_HPP_INCLUDED
