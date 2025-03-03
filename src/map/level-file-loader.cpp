// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "map/level-file-loader.hpp"

#include "anim-layer-acid-spout.hpp"
#include "anim-layer-acid.hpp"
#include "anim-layer-bomb.hpp"
#include "anim-layer-dart-trap.hpp"
#include "anim-layer-falling-rock.hpp"
#include "anim-layer-firewall.hpp"
#include "anim-layer-flame-trap.hpp"
#include "anim-layer-flaming-skull.hpp"
#include "anim-layer-ghost-bottle.hpp"
#include "anim-layer-lava-drip.hpp"
#include "anim-layer-lava.hpp"
#include "anim-layer-lightning.hpp"
#include "anim-layer-plant-puke.hpp"
#include "anim-layer-plant-trap.hpp"
#include "anim-layer-saw.hpp"
#include "anim-layer-spike-trap.hpp"
#include "anim-layer-trap.hpp"
#include "anim-layer-water.hpp"
#include "bramblefore/settings.hpp"
#include "map-textures.hpp"
#include "map/accents.hpp"
#include "map/anim-layer-chest.hpp"
#include "map/anim-layer-mimic.hpp"
#include "map/pickups.hpp"
#include "monster/monster-baby-dragon.hpp"
#include "monster/monster-bear.hpp"
#include "monster/monster-big-knight.hpp"
#include "monster/monster-bone-dragon.hpp"
#include "monster/monster-boss-knight.hpp"
#include "monster/monster-boss-tribal.hpp"
#include "monster/monster-boss-wizard.hpp"
#include "monster/monster-demon.hpp"
#include "monster/monster-dino.hpp"
#include "monster/monster-djinn.hpp"
#include "monster/monster-dragon.hpp"
#include "monster/monster-dwarf.hpp"
#include "monster/monster-ent.hpp"
#include "monster/monster-fire-imp.hpp"
#include "monster/monster-fire-knight.hpp"
#include "monster/monster-ghost.hpp"
#include "monster/monster-goblin.hpp"
#include "monster/monster-hound.hpp"
#include "monster/monster-imp.hpp"
#include "monster/monster-little-knight.hpp"
#include "monster/monster-lizard.hpp"
#include "monster/monster-medusa.hpp"
#include "monster/monster-orc.hpp"
#include "monster/monster-salamander.hpp"
#include "monster/monster-skeleton.hpp"
#include "monster/monster-skull.hpp"
#include "monster/monster-snake.hpp"
#include "monster/monster-spider.hpp"
#include "monster/monster-vampire.hpp"
#include "monster/monster-yeti.hpp"
#include "subsystem/background-images.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-util.hpp"

#include <filesystem>
#include <fstream>

namespace bramblefore
{

    LevelFileLoader::LevelFileLoader()
        : m_pathStr()
    {}

    void LevelFileLoader::load(Context & t_context, const std::string & t_filename)
    {
        const std::filesystem::path path{ t_context.settings.media_path / "map" / t_filename };
        M_CHECK(std::filesystem::exists(path), "The level file does not exist: " << path.string());

        m_pathStr = path.string();

        nlohmann::json json;

        {
            std::ifstream iStream(m_pathStr);
            M_CHECK(iStream, "Found but failed to open the level file: \"" << m_pathStr << "\"");
            iStream >> json;
        }

        parseLevelDetails(t_context, json);
        parseBackgroundImageName(t_context, json);
        parseObjectTextureGIDs(json);

        // everything else in the level file is saved in "layers"
        // which are parsed in order from back to front here, one at a time
        parseLayers(t_context, json);
    }

    void LevelFileLoader::parseLevelDetails(Context & t_context, const nlohmann::json & t_json)
    {
        // parse level tile size and counts
        t_context.level.tile_count = { t_json["width"], t_json["height"] };
        t_context.level.tile_size  = { t_json["tilewidth"], t_json["tileheight"] };

        t_context.level.tile_size_texture = sf::Vector2f{ t_context.level.tile_size };

        const float scale{ t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.tile_scale) };

        t_context.level.tile_size_screen = (sf::Vector2f{ t_context.level.tile_size } * scale);

        t_context.level.tile_size_screen.x = floorf(t_context.level.tile_size_screen.x);
        t_context.level.tile_size_screen.y = floorf(t_context.level.tile_size_screen.y);

        // calc map position offset
        const sf::Vector2f tileCountF{ t_context.level.tile_count };
        const sf::Vector2f mapSizeOrig{ t_context.level.tile_size_screen * tileCountF };

