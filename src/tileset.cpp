// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// tileset.cpp
//
#include "tileset.hpp"

namespace platformer
{

    TileSet::TileSet()
        : count(0, 0)
        , size(0, 0)
        , layers()
    {
        layers.reserve(16);
    }

    void TileSet::reset()
    {
        layers.clear();
        count = { 0, 0 };
        size  = { 0, 0 };
    }

} // namespace platformer
