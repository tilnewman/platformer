// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// item-images.cpp
//
#include "item-images.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/sfml-util.hpp"

namespace bramblefore
{

    ItemImages::ItemImages()
        : m_iconsTexture{}
        , m_backgroundTexture{}
    {}

    void ItemImages::setup(const Settings & t_settings)
    {
        m_iconsTexture.loadFromFile((t_settings.media_path / "image/item/items.png").string());
        m_iconsTexture.setSmooth(true);
        TextureStats::instance().process(m_iconsTexture);

        m_backgroundTexture.loadFromFile(
            (t_settings.media_path / "image/item/item-background.png").string());

        m_backgroundTexture.setSmooth(false);

        TextureStats::instance().process(m_backgroundTexture);
    }

} // namespace bramblefore
