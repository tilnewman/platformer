#ifndef FRAMERATE_DISPLAY_HPP_INCLUDED
#define FRAMERATE_DISPLAY_HPP_INCLUDED
//
// framerate-display.hpp
//
#include "util/stats-display.hpp"

#include <string>
#include <vector>
#include <memory>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>

namespace bramblefore
{

    struct Context;

    //

    class FramerateDisplay
    {
      public:
        FramerateDisplay();

        void setup(const Context & t_context);
        void update(const Context & t_context, const float t_elapsedTimeSec);
        
        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const;

      private:
        std::size_t m_frameCount;
        float m_elapsedTimeSec;
        std::unique_ptr<sf::Text> m_framerateTextUPtr;
    };

} // namespace bramblefore

#endif // FRAMERATE_DISPLAY_HPP_INCLUDED
