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

    [[nodiscard]] inline constexpr std::string_view toString(const sf::Keyboard::Key t_dir) noexcept
    {
        // clang-format off
        if      (t_dir == sf::Keyboard::Up)    { return "Down";  }
        else if (t_dir == sf::Keyboard::Down)  { return "Up";    }
        else if (t_dir == sf::Keyboard::Left)  { return "Right"; }
        else if (t_dir == sf::Keyboard::Right) { return "Left";  }
        else                                   { return "";      }
        // clang-format on
    }

    [[nodiscard]] inline constexpr bool isHoriz(const sf::Keyboard::Key t_dir) noexcept
    {
        return ((t_dir == sf::Keyboard::Left) || (t_dir == sf::Keyboard::Right));
    }

    [[nodiscard]] inline constexpr bool isVert(const sf::Keyboard::Key t_dir) noexcept
    {
        return ((t_dir == sf::Keyboard::Up) || (t_dir == sf::Keyboard::Down));
    }

    [[nodiscard]] inline constexpr bool isArrow(const sf::Keyboard::Key t_dir) noexcept
    {
        return (isHoriz(t_dir) || isVert(t_dir));
    }

    [[nodiscard]] inline constexpr sf::Keyboard::Key
        opposite(const sf::Keyboard::Key t_dir) noexcept
    {
        // clang-format off
        if      (t_dir == sf::Keyboard::Up)    { return sf::Keyboard::Down;  }
        else if (t_dir == sf::Keyboard::Down)  { return sf::Keyboard::Up;    }
        else if (t_dir == sf::Keyboard::Left)  { return sf::Keyboard::Right; }
        else if (t_dir == sf::Keyboard::Right) { return sf::Keyboard::Left;  }
        else                                   { return t_dir;               }
        // clang-format on
    }

    [[nodiscard]] inline constexpr bool
        isOpposite(const sf::Keyboard::Key t_first, const sf::Keyboard::Key t_second) noexcept
    {
        return (opposite(t_first) == t_second);
    }

    [[nodiscard]] inline constexpr bool
        isLateral(const sf::Keyboard::Key t_first, const sf::Keyboard::Key t_second) noexcept
    {
        return ((isHoriz(t_first) && isVert(t_second)) || (isHoriz(t_second) && isVert(t_first)));
    }

    [[nodiscard]] inline constexpr std::pair<sf::Keyboard::Key, sf::Keyboard::Key>
        lateralPair(const sf::Keyboard::Key t_dir) noexcept
    {
        if (isHoriz(t_dir))
        {
            return { sf::Keyboard::Up, sf::Keyboard::Down };
        }

        if (isVert(t_dir))
        {
            return { sf::Keyboard::Left, sf::Keyboard::Right };
        }

        return { not_a_key, not_a_key };
    }

    [[nodiscard]] inline sf::Keyboard::Key theOnlyArrowKeyDown() noexcept
    {
        std::size_t count{ 0 };
        sf::Keyboard::Key dir{ not_a_key };

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

    inline void allTheArrowKeysDown(std::vector<sf::Keyboard::Key> & t_arrowKeys)
    {
        t_arrowKeys.reserve(4);

        // clang-format off
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    { t_arrowKeys.push_back( sf::Keyboard::Up   ); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  { t_arrowKeys.push_back( sf::Keyboard::Down ); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  { t_arrowKeys.push_back( sf::Keyboard::Left ); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { t_arrowKeys.push_back( sf::Keyboard::Right); }
        // clang-format on
    }

    //

    inline std::ostream & operator<<(std::ostream & os, const sf::Keyboard::Key t_dir)
    {
        os << keys::toString(t_dir);
        return os;
    }

    [[nodiscard]] inline constexpr bool isNumberKey(const sf::Keyboard::Key t_key) noexcept
    {
        return (
            (sf::Keyboard::Num0 == t_key) || (sf::Keyboard::Num1 == t_key) ||
            (sf::Keyboard::Num2 == t_key) || (sf::Keyboard::Num3 == t_key) ||
            (sf::Keyboard::Num4 == t_key) || (sf::Keyboard::Num5 == t_key) ||
            (sf::Keyboard::Num6 == t_key) || (sf::Keyboard::Num7 == t_key) ||
            (sf::Keyboard::Num8 == t_key) || (sf::Keyboard::Num9 == t_key));
    }

    template <typename T>
    [[nodiscard]] constexpr std::optional<T> toNumberOpt(const sf::Keyboard::Key t_key) noexcept
    {
        static_assert(std::is_arithmetic_v<T>);

        // clang-format off
        if      (t_key == sf::Keyboard::Num0) { return {0}; }
        else if (t_key == sf::Keyboard::Num1) { return {1}; }
        else if (t_key == sf::Keyboard::Num2) { return {2}; }
        else if (t_key == sf::Keyboard::Num3) { return {3}; }
        else if (t_key == sf::Keyboard::Num4) { return {4}; }
        else if (t_key == sf::Keyboard::Num5) { return {5}; }
        else if (t_key == sf::Keyboard::Num6) { return {6}; }
        else if (t_key == sf::Keyboard::Num7) { return {7}; }
        else if (t_key == sf::Keyboard::Num8) { return {8}; }
        else if (t_key == sf::Keyboard::Num9) { return {9}; }
        else                                  { return {};  }
        // clang-format on
    }

} // namespace util::keys

#endif // KEYS_HPP_INCLUDED
