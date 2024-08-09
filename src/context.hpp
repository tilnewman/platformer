#ifndef CONTEXT_HPP_INCLUDED
#define CONTEXT_HPP_INCLUDED
//
// context.hpp
//

namespace sf
{
    class Window;
}

namespace util
{
    class Random;
    // class MusicPlayer;
    // class SoundPlayer;
} // namespace util

namespace platformer
{

    struct Settings;

    //

    struct Context
    {
        Context(const Settings & set, sf::Window & win, util::Random & ran)
            : settings(set)
            , window(win)
            , random(ran)
        {}

        const Settings & settings;
        sf::Window & window;
        util::Random & random;
    };

} // namespace platformer

#endif // CONTEXT_HPP_INCLUDED
