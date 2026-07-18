#ifndef TILE_LAYER_HPP_INCLUDED
#define TILE_LAYER_HPP_INCLUDED
//
// tile-layer.hpp
//
#include "map/tile-image.hpp"

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RenderTarget;
} // namespace sf

namespace bramblefore
{

    struct Context;

    //

    struct ITileLayer
    {
        virtual ~ITileLayer() = default;

        virtual void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const = 0;
        virtual void move(const Context & t_context, const float t_move)                     = 0;
        virtual void dumpInfo() const                                                        = 0;
        virtual void update(Context & t_context, const float t_frameTimeSec)                 = 0;

        virtual void appendVerts(
            const Context & t_context,
            const sf::Vector2f & t_mapOnScreenPosOffset,
            const sf::Vector2i & t_mapTileCount,
            const sf::Vector2i & t_tileSize,
            const sf::Vector2f & t_tileSizeOnScreen) = 0;
    };

    //

    struct IndexedTile
    {
        IndexedTile()
            : position{}
            , texture_rect{}
        {}

        IndexedTile(const sf::Vector2i & t_position, const sf::IntRect & t_textureRect)
            : position{ t_position }
            , texture_rect{ t_textureRect }
        {}

        sf::Vector2i position;
        sf::IntRect texture_rect;
    };

    //

    class TileLayer : public ITileLayer
    {
      public:
        TileLayer(
            const Context & t_context, const TileImage t_image, const std::vector<int> & t_indexes);

        virtual ~TileLayer() override;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const override;

        void move(const Context & t_context, const float t_move) override;
        void dumpInfo() const override;
        void update(Context &, const float) override {}

        void appendVerts(
            const Context & t_context,
            const sf::Vector2f & t_mapOnScreenPosOffset,
            const sf::Vector2i & t_mapTileCount,
            const sf::Vector2i & t_tileSize,
            const sf::Vector2f & t_sizeOnScreen) override;

      private:
        void appendVertsOriginal(
            const Context & t_context,
            const sf::Vector2f & t_mapOnScreenPosOffset,
            const sf::Vector2i & t_mapTileCount,
            const sf::Vector2i & t_tileSize,
            const sf::Vector2f & t_sizeOnScreen);

        void appendVertsOptimized(
            const Context & t_context,
            const sf::Vector2f & t_mapOnScreenPosOffset,
            const sf::Vector2f & t_sizeOnScreen);

        void populateVisibleVerts(const sf::FloatRect & t_visibleRect);

        void setupOptimizedTileIndexes(
            const sf::Vector2i & t_mapTileCount, const sf::Vector2i & t_tileSize);

      private:
        TileImage m_image;
        sf::Vector2i m_imageTileCounts;
        std::vector<int> m_indexes;
        std::vector<IndexedTile> m_indexTiles;
        std::vector<sf::Vertex> m_verts;
        std::vector<sf::Vertex> m_visibleVerts;
    };

} // namespace bramblefore

#endif // TILE_LAYER_HPP_INCLUDED
