// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "level-file-loader.hpp"

#include "accents.hpp"
#include "acid-anim-layer.hpp"
#include "acid-spout-anim-layer.hpp"
#include "background-images.hpp"
#include "check-macros.hpp"
#include "flaming-skull-anim-layer.hpp"
#include "lightning-anim-layer.hpp"
#include "map-textures.hpp"
#include "monster-baby-dragon.hpp"
#include "monster-bear.hpp"
#include "monster-big-knight.hpp"
#include "monster-bone-dragon.hpp"
#include "monster-boss-knight.hpp"
#include "monster-boss-tribal.hpp"
#include "monster-boss-wizard.hpp"
#include "monster-demon.hpp"
#include "monster-dino.hpp"
#include "monster-djinn.hpp"
#include "monster-dragon.hpp"
#include "monster-dwarf.hpp"
#include "monster-ent.hpp"
#include "monster-fire-imp.hpp"
#include "monster-fire-knight.hpp"
#include "monster-ghost.hpp"
#include "monster-goblin.hpp"
#include "monster-hound.hpp"
#include "monster-imp.hpp"
#include "monster-little-knight.hpp"
#include "monster-lizard.hpp"
#include "monster-medusa.hpp"
#include "monster-orc.hpp"
#include "monster-salamander.hpp"
#include "monster-skeleton.hpp"
#include "monster-skull.hpp"
#include "monster-snake.hpp"
#include "monster-spider.hpp"
#include "monster-vampire.hpp"
#include "monster-yeti.hpp"
#include "pickups.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "trap-anim-layer.hpp"
#include "water-anim-layer.hpp"

