// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "level-file-loader.hpp"

#include "background-images.hpp"
#include "check-macros.hpp"
#include "context.hpp"
#include "level.hpp"
#include "map-textures.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"

#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace platformer
{

    LevelFileLoader::LevelFileLoader()
        : m_pathStr()
    {}

    bool LevelFileLoader::load(Context & context)
    {
        // TODO fix to be more general
        const std::filesystem::path path = (context.settings.media_path / "map/forest-1.json");
        if (!std::filesystem::exists(path))
        {
            return false;
        }

        m_pathStr = path.string();

        Json json;

        {
            std::ifstream iStream(m_pathStr);
            M_CHECK(iStream, "Failed to open level file: \"" << m_pathStr << "\"");
            iStream >> json;
        }

        parseLevelDetails(context, json);
        parseBackgroundImageName(context, json);
        parseObjectTextureGIDs(context, json);

        // everything else in the level file is saved in "layers"
        // which are parsed in order from back to front here, one at a time
        parseLayers(context, json);

        return true;
    }

    void LevelFileLoader::parseLevelDetails(Context & context, Json & json)
    {
        // parse level tile size and counts
        context.level.tiles.count = { json["width"], json["height"] };
        context.level.tiles.size  = { json["tilewidth"], json["tileheight"] };

        context.level.tile_size_texture = sf::Vector2f{ context.level.tiles.size };

        context.level.tile_size_screen =
            (sf::Vector2f{ context.level.tiles.size } * context.settings.tile_scale);

        context.level.tile_size_screen.x = floorf(context.level.tile_size_screen.x);
        context.level.tile_size_screen.y = floorf(context.level.tile_size_screen.y);

        // calc map position offset
        const sf::Vector2f tileCountF{ context.level.tiles.count };
        const sf::Vector2f mapSizeOrig{ context.level.tile_size_screen * tileCountF };

        const float heightOffset{
            (context.layout.wholeRect().top + context.layout.wholeRect().height) - mapSizeOrig.y
        };

        context.level.map_position_offset = { 0.0f, heightOffset };
    }

    void LevelFileLoader::parseObjectTextureGIDs(Context & context, Json & wholeJson)
    {
        for (Json & json : wholeJson["tilesets"])
        {
            const std::string sourceStr{ json["source"] };
            const std::filesystem::path path{ sourceStr };
            const std::string filename{ path.filename().string() };

            const int gid{ json["firstgid"] };

            if (filename == toFilename(TileImage::ForestGround))
            {
                context.map_textures.get(TileImage::ForestGround).gid = gid;
            }
            else if (filename == toFilename(TileImage::ForestTrees))
            {
                context.map_textures.get(TileImage::ForestTrees).gid = gid;
            }
            else
            {
                std::cout << "Warning: While parsing \"" << m_pathStr
                          << "\": Ignored tileset named \"" << filename << "\", with gid=" << gid
                          << "\n";
            }
        }
    }

    void LevelFileLoader::parseBackgroundImageName(Context & context, Json & json)
    {
        std::string backgroundImageName;
        for (Json & propJson : json["properties"])
        {
            const std::string propName = propJson["name"];
            if ("background" == propName)
            {
                backgroundImageName = propJson["value"];
                break;
            }
            else
            {
                std::cout << "Warning: While parsing \"" << m_pathStr
                          << "\": Ignored custom property named \"" << propName << "\"\n";
            }
        }

        if (backgroundImageName.empty())
        {
            std::cout << "Error: While parsing \"" << m_pathStr
                      << "\": This map file is missing the custom int property named "
                         "\"background\", there will be no background on this map.\n";
        }

        context.bg_images.setup(
            context, context.bg_images.infoFactory(context, backgroundImageName));
    }

    void LevelFileLoader::parseLayers(Context & context, Json & jsonWholeFile)
    {
        context.level.tiles.layers.clear();

        for (Json & jsonLayer : jsonWholeFile["layers"])
        {
            const std::string layerName = jsonLayer["name"];

            if (layerName == toString(TileImage::ForestGround))
            {
                parseTileLayer(context, TileImage::ForestGround, jsonLayer);
            }
            else if (layerName == toString(TileImage::ForestTrees))
            {
                parseTileLayer(context, TileImage::ForestTrees, jsonLayer);
            }
            else if (layerName == "collision")
            {
                parseRectLayer(context, jsonLayer, context.level.collisions);
            }
            else if (layerName == "spawn")
            {
                parseSpawnLayer(context, jsonLayer);
            }
            else
            {
                std::cout << "WARNING:  While parsing level file \"" << m_pathStr
                          << "\".  Ignored unknown layer named \"" << layerName << "\".\n";
            }
        }

        M_CHECK(
            !context.level.tiles.layers.empty(),
            "Error Parsing Level File " << m_pathStr << ":  Failed to read any tile image layers.");
    }

    void LevelFileLoader::parseTileLayer(Context & context, const TileImage image, Json & json)
    {
        TileLayer layer;

        layer.image = image;

        const std::vector<int> indexes = json["data"];
        layer.indexes                  = indexes;

        M_CHECK(
            !layer.indexes.empty(),
            "Error Parsing Level File "
                << m_pathStr << ":  Failed to read tileset layer indexes for image " << image
                << ".");

        context.level.tiles.layers.push_back(layer);
    }

    void LevelFileLoader::parseRectLayer(
        Context & context, Json & json, std::vector<sf::FloatRect> & rects)
    {
        rects.clear();

        for (Json & collJson : json["objects"])
        {
            rects.emplace_back(parseAndConvertRect(context, collJson));
        }
    }

    const sf::FloatRect LevelFileLoader::parseAndConvertRect(const Context & context, Json & json)
    {
        sf::IntRect mapRect;
        mapRect.left   = json["x"];
        mapRect.top    = json["y"];
        mapRect.width  = json["width"];
        mapRect.height = json["height"];

        // convert from map to screen coordinates
        sf::FloatRect screenRect{ mapRect };
        screenRect.left *= context.settings.tile_scale;
        screenRect.top *= context.settings.tile_scale;
        screenRect.width *= context.settings.tile_scale;
        screenRect.height *= context.settings.tile_scale;
        //
        screenRect.left += context.level.map_position_offset.x;
        screenRect.top += context.level.map_position_offset.y;

        return screenRect;
    }

    void LevelFileLoader::parseSpawnLayer(Context & context, Json & json)
    {
        context.level.enter_rect = { 0.0f, 0.0f, -1.0f, -1.0f };
        context.level.exit_rect  = { 0.0f, 0.0f, -1.0f, -1.0f };

        for (Json & spawnJson : json["objects"])
        {
            const std::string name   = spawnJson["name"];
            const sf::FloatRect rect = parseAndConvertRect(context, spawnJson);

            if (name == "enter")
            {
                context.level.enter_rect = rect;
            }
            else if (name == "exit")
            {
                context.level.exit_rect = rect;
            }
            else
            {
                std::cout << "WARNING:  While parsing level file \"" << m_pathStr
                          << "\".  Ignored unknown spawn rect named \"" << name << "\".\n";
            }
        }

        M_CHECK(
            (context.level.enter_rect.width > 0.0f),
            "Error Parsing Level File " << m_pathStr << ":  Failed to find enter location.");

        M_CHECK(
            (context.level.exit_rect.width > 0.0f),
            "Error Parsing Level File " << m_pathStr << ":  Failed to find exit location.");
    }

} // namespace platformer
