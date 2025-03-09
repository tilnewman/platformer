#ifndef TILELAYER_HPP_INCLUDED
#define TILELAYER_HPP_INCLUDED
//
// tile-layer.hpp
//
#include "map/tile-image.hpp"

#include <memory>
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

        virtual void appendVertLayer(
            const Context & t_context,
            const sf::Vector2f & t_mapPositionOffset,
            const sf::Vector2i & t_tileCount,
            const sf::Vector2i & t_tileSize,
            const sf::Vector2f & t_tileSizeOnScreen) = 0;
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

        void appendVertLayer(
            const Context & t_context,
            const sf::Vector2f & t_mapPositionOffset,
            const sf::Vector2i & t_count,
            const sf::Vector2i & t_size,
            const sf::Vector2f & t_sizeOnScreen) override;

      private:
        void populateVisibleVerts(const sf::FloatRect & t_visibleRect);

      private:
        TileImage m_image;
        std::vector<int> m_indexes;
        std::vector<sf::Vertex> m_verts;
        std::vector<sf::Vertex> m_visibleVerts;
    };

} // namespace bramblefore

#endif // TILELAYER_HPP_INCLUDED
