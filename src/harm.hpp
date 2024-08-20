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
        explicit Harm(const sf::FloatRect & r = {}, const int d = 0, const std::string & s = "")
            : damage(d)
            , sfx(s)
            , rect(r)
        {}

        inline bool isAnyHarmDone() const { return (damage > 0); }

        int damage;
        std::string sfx;
        sf::FloatRect rect;
    };

} // namespace platformer

#endif // HARM_HPP_INCLUDED