#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace platformer
{

    LevelFileLoader::LevelFileLoader()
        : m_pathStr()
    {}

    bool LevelFileLoader::load(Context & t_context)
    {
        // TODO fix to be more general
        const std::filesystem::path path = (t_context.settings.media_path / "map/dungeon1-1.json");
        if (!std::filesystem::exists(path))
        {
            return false;
        }

        m_pathStr = path.string();

        nlohmann::json json;

        {
            std::ifstream iStream(m_pathStr);
            M_CHECK(iStream, "Failed to open level file: \"" << m_pathStr << "\"");
            iStream >> json;
        }

        parseLevelDetails(t_context, json);
        parseBackgroundImageName(t_context, json);
        parseObjectTextureGIDs(json);

        // everything else in the level file is saved in "layers"
        // which are parsed in order from back to front here, one at a time
        parseLayers(t_context, json);

        return true;
    }

    void LevelFileLoader::parseLevelDetails(Context & t_context, const nlohmann::json & t_json)
    {
        // parse level tile size and counts
        t_context.level.tile_count = { t_json["width"], t_json["height"] };
        t_context.level.tile_size  = { t_json["tilewidth"], t_json["tileheight"] };

        t_context.level.tile_size_texture = sf::Vector2f{ t_context.level.tile_size };

        t_context.level.tile_size_screen =
            (sf::Vector2f{ t_context.level.tile_size } * t_context.settings.tile_scale);

        t_context.level.tile_size_screen.x = floorf(t_context.level.tile_size_screen.x);
        t_context.level.tile_size_screen.y = floorf(t_context.level.tile_size_screen.y);

        // calc map position offset
        const sf::Vector2f tileCountF{ t_context.level.tile_count };
        const sf::Vector2f mapSizeOrig{ t_context.level.tile_size_screen * tileCountF };

        const float heightOffset{
            (t_context.layout.wholeRect().top + t_context.layout.wholeRect().height) - mapSizeOrig.y
        };

        t_context.level.map_position_offset = { 0.0f, heightOffset };
    }

    void LevelFileLoader::parseObjectTextureGIDs(const nlohmann::json & wholeJson)
    {
        for (const nlohmann::json & json : wholeJson["tilesets"])
        {
            const std::string sourceStr{ json["source"] };
            const std::filesystem::path path{ sourceStr };
            const std::string filename{ path.filename().string() };

            const int gid{ json["firstgid"] };

            if (filename == toMapFilename(TileImage::CastleGround))
            {
                MapTextureManager::instance().setGid(TileImage::CastleGround, gid);
            }
            else if (filename == toMapFilename(TileImage::CastleObject))
            {
                MapTextureManager::instance().setGid(TileImage::CastleObject, gid);
            }
            else if (filename == toMapFilename(TileImage::Dungeon1Ground))
            {
                MapTextureManager::instance().setGid(TileImage::Dungeon1Ground, gid);
            }
            else if (filename == toMapFilename(TileImage::Dungeon2Ground))
            {
                MapTextureManager::instance().setGid(TileImage::Dungeon2Ground, gid);
            }
            else if (filename == toMapFilename(TileImage::Dungeon2Object))
            {
                MapTextureManager::instance().setGid(TileImage::Dungeon2Object, gid);
            }
            else if (filename == toMapFilename(TileImage::ForestGround))
            {
                MapTextureManager::instance().setGid(TileImage::ForestGround, gid);
            }
            else if (filename == toMapFilename(TileImage::ForestTrees))
            {
                MapTextureManager::instance().setGid(TileImage::ForestTrees, gid);
            }
            else if (filename == toMapFilename(TileImage::MountainsGround))
            {
                MapTextureManager::instance().setGid(TileImage::MountainsGround, gid);
            }
            else if (filename == toMapFilename(TileImage::MountainsObject))
            {
                MapTextureManager::instance().setGid(TileImage::MountainsObject, gid);
            }
            else
            {
                std::cout << "Warning: While parsing \"" << m_pathStr
                          << "\": Ignored tileset named \"" << filename << "\", with gid=" << gid
                          << "\n";
            }
        }
    }

    void LevelFileLoader::parseBackgroundImageName(
        Context & t_context, const nlohmann::json & t_json)
    {
        std::string backgroundImageName;
        for (const nlohmann::json & propJson : t_json["properties"])
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

        t_context.bg_image.setup(t_context, backgroundImageName);
    }

    void LevelFileLoader::parseLayers(Context & t_context, const nlohmann::json & jsonWholeFile)
    {
        for (const nlohmann::json & jsonLayer : jsonWholeFile["layers"])
        {
            const std::string layerName = jsonLayer["name"];

            if (layerName == toString(TileImage::CastleGround))
            {
                parseTileLayer(t_context, TileImage::CastleGround, jsonLayer);
            }
            else if (layerName == toString(TileImage::CastleObject))
            {
                parseTileLayer(t_context, TileImage::CastleObject, jsonLayer);
            }
            else if (layerName == toString(TileImage::Dungeon1Ground))
            {
                parseTileLayer(t_context, TileImage::Dungeon1Ground, jsonLayer);
            }
            else if (layerName == toString(TileImage::Dungeon2Ground))
            {
                parseTileLayer(t_context, TileImage::Dungeon2Ground, jsonLayer);
            }
            else if (layerName == toString(TileImage::Dungeon2Object))
            {
                parseTileLayer(t_context, TileImage::Dungeon2Object, jsonLayer);
            }
            else if (layerName == toString(TileImage::ForestGround))
            {
                parseTileLayer(t_context, TileImage::ForestGround, jsonLayer);
            }
            else if (layerName == toString(TileImage::ForestTrees))
            {
                parseTileLayer(t_context, TileImage::ForestTrees, jsonLayer);
            }
            else if (layerName == toString(TileImage::MountainsGround))
            {
                parseTileLayer(t_context, TileImage::MountainsGround, jsonLayer);
            }
            else if (layerName == toString(TileImage::MountainsObject))
            {
                parseTileLayer(t_context, TileImage::MountainsObject, jsonLayer);
            }
            else if (layerName == "collision")
            {
                parseRectLayer(t_context, jsonLayer, t_context.level.collisions);
            }
            else if (layerName == "spawn")
            {
                parseSpawnLayer(t_context, jsonLayer);
            }
            else if (layerName == "pickup-anim")
            {
                parsePickupAnimLayer(t_context, jsonLayer);
            }
            else if (layerName == "accent-anim")
            {
                parseAccentAnimLayer(t_context, jsonLayer);
            }
            else if (layerName == "acid-anim")
            {
                parseLayerOfRects<AcidAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "acid-spout")
            {
                parseLayerOfRects<AcidSpoutAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "metal-trap")
            {
                parseLayerOfRects<TrapAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "fire-skull")
            {
                parseLayerOfRects<FlamingSkullAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "water-anim")
            {
                parseLayerOfRects<WaterAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "lightning-anim")
            {
                parseLayerOfRects<LightningAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "monster")
            {
                parseMonsterLayer(t_context, jsonLayer);
            }
            else
            {
                std::cout << "WARNING:  While parsing level file \"" << m_pathStr
                          << "\".  Ignored unknown layer named \"" << layerName << "\".\n";
            }
        }
    }

    void LevelFileLoader::parseTileLayer(
        Context & t_context, const TileImage image, const nlohmann::json & t_json)
    {
        const std::vector<int> indexes = t_json["data"];

        M_CHECK(
            !indexes.empty(),
            "Error Parsing Level File " << m_pathStr << ":  tile layer for image " << image
                                        << " was empty.");

        t_context.level.tile_layers.push_back(
            std::make_unique<TileLayer>(t_context, image, indexes));
    }

    void LevelFileLoader::parseRectLayer(
        Context & t_context, const nlohmann::json & t_json, std::vector<sf::FloatRect> & t_rects)
    {
        t_rects.clear();

        for (const nlohmann::json & collJson : t_json["objects"])
        {
            t_rects.emplace_back(parseAndConvertRect(t_context, collJson));
        }
    }

    sf::FloatRect LevelFileLoader::parseAndConvertRect(
        const Context & t_context, const nlohmann::json & t_json)
    {
        sf::IntRect mapRect;
        mapRect.left   = t_json["x"];
        mapRect.top    = t_json["y"];
        mapRect.width  = t_json["width"];
        mapRect.height = t_json["height"];

        // convert from map to screen coordinates
        sf::FloatRect screenRect{ mapRect };
        screenRect.left *= t_context.settings.tile_scale;
        screenRect.top *= t_context.settings.tile_scale;
        screenRect.width *= t_context.settings.tile_scale;
        screenRect.height *= t_context.settings.tile_scale;
        //
        screenRect.left += t_context.level.map_position_offset.x;
        screenRect.top += t_context.level.map_position_offset.y;

        return screenRect;
    }

    void LevelFileLoader::parseSpawnLayer(Context & t_context, const nlohmann::json & t_json)
    {
        t_context.level.enter_rect = { 0.0f, 0.0f, -1.0f, -1.0f };
        t_context.level.exit_rect  = { 0.0f, 0.0f, -1.0f, -1.0f };

        for (const nlohmann::json & spawnJson : t_json["objects"])
        {
            const std::string name   = spawnJson["name"];
            const sf::FloatRect rect = parseAndConvertRect(t_context, spawnJson);

            if (name == "enter")
            {
                t_context.level.enter_rect = rect;
            }
            else if (name == "exit")
            {
                t_context.level.exit_rect = rect;
            }
            else
            {
                std::cout << "WARNING:  While parsing level file \"" << m_pathStr
                          << "\".  Ignored unknown spawn rect named \"" << name << "\".\n";
            }
        }

        M_CHECK(
            (t_context.level.enter_rect.width > 0.0f),
            "Error Parsing Level File " << m_pathStr << ":  Failed to find enter location.");

        M_CHECK(
            (t_context.level.exit_rect.width > 0.0f),
            "Error Parsing Level File " << m_pathStr << ":  Failed to find exit location.");
    }

    void LevelFileLoader::parsePickupAnimLayer(Context & t_context, const nlohmann::json & t_json)
    {
        for (const nlohmann::json & pickupJson : t_json["objects"])
        {
            const std::string name   = pickupJson["name"];
            const sf::FloatRect rect = parseAndConvertRect(t_context, pickupJson);
            t_context.pickup.add(t_context, rect, name);
        }
    }

    void LevelFileLoader::parseAccentAnimLayer(Context & t_context, const nlohmann::json & t_json)
    {
        for (const nlohmann::json & accentJson : t_json["objects"])
        {
            const std::string name   = accentJson["name"];
            const sf::FloatRect rect = parseAndConvertRect(t_context, accentJson);
            t_context.accent.add(t_context, rect, name);
        }
    }

    void LevelFileLoader::parseMonsterLayer(Context & t_context, const nlohmann::json & t_json)
    {
        for (const nlohmann::json & monsterJson : t_json["objects"])
        {
            const std::string name   = monsterJson["name"];
            const sf::FloatRect rect = parseAndConvertRect(t_context, monsterJson);

            if (name == "goblin")
            {
                t_context.level.monsters.add(std::make_unique<Goblin>(t_context, rect));
            }
            else if (name == "dino")
            {
                t_context.level.monsters.add(std::make_unique<Dino>(t_context, rect));
            }
            else if (name == "spider")
            {
                t_context.level.monsters.add(std::make_unique<Spider>(t_context, rect));
            }
            else if (name == "ent")
            {
                t_context.level.monsters.add(std::make_unique<Ent>(t_context, rect));
            }
            else if (name == "big-knight")
            {
                t_context.level.monsters.add(std::make_unique<BigKnight>(t_context, rect));
            }
            else if (name == "little-knight")
            {
                t_context.level.monsters.add(std::make_unique<LittleKnight>(t_context, rect));
            }
            else if (name == "bone-dragon")
            {
                t_context.level.monsters.add(std::make_unique<BoneDragon>(t_context, rect));
            }
            else if (name == "ghost")
            {
                t_context.level.monsters.add(std::make_unique<Ghost>(t_context, rect));
            }
            else if (name == "skeleton")
            {
                t_context.level.monsters.add(std::make_unique<Skeleton>(t_context, rect));
            }
            else if (name == "vampire")
            {
                t_context.level.monsters.add(std::make_unique<Vampire>(t_context, rect));
            }
            else if (name == "fire-knight")
            {
                t_context.level.monsters.add(std::make_unique<FireKnight>(t_context, rect));
            }
            else if (name == "fire-imp")
            {
                t_context.level.monsters.add(std::make_unique<FireImp>(t_context, rect));
            }
            else if (name == "hound")
            {
                t_context.level.monsters.add(std::make_unique<Hound>(t_context, rect));
            }
            else if (name == "imp")
            {
                t_context.level.monsters.add(std::make_unique<Imp>(t_context, rect));
            }
            else if (name == "salamander")
            {
                t_context.level.monsters.add(std::make_unique<Salamander>(t_context, rect));
            }
            else if (name == "skull")
            {
                t_context.level.monsters.add(std::make_unique<Skull>(t_context, rect));
            }
            else if (name == "bear")
            {
                t_context.level.monsters.add(std::make_unique<Bear>(t_context, rect));
            }
            else if (name == "dwarf")
            {
                t_context.level.monsters.add(std::make_unique<Dwarf>(t_context, rect));
            }
            else if (name == "orc")
            {
                t_context.level.monsters.add(std::make_unique<Orc>(t_context, rect));
            }
            else if (name == "snake")
            {
                t_context.level.monsters.add(std::make_unique<Snake>(t_context, rect));
            }
            else if (name == "yeti")
            {
                t_context.level.monsters.add(std::make_unique<Yeti>(t_context, rect));
            }
            else if (name == "demon")
            {
                t_context.level.monsters.add(std::make_unique<Demon>(t_context, rect));
            }
            else if (name == "dragon")
            {
                t_context.level.monsters.add(std::make_unique<Dragon>(t_context, rect));
            }
            else if (name == "djinn")
            {
                t_context.level.monsters.add(std::make_unique<Djinn>(t_context, rect));
            }
            else if (name == "lizard")
            {
                t_context.level.monsters.add(std::make_unique<Lizard>(t_context, rect));
            }
            else if (name == "medusa")
            {
                t_context.level.monsters.add(std::make_unique<Medusa>(t_context, rect));
            }
            else if (name == "baby-dragon")
            {
                t_context.level.monsters.add(std::make_unique<BabyDragon>(t_context, rect));
            }
            else if (name == "boss-tribal")
            {
                t_context.level.monsters.add(std::make_unique<BossTribal>(t_context, rect));
            }
            else if (name == "boss-knight")
            {
                t_context.level.monsters.add(std::make_unique<BossKnight>(t_context, rect));
            }
            else if (name == "boss-wizard")
            {
                t_context.level.monsters.add(std::make_unique<BossWizard>(t_context, rect));
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
