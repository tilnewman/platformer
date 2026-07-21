#ifndef SCREEN_LAYOUT_HPP_INCLUDED
#define SCREEN_LAYOUT_HPP_INCLUDED
//
// screen-layout.hpp
//
#include <SFML/Graphics/Rect.hpp>

namespace bramblefore
{

    struct Context;

    //

    enum class CalcOrder : unsigned char
    {
        Normal,
        Inverse
    };

    //

    class ScreenLayout
    {
      public:
        ScreenLayout();

        void setup(const sf::Vector2u & t_windowSize);

        [[nodiscard]] inline const sf::Vector2f wholeSize() const noexcept { return m_wholeSize; }
        [[nodiscard]] inline const sf::FloatRect wholeRect() const noexcept { return m_wholeRect; }

        [[nodiscard]] float calScaleBasedOnResolution(
            const Context & t_context,
            const float t_originalScale,
            const CalcOrder t_order = CalcOrder::Normal) const;

      private:
        sf::Vector2f m_wholeSize;
        sf::FloatRect m_wholeRect;
    };

} // namespace bramblefore

#endif // SCREEN_LAYOUT_HPP_INCLUDED
