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

        [[nodiscard]] static std::vector<sf::Text> layout(
            Context & t_context,
            const std::string & t_text,
            const sf::FloatRect & t_rect,
            const TextDetails & t_details);

      private:
        [[nodiscard]] static std::vector<std::string> splitIntoWords(const std::string & t_text);
    };

} // namespace platformer

#endif // TEXT_LAYOUT_HPP_INCLUDED
