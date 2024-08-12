#ifndef TILELAYER_HPP_INCLUDED
#define TILELAYER_HPP_INCLUDED
//
// tile-layer.hpp
//
#include "tile-image.hpp"

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

    struct ITileLayer
    {
        virtual ~ITileLayer() = default;

        virtual void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const = 0;
        virtual void move(const Context & context, const float move)                         = 0;
        virtual float findFarthestHorizVert() const                                          = 0;
        virtual void dumpInfo() const                                                        = 0;
        virtual void update(Context & context, const float frameTimeSec)                     = 0;

        virtual void appendVertLayer(
            const Context & context,
            const sf::Vector2f & mapPositionOffset,
            const sf::Vector2i & count,
            const sf::Vector2i & size,
            const sf::Vector2f & sizeOnScreen) = 0;
    };

    //

    class TileLayer : public ITileLayer
    {
      public:
        TileLayer(const TileImage image, const std::vector<int> & indexes);
        virtual ~TileLayer() override = default;

        void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const override;
        void move(const Context & context, const float move) override;
        float findFarthestHorizVert() const override;
        void dumpInfo() const override;
        void update(Context &, const float) override {}

        void appendVertLayer(
            const Context & context,
            const sf::Vector2f & mapPositionOffset,
            const sf::Vector2i & count,
            const sf::Vector2i & size,
            const sf::Vector2f & sizeOnScreen) override;

      private:
        void populateVisibleVerts(const sf::FloatRect & visibleRect);

      private:
        TileImage m_image;
        std::vector<int> m_indexes;
        std::vector<sf::Vertex> m_verts;
        std::vector<sf::Vertex> m_visibleVerts;
    };

} // namespace platformer

#endif // TILELAYER_HPP_INCLUDED
