#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED
//
// level.hpp
//
#include "map/tile-layer.hpp"
#include "monster/monster-manager.hpp"

#include <optional>
#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{
    struct Context;
    struct TileTexture;
    class ScreenLayout;

    //

    class Level
    {
      public:
        Level();

        [[nodiscard]] inline const std::string name() const noexcept { return m_name; }

        [[nodiscard]] inline const sf::Vector2f mapScreenPosOffset() const noexcept
        {
            return m_mapScreenPosOffset;
        }

        [[nodiscard]] inline const sf::Vector2f tileSizeScreen() const noexcept
        {
            return m_tileSizeScreen;
        }

        [[nodiscard]] inline const sf::Vector2f tileSizeTexture() const noexcept
        {
            return m_tileSizeTexture;
        }

        [[nodiscard]] inline const std::vector<sf::FloatRect> & collisions() noexcept
        {
            return m_collisions;
        }

        inline void collisions(const std::vector<sf::FloatRect> & t_collisions)
        {
            m_collisions = t_collisions;
        }

        [[nodiscard]] inline const std::vector<sf::FloatRect> & killCollisions() noexcept
        {
            return m_killCollisions;
        }

        inline void killCollisions(const std::vector<sf::FloatRect> & t_killCollisions)
        {
            m_killCollisions = t_killCollisions;
        }

        [[nodiscard]] inline const std::vector<sf::FloatRect> & layerCollisions() noexcept
        {
            return m_layerCollisions;
        }

        inline void appendLayerCollision(const sf::FloatRect & t_layerCollision)
        {
            m_layerCollisions.push_back(t_layerCollision);
        }

        [[nodiscard]] inline const std::vector<sf::FloatRect> & ladders() noexcept
        {
            return m_ladders;
        }

        inline void ladders(const std::vector<sf::FloatRect> & t_ladders) noexcept
        {
            m_ladders = t_ladders;
        }

        [[nodiscard]] inline const sf::FloatRect enterRect() const noexcept { return m_enterRect; }
        [[nodiscard]] inline const sf::FloatRect exitRect() const noexcept { return m_exitRect; }
        [[nodiscard]] inline const sf::Vector2i tileCount() const noexcept { return m_tileCount; }
        [[nodiscard]] inline const sf::Vector2i tileSize() const noexcept { return m_tileSize; }

        [[nodiscard]] inline MonsterManager & monsters() { return m_monsters; }

        void appendTileLayer(std::unique_ptr<ITileLayer> t_tileLayerUPtr)
        {
            m_tileLayers.emplace_back(std::move(t_tileLayerUPtr));
        }

        void setupDetails(
            const sf::Vector2i & t_tileCount,
            const sf::Vector2i & t_tileSize,
            const sf::Vector2f & t_tileSizeScreen,
            const sf::Vector2f & t_mapScreenPosOffset);

        inline void enterRect(const sf::FloatRect & t_enterRect) { m_enterRect = t_enterRect; }
        inline void exitRect(const sf::FloatRect & t_exitRect) { m_exitRect = t_exitRect; }

        void load(const Context & t_context, const std::string & t_filename);
        void reset();

        // returns true if the map actually moved
        bool move(const Context & t_context, const sf::Vector2f & t_move);

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const;

        void update(const Context & t_context, const float t_frameTimeSec);

        const std::optional<sf::FloatRect>
            ladderCollisionRect(const sf::FloatRect & avatarRect) const;

      private:
        void appendVertLayers(const Context & t_context);
        void dumpInfo(const std::string & t_filename) const;

      private:
        std::string m_name;

        sf::Vector2f m_mapScreenPosOffset;
        sf::Vector2f m_tileSizeScreen;
        sf::Vector2f m_tileSizeTexture;

        std::vector<sf::FloatRect> m_collisions;
        std::vector<sf::FloatRect> m_killCollisions;
        std::vector<sf::FloatRect> m_layerCollisions;
        std::vector<sf::FloatRect> m_ladders;

        sf::FloatRect m_enterRect;
        sf::FloatRect m_exitRect;

        sf::Vector2i m_tileCount;
        sf::Vector2i m_tileSize;

        std::vector<std::unique_ptr<ITileLayer>> m_tileLayers;

        MonsterManager m_monsters;

        float m_farthestHorizTraveled;
        float m_farthestHorizMapPixel;
        float m_traveledVert;
    };

} // namespace bramblefore

#endif // LEVEL_HPP_INCLUDED
