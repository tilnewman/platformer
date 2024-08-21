// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "level-file-loader.hpp"

#include "accents.hpp"
#include "acid-anim-layer.hpp"
#include "background-images.hpp"
#include "check-macros.hpp"
#include "context.hpp"
#include "level.hpp"
#include "lightning-anim-layer.hpp"
#include "map-textures.hpp"
#include "monster-big-knight.hpp"
#include "monster-dino.hpp"
#include "monster-dragon.hpp"
#include "monster-ent.hpp"
#include "monster-fire-imp.hpp"
#include "monster-fire-knight.hpp"
#include "monster-ghost.hpp"
#include "monster-goblin.hpp"
#include "monster-hound.hpp"
#include "monster-imp.hpp"
#include "monster-little-knight.hpp"
#include "monster-salamander.hpp"
#include "monster-skeleton.hpp"
#include "monster-spider.hpp"
#include "monster-vampire.hpp"
#include "pickups.hpp"
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
        const std::filesystem::path path = (context.settings.media_path / "map/dungeon1-1.json");
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
        context.level.tile_count = { json["width"], json["height"] };
        context.level.tile_size  = { json["tilewidth"], json["tileheight"] };

        context.level.tile_size_texture = sf::Vector2f{ context.level.tile_size };

        context.level.tile_size_screen =
            (sf::Vector2f{ context.level.tile_size } * context.settings.tile_scale);

        context.level.tile_size_screen.x = floorf(context.level.tile_size_screen.x);
        context.level.tile_size_screen.y = floorf(context.level.tile_size_screen.y);

        // calc map position offset
        const sf::Vector2f tileCountF{ context.level.tile_count };
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

            if (filename == toFilename(TileImage::CastleGround))
            {
                context.map_texture.get(TileImage::CastleGround).gid = gid;
            }
            else if (filename == toFilename(TileImage::CastleObject))
            {
                context.map_texture.get(TileImage::CastleObject).gid = gid;
            }
            else if (filename == toFilename(TileImage::Dungeon1Ground))
            {
                context.map_texture.get(TileImage::Dungeon1Ground).gid = gid;
            }
            else if (filename == toFilename(TileImage::Dungeon2Ground))
            {
                context.map_texture.get(TileImage::Dungeon2Ground).gid = gid;
            }
            else if (filename == toFilename(TileImage::Dungeon2Object))
            {
                context.map_texture.get(TileImage::Dungeon2Object).gid = gid;
            }
            else if (filename == toFilename(TileImage::ForestGround))
            {
                context.map_texture.get(TileImage::ForestGround).gid = gid;
            }
            else if (filename == toFilename(TileImage::ForestTrees))
            {
                context.map_texture.get(TileImage::ForestTrees).gid = gid;
            }
            else if (filename == toFilename(TileImage::MountainsGround))
            {
                context.map_texture.get(TileImage::MountainsGround).gid = gid;
            }
            else if (filename == toFilename(TileImage::MountainsObject))
            {
                context.map_texture.get(TileImage::MountainsObject).gid = gid;
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

        context.bg_image.setup(context, backgroundImageName);
    }

    void LevelFileLoader::parseLayers(Context & context, Json & jsonWholeFile)
    {
        for (Json & jsonLayer : jsonWholeFile["layers"])
        {
            const std::string layerName = jsonLayer["name"];

            if (layerName == toString(TileImage::CastleGround))
            {
                parseTileLayer(context, TileImage::CastleGround, jsonLayer);
            }
            else if (layerName == toString(TileImage::CastleObject))
            {
                parseTileLayer(context, TileImage::CastleObject, jsonLayer);
            }
            else if (layerName == toString(TileImage::Dungeon1Ground))
            {
                parseTileLayer(context, TileImage::Dungeon1Ground, jsonLayer);
            }
            else if (layerName == toString(TileImage::Dungeon2Ground))
            {
                parseTileLayer(context, TileImage::Dungeon2Ground, jsonLayer);
            }
            else if (layerName == toString(TileImage::Dungeon2Object))
            {
                parseTileLayer(context, TileImage::Dungeon2Object, jsonLayer);
            }
            else if (layerName == toString(TileImage::ForestGround))
            {
                parseTileLayer(context, TileImage::ForestGround, jsonLayer);
            }
            else if (layerName == toString(TileImage::ForestTrees))
            {
                parseTileLayer(context, TileImage::ForestTrees, jsonLayer);
            }
            else if (layerName == toString(TileImage::MountainsGround))
            {
                parseTileLayer(context, TileImage::MountainsGround, jsonLayer);
            }
            else if (layerName == toString(TileImage::MountainsObject))
            {
                parseTileLayer(context, TileImage::MountainsObject, jsonLayer);
            }
            else if (layerName == "collision")
            {
                parseRectLayer(context, jsonLayer, context.level.collisions);
            }
            else if (layerName == "spawn")
            {
                parseSpawnLayer(context, jsonLayer);
            }
            else if (layerName == "pickup-anim")
            {
                parsePickupAnimLayer(context, jsonLayer);
            }
            else if (layerName == "accent-anim")
            {
                parseAccentAnimLayer(context, jsonLayer);
            }
            else if (layerName == "acid-anim")
            {
                parseAcidAnimLayer(context, jsonLayer);
            }
            else if (layerName == "lightning-anim")
            {
                parseLightningAnimLayer(context, jsonLayer);
            }
            else if (layerName == "monster")
            {
                parseMonsterLayer(context, jsonLayer);
            }
            else
            {
                std::cout << "WARNING:  While parsing level file \"" << m_pathStr
                          << "\".  Ignored unknown layer named \"" << layerName << "\".\n";
            }
        }
    }

    void LevelFileLoader::parseTileLayer(Context & context, const TileImage image, Json & json)
    {
        const std::vector<int> indexes = json["data"];

        M_CHECK(
            !indexes.empty(),
            "Error Parsing Level File " << m_pathStr << ":  tile layer for image " << image
                                        << " was empty.");

        context.level.tile_layers.push_back(std::make_unique<TileLayer>(image, indexes));
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

    void LevelFileLoader::parsePickupAnimLayer(Context & context, Json & json)
    {
        for (Json & pickupJson : json["objects"])
        {
            const std::string name   = pickupJson["name"];
            const sf::FloatRect rect = parseAndConvertRect(context, pickupJson);
            context.pickup.add(context, rect, name);
        }
    }

    void LevelFileLoader::parseAccentAnimLayer(Context & context, Json & json)
    {
        for (Json & accentJson : json["objects"])
        {
            const std::string name   = accentJson["name"];
            const sf::FloatRect rect = parseAndConvertRect(context, accentJson);
            context.accent.add(context, rect, name);
        }
    }

    void LevelFileLoader::parseAcidAnimLayer(Context & context, Json & json)
    {
        std::vector<sf::FloatRect> rects;
        rects.reserve(256);

        for (Json & accentJson : json["objects"])
        {
            rects.push_back(parseAndConvertRect(context, accentJson));
        }

        context.level.tile_layers.push_back(std::make_unique<AcidAnimationLayer>(context, rects));
    }

    void LevelFileLoader::parseLightningAnimLayer(Context & context, Json & json)
    {
        std::vector<sf::FloatRect> rects;
        rects.reserve(256);

        for (Json & lightningJson : json["objects"])
        {
            rects.push_back(parseAndConvertRect(context, lightningJson));
        }

        context.level.tile_layers.push_back(
            std::make_unique<LightningAnimationLayer>(context, rects));
    }

    void LevelFileLoader::parseMonsterLayer(Context & context, Json & json)
    {
        for (Json & monsterJson : json["objects"])
        {
            const std::string name   = monsterJson["name"];
            const sf::FloatRect rect = parseAndConvertRect(context, monsterJson);

            if (name == "goblin")
            {
                context.level.monsters.add(std::make_unique<Goblin>(context, rect));
            }
            else if (name == "dino")
            {
                context.level.monsters.add(std::make_unique<Dino>(context, rect));
            }
            else if (name == "spider")
            {
                context.level.monsters.add(std::make_unique<Spider>(context, rect));
            }
            else if (name == "ent")
            {
                context.level.monsters.add(std::make_unique<Ent>(context, rect));
            }
            else if (name == "big-knight")
            {
                context.level.monsters.add(std::make_unique<BigKnight>(context, rect));
            }
            else if (name == "little-knight")
            {
                context.level.monsters.add(std::make_unique<LittleKnight>(context, rect));
            }
            else if (name == "dragon")
            {
                context.level.monsters.add(std::make_unique<Dragon>(context, rect));
            }
            else if (name == "ghost")
            {
                context.level.monsters.add(std::make_unique<Ghost>(context, rect));
            }
            else if (name == "skeleton")
            {
                context.level.monsters.add(std::make_unique<Skeleton>(context, rect));
            }
            else if (name == "vampire")
            {
                context.level.monsters.add(std::make_unique<Vampire>(context, rect));
            }
            else if (name == "fire-knight")
            {
                context.level.monsters.add(std::make_unique<FireKnight>(context, rect));
            }
            else if (name == "fire-imp")
            {
                context.level.monsters.add(std::make_unique<FireImp>(context, rect));
            }
            else if (name == "hound")
            {
                context.level.monsters.add(std::make_unique<Hound>(context, rect));
            }
            else if (name == "imp")
            {
                context.level.monsters.add(std::make_unique<Imp>(context, rect));
            }
            else if (name == "salamander")
            {
                context.level.monsters.add(std::make_unique<Salamander>(context, rect));
            }
            else
            {
                std::cout << "WARNING:  While parsing level file \"" << m_pathStr
                          << "\".  Ignored unknown monster layer object named \"" << name
                          << "\".\n";
            }
        }
    }

} // namespace platformer
