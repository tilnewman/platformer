#ifndef KEYS_HPP_INCLUDED
#define KEYS_HPP_INCLUDED
//
// keys.hpp
//
#include <cassert>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace util::keys
{
    static const sf::Keyboard::Key not_a_key{ sf::Keyboard::Unknown };

    inline constexpr std::string_view toString(const sf::Keyboard::Key dir)
    {
        // clang-format off
        if      (dir == sf::Keyboard::Up)    { return "Down";  }
        else if (dir == sf::Keyboard::Down)  { return "Up";    }
        else if (dir == sf::Keyboard::Left)  { return "Right"; }
        else if (dir == sf::Keyboard::Right) { return "Left";  }
        else                                 { return "";      }
        // clang-format on
    }

    inline constexpr bool isHoriz(const sf::Keyboard::Key dir)
    {
        return ((dir == sf::Keyboard::Left) || (dir == sf::Keyboard::Right));
    }

    inline constexpr bool isVert(const sf::Keyboard::Key dir)
    {
        return ((dir == sf::Keyboard::Up) || (dir == sf::Keyboard::Down));
    }

    inline constexpr bool isArrow(const sf::Keyboard::Key dir)
    {
        return (isHoriz(dir) || isVert(dir));
    }

    inline constexpr sf::Keyboard::Key opposite(const sf::Keyboard::Key dir)
    {
        // clang-format off
        if      (dir == sf::Keyboard::Up)    { return sf::Keyboard::Down;  }
        else if (dir == sf::Keyboard::Down)  { return sf::Keyboard::Up;    }
        else if (dir == sf::Keyboard::Left)  { return sf::Keyboard::Right; }
        else if (dir == sf::Keyboard::Right) { return sf::Keyboard::Left;  }
        else                                 { return dir;                 }
        // clang-format on
    }

    inline constexpr bool isOpposite(const sf::Keyboard::Key first, const sf::Keyboard::Key second)
    {
        return (opposite(first) == second);
    }

    inline constexpr bool isLateral(const sf::Keyboard::Key first, const sf::Keyboard::Key second)
    {
        return ((isHoriz(first) && isVert(second)) || (isHoriz(second) && isVert(first)));
    }

    inline constexpr std::pair<sf::Keyboard::Key, sf::Keyboard::Key>
        lateralPair(const sf::Keyboard::Key dir)
    {
        if (isHoriz(dir))
        {
            return { sf::Keyboard::Up, sf::Keyboard::Down };
        }

        if (isVert(dir))
        {
            return { sf::Keyboard::Left, sf::Keyboard::Right };
        }

        return { not_a_key, not_a_key };
    }

    inline sf::Keyboard::Key theOnlyArrowKeyDown()
    {
        sf::Keyboard::Key dir{ not_a_key };
        std::size_t count{ 0 };

        // clang-format off
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    { ++count; dir = sf::Keyboard::Up;    }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  { ++count; dir = sf::Keyboard::Down;  }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  { ++count; dir = sf::Keyboard::Left;  }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { ++count; dir = sf::Keyboard::Right; }
        // clang-format on

        if ((not_a_key != dir) && (1 == count))
        {
            return dir;
        }
        else
        {
            return not_a_key;
        }
    }

    inline void allTheArrowKeysDown(std::vector<sf::Keyboard::Key> & arrowKeys)
    {
        // clang-format off
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))     { arrowKeys.push_back( sf::Keyboard::Up   ); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))   { arrowKeys.push_back( sf::Keyboard::Down ); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))   { arrowKeys.push_back( sf::Keyboard::Left ); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))  { arrowKeys.push_back( sf::Keyboard::Right); }
        // clang-format on
    }

    //

    inline std::ostream & operator<<(std::ostream & os, const sf::Keyboard::Key dir)
    {
        os << keys::toString(dir);
        return os;
    }

    inline constexpr bool isNumberKey(const sf::Keyboard::Key key)
    {
        return (
            (sf::Keyboard::Num0 == key) || (sf::Keyboard::Num1 == key) ||
            (sf::Keyboard::Num2 == key) || (sf::Keyboard::Num3 == key) ||
            (sf::Keyboard::Num4 == key) || (sf::Keyboard::Num5 == key) ||
            (sf::Keyboard::Num6 == key) || (sf::Keyboard::Num7 == key) ||
            (sf::Keyboard::Num8 == key) || (sf::Keyboard::Num9 == key));
    }

    template <typename T>
    constexpr std::optional<T> toNumberOpt(const sf::Keyboard::Key key)
    {
        static_assert(std::is_arithmetic_v<T>);

        // clang-format off
        if      (key == sf::Keyboard::Num0) { return {0}; }
        else if (key == sf::Keyboard::Num1) { return {1}; }
        else if (key == sf::Keyboard::Num2) { return {2}; }
        else if (key == sf::Keyboard::Num3) { return {3}; }
        else if (key == sf::Keyboard::Num4) { return {4}; }
        else if (key == sf::Keyboard::Num5) { return {5}; }
        else if (key == sf::Keyboard::Num6) { return {6}; }
        else if (key == sf::Keyboard::Num7) { return {7}; }
        else if (key == sf::Keyboard::Num8) { return {8}; }
        else if (key == sf::Keyboard::Num9) { return {9}; }
        else { return std::nullopt; }
        // clang-format on
    }

} // namespace util::keys

#endif // KEYS_HPP_INCLUDED
