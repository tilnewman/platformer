// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// map-textures.cpp
//
#include "map-textures.hpp"

#include "check-macros.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"

namespace platformer
{

    MapTextures::MapTextures()
        : forest_ground()
        , forest_tree()
    {}

    void MapTextures::setup(const Settings & settings)
    {
        const std::string imagePath = (settings.media_path / "image/map/").string();

        forest_ground.which = TileImage::ForestGround;
        forest_ground.texture.loadFromFile(imagePath + "forest-ground.png");
        forest_ground.texture.setSmooth(false);
        forest_ground.size = sf::Vector2i(forest_ground.texture.getSize());
        forest_ground.gid  = 0;

        forest_tree.texture.loadFromFile(imagePath + "forest-trees.png");
        forest_tree.which = TileImage::ForestTrees;
        forest_tree.texture.setSmooth(true);
        forest_tree.size = sf::Vector2i(forest_tree.texture.getSize());
        forest_tree.gid  = 0;
    }

    TileTexture & MapTextures::get(const TileImage image)
    {
        // clang-format off
        switch (image)
        {
            case TileImage::ForestGround:   { return forest_ground; }
            case TileImage::ForestTrees:    { return forest_tree;   }
            default:
            {
                throw std::runtime_error("MapTextures::tileTexture() given an invalid TileImage enum.");
            }
        }
        // clang-format on
    }

} // namespace platformer
