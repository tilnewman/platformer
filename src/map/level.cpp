// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// level.cpp
//
#include "map/level.hpp"

#include "avatar/avatar.hpp"
#include "avatar/spells-anim.hpp"
#include "map/accents.hpp"
#include "map/level-file-loader.hpp"
#include "map/pickups.hpp"
#include "subsystem/background-images.hpp"
#include "subsystem/blood-splat.hpp"
#include "subsystem/context.hpp"
#include "subsystem/floating-text.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/util.hpp"

#include <iostream>

namespace bramblefore
{

    Level::Level()
        : m_name{}
        , m_mapScreenPosOffset{}
        , m_tileSizeScreen{}
        , m_tileSizeTexture{}
        , m_collisions{}
        , m_killCollisions{}
        , m_layerCollisions{}
        , m_ladders{}
        , m_enterRect{}
        , m_exitRect{}
        , m_tileCount{}
        , m_tileSize{}
        , m_tileLayers{}
        , m_monsters{}
        , m_farthestHorizTraveled{ 0.0f }
        , m_farthestHorizMapPixel{ 0.0f }
        , m_traveledVert{ 0.0f }
    {
        // harmless guesses based on what I know is in map files
        m_collisions.reserve(1024);
        m_killCollisions.reserve(32);
        m_layerCollisions.reserve(32);
        m_ladders.reserve(16);
        m_tileLayers.reserve(32);
    }

    void Level::reset()
    {
        m_tileLayers.clear();
        m_collisions.clear();
        m_killCollisions.clear();
        m_layerCollisions.clear();
        m_ladders.clear();
        m_monsters.clear();
        m_farthestHorizTraveled = 0.0f;
        m_farthestHorizMapPixel = 0.0f;
        m_traveledVert          = 0.0f;
    }

    void Level::load(const Context & t_context, const std::string & t_filename)
    {
        reset();
        t_context.level_loader.load(t_context, t_filename);
        appendVertLayers(t_context);
        m_farthestHorizMapPixel = m_exitRect.position.x;
        // dumpInfo(t_filename);

        t_context.avatar.resetBeforePlacingInNewLevel();
        t_context.avatar.setToSpawnPosition(m_enterRect);
    }

    void Level::setupDetails(
        const sf::Vector2i & t_tileCount,
        const sf::Vector2i & t_tileSize,
        const sf::Vector2f & t_tileSizeScreen,
        const sf::Vector2f & t_mapScreenPosOffset)
    {
        m_tileCount          = t_tileCount;
        m_tileSize           = t_tileSize;
        m_tileSizeTexture    = sf::Vector2f{ t_tileSize };
        m_tileSizeScreen     = t_tileSizeScreen;
        m_mapScreenPosOffset = t_mapScreenPosOffset;
    }

    bool Level::move(const Context & t_context, const sf::Vector2f & t_moveOrig)
    {
        sf::Vector2f move{ 0.0f, 0.0f };

        // the level can progress to the right but not left
        m_farthestHorizTraveled += util::abs(t_moveOrig.x);
        if (m_farthestHorizTraveled < (m_farthestHorizMapPixel - t_context.layout.wholeSize().x))
        {
            move.x = t_moveOrig.x;
        }

        if (t_moveOrig.y > 0.0f)
        {
            m_traveledVert += t_moveOrig.y;
            move.y = t_moveOrig.y;
        }
        else if ((t_moveOrig.y < 0.0f) && (m_traveledVert > 0.0f))
        {
            m_traveledVert += t_moveOrig.y;
            move.y = t_moveOrig.y;
        }

        if ((move.x < 0.0f) || (move.x > 0.0f) || (move.y < 0.0f) || (move.y > 0.0f))
        {
            m_enterRect.position += move;
            m_exitRect.position += move;

            for (sf::FloatRect & rect : m_collisions)
            {
                rect.position += move;
            }

            for (sf::FloatRect & rect : m_killCollisions)
            {
                rect.position += move;
            }

            for (sf::FloatRect & rect : m_layerCollisions)
            {
                rect.position += move;
            }

            for (sf::FloatRect & rect : m_ladders)
            {
                rect.position += move;
            }

            for (auto & layerUPtr : m_tileLayers)
            {
                layerUPtr->move(t_context, move);
            }

            m_monsters.move(move);
            t_context.accent.move(move);
            t_context.pickup.move(move);
            t_context.bg_image.move(move.x); // the map doesn't move up or down
            t_context.spell.move(move);
            t_context.float_text.move(move);
            t_context.blood_splat.move(move);

            return true;
        }
        else
        {
            return false;
        }
    }

    void Level::appendVertLayers(const Context & t_context)
    {
        for (auto & layerUPtr : m_tileLayers)
        {
            layerUPtr->appendVerts(t_context, m_mapScreenPosOffset, m_tileSizeScreen);
        }
    }

    void Level::dumpInfo(const std::string & t_filename) const
    {
        std::clog << t_filename << " Map Graphics Layer Info:\n";

        for (const auto & layerUPtr : m_tileLayers)
        {
            layerUPtr->dumpInfo();
        }

        std::clog << '\n';
    }

    void Level::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (auto & layerUPtr : m_tileLayers)
        {
            layerUPtr->draw(t_context, t_target, t_states);
        }

        m_monsters.draw(t_context, t_target, t_states);
    }

    void Level::update(const Context & t_context, const float t_frameTimeSec)
    {
        for (auto & layerUPtr : m_tileLayers)
        {
            layerUPtr->update(t_context, t_frameTimeSec);
        }

        m_monsters.update(t_context, t_frameTimeSec);
    }

    const std::optional<sf::FloatRect>
        Level::ladderCollisionRect(const sf::FloatRect & avatarRect) const
    {
        for (const sf::FloatRect & ladderRect : m_ladders)
        {
            if (avatarRect.findIntersection(ladderRect))
            {
                return { ladderRect };
            }
        }

        return {};
    }

} // namespace bramblefore
