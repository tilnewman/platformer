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
    static const sf::Keyboard::Scancode not_a_key{ sf::Keyboard::Scancode::Unknown };

    [[nodiscard]] inline constexpr std::string_view
        toString(const sf::Keyboard::Scancode t_dir) noexcept
    {
        // clang-format off
        if      (t_dir == sf::Keyboard::Scancode::Up)    { return "Down";  }
        else if (t_dir == sf::Keyboard::Scancode::Down)  { return "Up";    }
        else if (t_dir == sf::Keyboard::Scancode::Left)  { return "Right"; }
        else if (t_dir == sf::Keyboard::Scancode::Right) { return "Left";  }
        else                                   { return "";      }
        // clang-format on
    }

    [[nodiscard]] inline constexpr bool isHoriz(const sf::Keyboard::Scancode t_dir) noexcept
    {
        return (
            (t_dir == sf::Keyboard::Scancode::Left) || (t_dir == sf::Keyboard::Scancode::Right));
    }

    [[nodiscard]] inline constexpr bool isVert(const sf::Keyboard::Scancode t_dir) noexcept
    {
        return ((t_dir == sf::Keyboard::Scancode::Up) || (t_dir == sf::Keyboard::Scancode::Down));
    }

    [[nodiscard]] inline constexpr bool isArrow(const sf::Keyboard::Scancode t_dir) noexcept
    {
        return (isHoriz(t_dir) || isVert(t_dir));
    }

    [[nodiscard]] inline constexpr sf::Keyboard::Scancode
        opposite(const sf::Keyboard::Scancode t_dir) noexcept
    {
        // clang-format off
        if      (t_dir == sf::Keyboard::Scancode::Up)    { return sf::Keyboard::Scancode::Down;  }
        else if (t_dir == sf::Keyboard::Scancode::Down)  { return sf::Keyboard::Scancode::Up;    }
        else if (t_dir == sf::Keyboard::Scancode::Left)  { return sf::Keyboard::Scancode::Right; }
        else if (t_dir == sf::Keyboard::Scancode::Right) { return sf::Keyboard::Scancode::Left;  }
        else                                   { return t_dir;               }
        // clang-format on
    }

    [[nodiscard]] inline constexpr bool isOpposite(
        const sf::Keyboard::Scancode t_first, const sf::Keyboard::Scancode t_second) noexcept
    {
        return (opposite(t_first) == t_second);
    }

    [[nodiscard]] inline constexpr bool isLateral(
        const sf::Keyboard::Scancode t_first, const sf::Keyboard::Scancode t_second) noexcept
    {
        return ((isHoriz(t_first) && isVert(t_second)) || (isHoriz(t_second) && isVert(t_first)));
    }

    [[nodiscard]] inline constexpr std::
        pair<sf::Keyboard::Scancode, sf::Keyboard::Scancode>
        lateralPair(const sf::Keyboard::Scancode t_dir) noexcept
    {
        if (isHoriz(t_dir))
        {
            return { sf::Keyboard::Scancode::Up, sf::Keyboard::Scancode::Down };
        }

        if (isVert(t_dir))
        {
            return { sf::Keyboard::Scancode::Left, sf::Keyboard::Scancode::Right };
        }

        return { not_a_key, not_a_key };
    }

    [[nodiscard]] inline sf::Keyboard::Scancode theOnlyArrowKeyDown() noexcept
    {
        std::size_t count{ 0 };
        sf::Keyboard::Scancode dir{ not_a_key };

        // clang-format off
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))    { ++count; dir = sf::Keyboard::Scancode::Up;    }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))  { ++count; dir = sf::Keyboard::Scancode::Down;  }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))  { ++count; dir = sf::Keyboard::Scancode::Left;  }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) { ++count; dir = sf::Keyboard::Scancode::Right; }
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

    inline void allTheArrowKeysDown(std::vector<sf::Keyboard::Scancode> & t_arrowKeys)
    {
        // clang-format off
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))    { t_arrowKeys.push_back( sf::Keyboard::Scancode::Up   ); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))  { t_arrowKeys.push_back( sf::Keyboard::Scancode::Down ); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))  { t_arrowKeys.push_back( sf::Keyboard::Scancode::Left ); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) { t_arrowKeys.push_back( sf::Keyboard::Scancode::Right); }
        // clang-format on
    }

    //

    inline std::ostream & operator<<(std::ostream & os, const sf::Keyboard::Scancode t_dir)
    {
        os << keys::toString(t_dir);
        return os;
    }

    [[nodiscard]] inline constexpr bool
        isNumberKey(const sf::Keyboard::Scancode t_key) noexcept
    {
        return (
            (sf::Keyboard::Scancode::Num0 == t_key) || (sf::Keyboard::Scancode::Num1 == t_key) ||
            (sf::Keyboard::Scancode::Num2 == t_key) || (sf::Keyboard::Scancode::Num3 == t_key) ||
            (sf::Keyboard::Scancode::Num4 == t_key) || (sf::Keyboard::Scancode::Num5 == t_key) ||
            (sf::Keyboard::Scancode::Num6 == t_key) || (sf::Keyboard::Scancode::Num7 == t_key) ||
            (sf::Keyboard::Scancode::Num8 == t_key) || (sf::Keyboard::Scancode::Num9 == t_key));
    }

    template <typename T>
    [[nodiscard]] constexpr std::optional<T>
        toNumberOpt(const sf::Keyboard::Scancode t_key) noexcept
    {
        static_assert(std::is_arithmetic_v<T>);

        // clang-format off
        if      (t_key == sf::Keyboard::Scancode::Num0) { return {0}; }
        else if (t_key == sf::Keyboard::Scancode::Num1) { return {1}; }
        else if (t_key == sf::Keyboard::Scancode::Num2) { return {2}; }
        else if (t_key == sf::Keyboard::Scancode::Num3) { return {3}; }
        else if (t_key == sf::Keyboard::Scancode::Num4) { return {4}; }
        else if (t_key == sf::Keyboard::Scancode::Num5) { return {5}; }
        else if (t_key == sf::Keyboard::Scancode::Num6) { return {6}; }
        else if (t_key == sf::Keyboard::Scancode::Num7) { return {7}; }
        else if (t_key == sf::Keyboard::Scancode::Num8) { return {8}; }
        else if (t_key == sf::Keyboard::Scancode::Num9) { return {9}; }
        else                                            { return {};  }
        // clang-format on
    }

} // namespace util::keys

#endif // KEYS_HPP_INCLUDED
