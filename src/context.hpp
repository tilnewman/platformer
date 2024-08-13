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

    class Avatar;
    class MapTextures;
    class FontManager;
    class ScreenLayout;
    class StateManager;
    class SpellAnimations;
    class LevelFileLoader;
    class BackgroundImages;
    class PickupAnimations;
    class AccentAnimations;
    class AvatarTextureManager;

    //

    struct Context
    {
        Context(
            const Settings & set,
            sf::Window & win,
            util::Random & ran,
            StateManager & sma,
            FontManager & fma,
            Avatar & ava,
            AvatarTextureManager & atm,
            ScreenLayout & scl,
            LevelFileLoader & lfl,
            Level & lvl,
            MapTextures & mtx,
            BackgroundImages & bim,
            PickupAnimations & pan,
            AccentAnimations & aan,
            SpellAnimations & san)
            : settings(set)
            , window(win)
            , random(ran)
            , states(sma)
            , fonts(fma)
            , avatar(ava)
            , avatar_textures(atm)
            , layout(scl)
            , level_loader(lfl)
            , level(lvl)
            , map_textures(mtx)
            , bg_images(bim)
            , pickups(pan)
            , accents(aan)
            , spells(san)
        {}

        const Settings & settings;
        sf::Window & window;
        util::Random & random;
        StateManager & states;
        FontManager & fonts;
        Avatar & avatar;
        AvatarTextureManager & avatar_textures;
        ScreenLayout & layout;
        LevelFileLoader & level_loader;
        Level & level;
        MapTextures & map_textures;
        BackgroundImages & bg_images;
        PickupAnimations & pickups;
        AccentAnimations & accents;
        SpellAnimations & spells;
    };

} // namespace platformer

#endif // CONTEXT_HPP_INCLUDED
