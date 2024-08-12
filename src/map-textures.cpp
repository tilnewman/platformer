// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// map-textures.cpp
//
#include "map-textures.hpp"

#include "check-macros.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

namespace platformer
{

    MapTextures::MapTextures()
        : m_castleGround()
        , m_castleObject()
        , m_dungeon1Ground()
        , m_dungeon2Ground()
        , m_dungeon2Object()
        , m_forestGround()
        , m_forestTrees()
        , m_mountainsGround()
        , m_mountainsObject()
    {}

    void MapTextures::setup(const Settings & settings)
    {
        const std::string imagePath = (settings.media_path / "image/map/").string();

        m_castleGround.which = TileImage::CastleGround;
        m_castleGround.texture.loadFromFile(imagePath + "castle-ground.png");
        TextureStats::instance().process(m_castleGround.texture);
        m_castleGround.texture.setSmooth(false);
        m_castleGround.size = sf::Vector2i(m_castleGround.texture.getSize());
        m_castleGround.gid  = 0;

        m_castleObject.which = TileImage::CastleObject;
        m_castleObject.texture.loadFromFile(imagePath + "castle-object.png");
        TextureStats::instance().process(m_castleObject.texture);
        m_castleObject.texture.setSmooth(true);
        m_castleObject.size = sf::Vector2i(m_castleObject.texture.getSize());
        m_castleObject.gid  = 0;

        m_dungeon1Ground.which = TileImage::Dungeon1Ground;
        m_dungeon1Ground.texture.loadFromFile(imagePath + "dungeon1-ground.png");
        TextureStats::instance().process(m_dungeon1Ground.texture);
        m_dungeon1Ground.texture.setSmooth(false);
        m_dungeon1Ground.size = sf::Vector2i(m_dungeon1Ground.texture.getSize());
        m_dungeon1Ground.gid  = 0;

        m_dungeon2Ground.which = TileImage::Dungeon2Ground;
        m_dungeon2Ground.texture.loadFromFile(imagePath + "dungeon2-ground.png");
        TextureStats::instance().process(m_dungeon2Ground.texture);
        m_dungeon2Ground.texture.setSmooth(false);
        m_dungeon2Ground.size = sf::Vector2i(m_dungeon2Ground.texture.getSize());
        m_dungeon2Ground.gid  = 0;

        m_dungeon2Object.which = TileImage::Dungeon2Object;
        m_dungeon2Object.texture.loadFromFile(imagePath + "dungeon2-object.png");
        TextureStats::instance().process(m_dungeon2Object.texture);
        m_dungeon2Object.texture.setSmooth(true);
        m_dungeon2Object.size = sf::Vector2i(m_dungeon2Object.texture.getSize());
        m_dungeon2Object.gid  = 0;

        m_forestGround.which = TileImage::ForestGround;
        m_forestGround.texture.loadFromFile(imagePath + "forest-ground.png");
        TextureStats::instance().process(m_forestGround.texture);
        m_forestGround.texture.setSmooth(false);
        m_forestGround.size = sf::Vector2i(m_forestGround.texture.getSize());
        m_forestGround.gid  = 0;

        m_forestTrees.which = TileImage::ForestTrees;
        m_forestTrees.texture.loadFromFile(imagePath + "forest-trees.png");
        TextureStats::instance().process(m_forestTrees.texture);
        m_forestTrees.texture.setSmooth(true);
        m_forestTrees.size = sf::Vector2i(m_forestTrees.texture.getSize());
        m_forestTrees.gid  = 0;

        m_mountainsGround.which = TileImage::MountainsGround;
        m_mountainsGround.texture.loadFromFile(imagePath + "mountains-ground.png");
        TextureStats::instance().process(m_mountainsGround.texture);
        m_mountainsGround.texture.setSmooth(false);
        m_mountainsGround.size = sf::Vector2i(m_mountainsGround.texture.getSize());
        m_mountainsGround.gid  = 0;

        m_mountainsObject.which = TileImage::MountainsObject;
        m_mountainsObject.texture.loadFromFile(imagePath + "mountains-object.png");
        TextureStats::instance().process(m_mountainsObject.texture);
        m_mountainsObject.texture.setSmooth(false);
        m_mountainsObject.size = sf::Vector2i(m_mountainsObject.texture.getSize());
        m_mountainsObject.gid  = 0;
    }

    TileTexture & MapTextures::get(const TileImage image)
    {
        // clang-format off
        switch (image)
        {
            case TileImage::CastleGround:       { return m_castleGround;    }
            case TileImage::CastleObject:       { return m_castleObject;    }
            case TileImage::Dungeon1Ground:     { return m_dungeon1Ground;  }
            case TileImage::Dungeon2Ground:     { return m_dungeon2Ground;  }
            case TileImage::Dungeon2Object:     { return m_dungeon2Object;  }
            case TileImage::ForestGround:       { return m_forestGround;    }
            case TileImage::ForestTrees:        { return m_forestTrees;     }
            case TileImage::MountainsGround:    { return m_mountainsGround; } 
            case TileImage::MountainsObject:    { return m_mountainsObject; }
            default:
            {
                throw std::runtime_error("MapTextures::get() given an invalid TileImage enum.");
            }
        }
        // clang-format on
    }

} // namespace platformer
