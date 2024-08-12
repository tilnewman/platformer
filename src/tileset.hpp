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

    class TileSet
    {
      public:
        TileSet();

        void reset();

        inline const sf::Vector2i tileCount() const { return m_tileCount; }
        inline const sf::Vector2i tileSize() const { return m_tileSize; }

        void setTileCountAndSize(const sf::Vector2i tileCount, const sf::Vector2i tileSize);
        void appendTileLayer(std::unique_ptr<ITileLayer> uptr);
        void move(const Context & context, const float move);
        float findFarthestHorizMapPixel();
        void dumpInfo() const;
        void update(Context & context, const float frameTimeSec);

        void appendVertLayers(
            const Context & context,
            const sf::Vector2f & mapPosOffset,
            const sf::Vector2f & tileScreenSize);

        void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const;

      private:
        sf::Vector2i m_tileCount;
        sf::Vector2i m_tileSize;
        std::vector<std::unique_ptr<ITileLayer>> m_layers;
    };

} // namespace platformer

#endif // TILESET_HPP_INCLUDED
