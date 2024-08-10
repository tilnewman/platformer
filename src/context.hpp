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

    struct Level;
    struct Settings;
    class MapTextures;
    class ScreenLayout;
    class LevelFileLoader;
    class AvatarTextureManager;

    //

    struct Context
    {
        Context(
            const Settings & set,
            sf::Window & win,
            util::Random & ran,
            AvatarTextureManager & atm,
            ScreenLayout & scl,
            LevelFileLoader & lfl,
            Level & lvl,
            MapTextures & mtx)
            : settings(set)
            , window(win)
            , random(ran)
            , avatar_textures(atm)
            , layout(scl)
            , level_loader(lfl)
            , level(lvl)
            , map_textures(mtx)
        {}

        const Settings & settings;
        sf::Window & window;
        util::Random & random;
        AvatarTextureManager & avatar_textures;
        ScreenLayout & layout;
        LevelFileLoader & level_loader;
        Level & level;
        MapTextures & map_textures;
    };

} // namespace platformer

#endif // CONTEXT_HPP_INCLUDED