        const float heightOffset{ (t_context.layout.wholeRect().position.y +
                                   t_context.layout.wholeRect().size.y) -
                                  mapSizeOrig.y };

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
            else if (filename == toMapFilename(TileImage::CaveGround))
            {
                MapTextureManager::instance().setGid(TileImage::CaveGround, gid);
            }
            else if (filename == toMapFilename(TileImage::CaveObject))
            {
                MapTextureManager::instance().setGid(TileImage::CaveObject, gid);
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
                util::log() << "While parsing \"" << m_pathStr << "\": Ignored tileset named \""
                            << filename << "\", with gid=" << gid << "\n";
            }
        }
    }

    void LevelFileLoader::parseBackgroundImageName(
        Context & t_context, const nlohmann::json & t_json)
    {
        std::string backgroundImageName;

        try
        {
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
                    util::log() << "While parsing \"" << m_pathStr
                                << "\": Ignored custom property named \"" << propName << "\"\n";
                }
            }
        }
        catch (...)
        {
            util::log() << "Exception Error: While parsing \"" << m_pathStr
                        << "\":  Perhaps the map maker forgot to add the background property?\n";
        }

        if (backgroundImageName.empty())
        {
            util::log() << "Error: While parsing \"" << m_pathStr
                        << "\": This map file is missing the custom int property named "
                           "\"background\", there will be no background on this map.\n";
        }

        t_context.bg_image.setup(t_context, backgroundImageName);
    }

    void LevelFileLoader::parseLayers(Context & t_context, const nlohmann::json & jsonWholeFile)
    {
        for (const nlohmann::json & jsonLayer : jsonWholeFile["layers"])
        {
            const std::string layerName{ jsonLayer["name"] };

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
            else if (layerName == toString(TileImage::CaveGround))
            {
                parseTileLayer(t_context, TileImage::CaveGround, jsonLayer);
            }
            else if (layerName == toString(TileImage::CaveObject))
            {
                parseTileLayer(t_context, TileImage::CaveObject, jsonLayer);
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
            else if (layerName == "kill-collision")
            {
                parseRectLayer(t_context, jsonLayer, t_context.level.kill_collisions);
            }
            else if (layerName == "ladder")
            {
                parseRectLayer(t_context, jsonLayer, t_context.level.ladders);
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
            else if (layerName == "chest")
            {
                parseChestAnimLayer(t_context, jsonLayer);
            }
            else if (layerName == "acid-anim")
            {
                parseLayerOfRects<AcidAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "lava")
            {
                parseLayerOfRects<LavaAnimationLayer>(t_context, jsonLayer);
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
            else if (layerName == "water-anim-surface")
            {
                parseWaterLayer(t_context, jsonLayer, true);
            }
            else if (layerName == "water-anim-basin")
            {
                parseWaterLayer(t_context, jsonLayer, false);
            }
            else if (layerName == "lightning-anim")
            {
                parseLayerOfRects<LightningAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "ghost-bottle")
            {
                parseLayerOfRects<GhostBottleAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "bomb")
            {
                parseLayerOfRects<BombAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "mimic")
            {
                parseLayerOfRects<MimicAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "monster")
            {
                parseMonsterLayer(t_context, jsonLayer);
            }
            else if (layerName == "flame-trap")
            {
                parseFlameTrapLayer(t_context, jsonLayer);
            }
            else if (layerName == "plant-trap")
            {
                parseLayerOfRects<PlantTrapAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "puke-trap")
            {
                parseLayerOfRects<PukeTrapAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "spike-trap")
            {
                parseLayerOfRects<SpikeTrapAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "dart-trap")
            {
                parseDartTrapLayer(t_context, jsonLayer);
            }
            else if (layerName == "rock-trap")
            {
                parseFallingRockTrapLayer(t_context, jsonLayer);
            }
            else if (layerName == "lava-drip")
            {
                parseLavaDripTrapLayer(t_context, jsonLayer);
            }
            else if (layerName == "saw")
            {
                parseLayerOfRects<SawAnimationLayer>(t_context, jsonLayer);
            }
            else if (layerName == "firewall")
            {
                parseLayerOfRects<FirewallAnimationLayer>(t_context, jsonLayer);
            }
            else
            {
                util::log() << "While parsing level file \"" << m_pathStr
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
        mapRect.position.x = t_json["x"];
        mapRect.position.y = t_json["y"];
        mapRect.size.x     = t_json["width"];
        mapRect.size.y     = t_json["height"];

        const float scale{ t_context.layout.calScaleBasedOnResolution(
            t_context, t_context.settings.tile_scale) };

        // convert from map to screen coordinates
        sf::FloatRect screenRect{ mapRect };
        screenRect.position.x *= scale;
        screenRect.position.y *= scale;
        screenRect.size.x *= scale;
        screenRect.size.y *= scale;
        //
        screenRect.position.x += t_context.level.map_position_offset.x;
        screenRect.position.y += t_context.level.map_position_offset.y;

        return screenRect;
    }

    void LevelFileLoader::parseSpawnLayer(Context & t_context, const nlohmann::json & t_json)
    {
        t_context.level.enter_rect = { { 0.0f, 0.0f }, { -1.0f, -1.0f } };
        t_context.level.exit_rect  = { { 0.0f, 0.0f }, { -1.0f, -1.0f } };

        for (const nlohmann::json & spawnJson : t_json["objects"])
        {
            const std::string name{ spawnJson["name"] };
            const sf::FloatRect rect{ parseAndConvertRect(t_context, spawnJson) };

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
                util::log() << "While parsing level file \"" << m_pathStr
                            << "\".  Ignored unknown spawn rect named \"" << name << "\".\n";
            }
        }

        M_CHECK(
            (t_context.level.enter_rect.size.x > 0.0f),
            "Error Parsing Level File " << m_pathStr << ":  Failed to find enter location.");

        M_CHECK(
            (t_context.level.exit_rect.size.x > 0.0f),
            "Error Parsing Level File " << m_pathStr << ":  Failed to find exit location.");
    }

    void LevelFileLoader::parsePickupAnimLayer(Context & t_context, const nlohmann::json & t_json)
    {
        for (const nlohmann::json & pickupJson : t_json["objects"])
        {
            t_context.pickup.add(
                t_context, parseAndConvertRect(t_context, pickupJson), pickupJson["name"]);
        }
    }

    void LevelFileLoader::parseAccentAnimLayer(Context & t_context, const nlohmann::json & t_json)
    {
        for (const nlohmann::json & accentJson : t_json["objects"])
        {
            t_context.accent.add(
                t_context, parseAndConvertRect(t_context, accentJson), accentJson["name"]);
        }
    }

    void LevelFileLoader::parseChestAnimLayer(Context & t_context, const nlohmann::json & t_json)
    {
        auto chestAnimLayerUPtr{ std::make_unique<ChestAnimationLayer>(t_context) };

        for (const nlohmann::json & accentJson : t_json["objects"])
        {
            chestAnimLayerUPtr->add(
                t_context, accentJson["name"], parseAndConvertRect(t_context, accentJson));
        }

        t_context.level.tile_layers.push_back(std::move(chestAnimLayerUPtr));
    }

    void LevelFileLoader::parseMonsterLayer(Context & t_context, const nlohmann::json & t_json)
    {
        for (const nlohmann::json & monsterJson : t_json["objects"])
        {
            const std::string name{ monsterJson["name"] };
            const sf::FloatRect rect{ parseAndConvertRect(t_context, monsterJson) };

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
                util::log() << "While parsing level file \"" << m_pathStr
                            << "\".  Ignored unknown monster layer object named \"" << name
                            << "\".\n";
            }
        }
    }

    void LevelFileLoader::parseFlameTrapLayer(Context & t_context, const nlohmann::json & t_json)
    {
        std::vector<FlameTrapRectDir> rectDirs;
        rectDirs.reserve(16); // harmless guess based on what I know is in the maps

        for (const nlohmann::json & trapJson : t_json["objects"])
        {
            rectDirs.emplace_back(
                parseAndConvertRect(t_context, trapJson), stringToTrapDirection(trapJson["name"]));
        }

        t_context.level.tile_layers.push_back(
            std::make_unique<FlameTrapAnimationLayer>(t_context, rectDirs));
    }

    void LevelFileLoader::parseFallingRockTrapLayer(
        Context & t_context, const nlohmann::json & t_json)
    {
        std::vector<RectRock> rectRocks;
        rectRocks.reserve(32); // harmless guess based on what I know is in the maps

        for (const nlohmann::json & trapJson : t_json["objects"])
        {
            rectRocks.emplace_back(
                parseAndConvertRect(t_context, trapJson), stringToRock(trapJson["name"]));
        }

        t_context.level.tile_layers.push_back(
            std::make_unique<FallingRockAnimationLayer>(t_context, rectRocks));
    }

    void LevelFileLoader::parseLavaDripTrapLayer(Context & t_context, const nlohmann::json & t_json)
    {
        std::vector<LavaRectSize> rectSizes;
        rectSizes.reserve(16); // harmless guess based on what I know is in the maps

        for (const nlohmann::json & trapJson : t_json["objects"])
        {
            rectSizes.emplace_back(
                parseAndConvertRect(t_context, trapJson), stringToDripSize(trapJson["name"]));
        }

        t_context.level.tile_layers.push_back(
            std::make_unique<LavaDripAnimationLayer>(t_context, rectSizes));
    }

    void LevelFileLoader::parseDartTrapLayer(Context & t_context, const nlohmann::json & t_json)
    {
        std::vector<DartRectDir> rectDirs;
        rectDirs.reserve(16); // harmless guess based on what I know is in the maps

        for (const nlohmann::json & trapJson : t_json["objects"])
        {
            rectDirs.emplace_back(
                (trapJson["name"] == "left"), parseAndConvertRect(t_context, trapJson));
        }

        t_context.level.tile_layers.push_back(
            std::make_unique<DartTrapAnimationLayer>(t_context, rectDirs));
    }

    void LevelFileLoader::parseWaterLayer(
        Context & t_context, const nlohmann::json & t_json, const bool isSurface)
    {
        std::vector<WaterTypeRect> typeRects;
        typeRects.reserve(64); // harmless guess based on what I know is in the maps

        for (const nlohmann::json & trapJson : t_json["objects"])
        {
            typeRects.emplace_back(isSurface, parseAndConvertRect(t_context, trapJson));
        }

        t_context.level.tile_layers.push_back(
            std::make_unique<WaterAnimationLayer>(t_context, typeRects));
    }

} // namespace bramblefore
