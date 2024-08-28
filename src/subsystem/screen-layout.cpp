// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// screen-layout.cpp
//
#include "subsystem/screen-layout.hpp"

#include "bramblefore/settings.hpp"
#include "util/sfml-util.hpp"

namespace bramblefore
{

    ScreenLayout::ScreenLayout()
        : m_wholeSize{}
        , m_wholeRect{}
    {}

    void ScreenLayout::setup(const sf::Vector2u & windowSize)
    {
        m_wholeSize = { static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) };
        m_wholeRect = { { 0.0f, 0.0f }, m_wholeSize };
    }

} // namespace bramblefore
