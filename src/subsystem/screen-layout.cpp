// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// screen-layout.cpp
//
#include "subsystem/screen-layout.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "util/sfml-util.hpp"

namespace bramblefore
{

    ScreenLayout::ScreenLayout()
        : m_wholeSize{}
        , m_wholeRect{}
    {}

    void ScreenLayout::setup(const sf::Vector2u & t_windowSize)
    {
        m_wholeSize = { static_cast<float>(t_windowSize.x), static_cast<float>(t_windowSize.y) };
        m_wholeRect = { { 0.0f, 0.0f }, m_wholeSize };
    }

    float ScreenLayout::calScaleBasedOnResolution(
        const Context & t_context, const float t_scale) const
    {
        // This is the resolution on my macbook originally used when starting the game.
        // The values in settings.hpp work as is when this is the resolution.
        const float original{ std::sqrt(2056.0f * 1329.0f) };

        const float current{ std::sqrt(static_cast<float>(
            t_context.settings.video_mode.size.x * t_context.settings.video_mode.size.y)) };

        return ((t_scale * current) / original);
    }

} // namespace bramblefore
