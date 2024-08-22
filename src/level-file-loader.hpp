#ifndef LEVEL_FILE_LOADER_HPP_INCLUDED
#define LEVEL_FILE_LOADER_HPP_INCLUDED
//
// level-file-loader.hpp
//
#include "json.hpp"
#include "tile-image.hpp"

#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{
    struct Context;
    struct Settings;
    using Json = nlohmann::json;

    //

    class LevelFileLoader
    {
      public:
        LevelFileLoader();

        bool load(Context & context);

      private:
        void parseLevelDetails(Context & context, Json & json);
        void parseObjectTextureGIDs(Json & json);
        void parseBackgroundImageName(Context & context, Json & json);
        void parseLayers(Context & context, Json & json);
        void parseTileLayer(Context & context, const TileImage image, Json & json);
        void parseRectLayer(Context & context, Json & json, std::vector<sf::FloatRect> & rects);
        const sf::FloatRect parseAndConvertRect(const Context & context, Json & json);
        void parseSpawnLayer(Context & context, Json & json);
        void parsePickupAnimLayer(Context & context, Json & json);
        void parseAccentAnimLayer(Context & context, Json & json);
        void parseAcidAnimLayer(Context & context, Json & json);
        void parseWaterAnimLayer(Context & context, Json & json);
        void parseLightningAnimLayer(Context & context, Json & json);
        void parseMonsterLayer(Context & context, Json & json);

      private:
        std::string m_pathStr;
    };

} // namespace platformer

#endif // LEVEL_FILE_LOADER_HPP_INCLUDED
