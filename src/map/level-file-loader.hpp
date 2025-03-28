#ifndef LEVEL_FILE_LOADER_HPP_INCLUDED
#define LEVEL_FILE_LOADER_HPP_INCLUDED
//
// level-file-loader.hpp
//
#include "map/level.hpp"
#include "map/tile-image.hpp"
#include "subsystem/context.hpp"
#include "util/json-wrapper.hpp"

#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    class LevelFileLoader
    {
      public:
        LevelFileLoader();

        void load(Context & t_context, const std::string & t_filename);

      private:
        void parseLevelDetails(Context & t_context, const nlohmann::json & t_json);
        void parseObjectTextureGIDs(const nlohmann::json & t_json);
        void parseBackgroundImageName(Context & t_context, const nlohmann::json & t_json);
        void parseLayers(Context & t_context, const nlohmann::json & t_json);

        void parseTileLayer(
            Context & t_context, const TileImage image, const nlohmann::json & t_json);

        void parseRectLayer(
            const Context & t_context,
            const nlohmann::json & t_json,
            std::vector<sf::FloatRect> & t_rects);

        [[nodiscard]] sf::FloatRect
            parseAndConvertRect(const Context & t_context, const nlohmann::json & t_json);

        void parseSpawnLayer(Context & t_context, const nlohmann::json & t_json);
        void parsePickupAnimLayer(const Context & t_context, const nlohmann::json & t_json);
        void parseAccentAnimLayer(const Context & t_context, const nlohmann::json & t_json);
        void parseChestAnimLayer(const Context & t_context, const nlohmann::json & t_json);
        void parseMonsterLayer(const Context & t_context, const nlohmann::json & t_json);
        void parseFlameTrapLayer(const Context & t_context, const nlohmann::json & t_json);
        void parseFallingRockTrapLayer(const Context & t_context, const nlohmann::json & t_json);
        void parseLavaDripTrapLayer(const Context & t_context, const nlohmann::json & t_json);
        void parseDartTrapLayer(const Context & t_context, const nlohmann::json & t_json);
        void parseWaterRockLayer(const Context & t_context, const nlohmann::json & t_json);

        void parseWaterLayer(
            const Context & t_context, const nlohmann::json & t_json, const bool isSurface);

        template <typename Layer_t>
        void parseLayerOfRects(const Context & t_context, const nlohmann::json & t_json)
        {
            std::vector<sf::FloatRect> rects;
            rects.reserve(128); // harmless guess based on knowledge of maps in the game

            for (const nlohmann::json & subJson : t_json["objects"])
            {
                rects.push_back(parseAndConvertRect(t_context, subJson));
            }

            t_context.level.tile_layers.push_back(std::make_unique<Layer_t>(t_context, rects));
        }

      private:
        std::string m_pathStr;
    };

} // namespace bramblefore

#endif // LEVEL_FILE_LOADER_HPP_INCLUDED
