#ifndef HARM_HPP_INCLUDED
#define HARM_HPP_INCLUDED
//
// harm.hpp
//
#include <string>

#include <SFML/Graphics/Rect.hpp>

namespace platformer
{

    struct Harm
    {
        explicit Harm(
            const sf::FloatRect & t_rect = {},
            const int t_damage           = 0,
            const std::string & t_sfx    = "")
            : damage(t_damage)
            , sfx(t_sfx)
            , rect(t_rect)
        {}

       [[nodiscard]] inline bool isAnyHarmDone() const noexcept { return ((damage > 0) || !sfx.empty()); }

        int damage;
        std::string sfx;
        sf::FloatRect rect;
    };

} // namespace platformer

#endif // HARM_HPP_INCLUDED
