#ifndef TILESET_HPP_INCLUDED
#define TILESET_HPP_INCLUDED
//
// tileset.hpp
//
#include <cassert>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include <SFML/Graphics/Drawable.hpp>
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

    struct ITileLayer
    {
        virtual ~ITileLayer() = default;

        virtual void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const = 0;

        virtual TileImage image() const                  = 0;
        virtual std::vector<int> & indexes()             = 0;
        virtual std::vector<sf::Vertex> & verts()        = 0;
        virtual std::vector<sf::Vertex> & visibleVerts() = 0;

        virtual void moveVerts(const float move)                             = 0;
        virtual float findFarthestHorizVert() const                          = 0;
        virtual void dumpInfo() const                                        = 0;
        virtual void populateVisibleVerts(const sf::FloatRect & visibleRect) = 0;
    };

    //

    class TileLayer : public ITileLayer
    {
      public:
        TileLayer(const TileImage image, const std::vector<int> & indexes);
        virtual ~TileLayer() override = default;

        void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const override;

        TileImage image() const override { return m_image; }
        std::vector<int> & indexes() override { return m_indexes; }
        std::vector<sf::Vertex> & verts() override { return m_verts; }
        std::vector<sf::Vertex> & visibleVerts() override { return m_visibleVerts; }

        void moveVerts(const float move) override;
        float findFarthestHorizVert() const override;
        void dumpInfo() const override;
        void populateVisibleVerts(const sf::FloatRect & visibleRect) override;

      private:
        TileImage m_image;
        std::vector<int> m_indexes;
        std::vector<sf::Vertex> m_verts;
        std::vector<sf::Vertex> m_visibleVerts;
    };

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
