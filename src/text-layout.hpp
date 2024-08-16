#ifndef TEXT_LAYOUT_HPP_INCLUDED
#define TEXT_LAYOUT_HPP_INCLUDED
//
// text-layout.hpp
//
#include "font.hpp"

#include <string>
#include <vector>

#include <SFML/Graphics/Text.hpp>

namespace platformer
{

    struct Context;

    //

    class TextLayout
    {
      public:
        TextLayout();

        static const std::vector<sf::Text> layout(
            Context & context,
            const std::string & text,
            const sf::FloatRect & rect,
            const TextDetails & details);

      private:
        static const std::vector<std::string> splitIntoWords(const std::string & text);
    };

} // namespace platformer

#endif // TEXT_LAYOUT_HPP_INCLUDED
