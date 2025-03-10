// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// item-images.cpp
//
#include "item-images.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "util/texture-loader.hpp"

namespace bramblefore
{

    ItemImages::ItemImages()
        : m_iconsTexture{}
        , m_backgroundTexture{}
    {}

    void ItemImages::setup(const Settings & t_settings)
    {
        util::TextureLoader::load(
            m_iconsTexture, (t_settings.media_path / "image/item/items.png"), true);

        util::TextureLoader::load(
            m_backgroundTexture, (t_settings.media_path / "image/item/item-background.png"), true);
    }

} // namespace bramblefore
