#ifndef LEVEL_FILE_LOADER_HPP_INCLUDED
#define LEVEL_FILE_LOADER_HPP_INCLUDED
//
// level-file-loader.hpp
//
#include "context.hpp"
#include "json-wrapper.hpp"
#include "level.hpp"
#include "tile-image.hpp"

#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    class LevelFileLoader
    {
      public:
        LevelFileLoader();

        bool load(Context & t_context);

      private:
        void parseLevelDetails(Context & t_context, const nlohmann::json & t_json);
        void parseObjectTextureGIDs(const nlohmann::json & t_json);
        void parseBackgroundImageName(Context & t_context, const nlohmann::json & t_json);
        void parseLayers(Context & t_context, const nlohmann::json & t_json);

        void parseTileLayer(
            Context & t_context, const TileImage image, const nlohmann::json & t_json);

        void parseRectLayer(
            Context & t_context,
            const nlohmann::json & t_json,
            std::vector<sf::FloatRect> & t_rects);

        sf::FloatRect parseAndConvertRect(const Context & t_context, const nlohmann::json & t_json);
        void parseSpawnLayer(Context & t_context, const nlohmann::json & t_json);
        void parsePickupAnimLayer(Context & t_context, const nlohmann::json & t_json);
        void parseAccentAnimLayer(Context & t_context, const nlohmann::json & t_json);
        void parseLightningAnimLayer(Context & t_context, const nlohmann::json & t_json);
        void parseMonsterLayer(Context & t_context, const nlohmann::json & t_json);

        template <typename Layer_t>
        void parseLayerOfRects(Context & t_context, const nlohmann::json & t_json)
        {
            std::vector<sf::FloatRect> rects;
            rects.reserve(128);

            for (const nlohmann::json & subJson : t_json["objects"])
            {
                rects.push_back(parseAndConvertRect(t_context, subJson));
            }

            t_context.level.tile_layers.push_back(std::make_unique<Layer_t>(t_context, rects));
        }

      private:
        std::string m_pathStr;
    };

} // namespace platformer

#endif // LEVEL_FILE_LOADER_HPP_INCLUDED
