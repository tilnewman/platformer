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
    class SoundPlayer;
} // namespace util

namespace platformer
{

    struct Level;
    struct Settings;

    class Avatar;
    class LevelInfo;
    class PlayerInfo;
    class ItemImages;
    class MapTextures;
    class FontManager;
    class ScreenLayout;
    class StateManager;
    class SpellAnimations;
    class LevelFileLoader;
    class BackgroundImages;
    class PickupAnimations;
    class AccentAnimations;
    class PlayerInfoDisplay;
    class AvatarTextureManager;

    //

    struct Context
    {
        Context(
            const Settings & set,
            sf::Window & win,
            util::Random & ran,
            util::SoundPlayer & spl,
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
            SpellAnimations & san,
            ItemImages & ima,
            LevelInfo & lin,
            PlayerInfo & pin,
            PlayerInfoDisplay & pid)
            : settings(set)
            , window(win)
            , random(ran)
            , sfx(spl)
            , state(sma)
            , font(fma)
            , avatar(ava)
            , avatar_texture(atm)
            , layout(scl)
            , level_loader(lfl)
            , level(lvl)
            , map_texture(mtx)
            , bg_image(bim)
            , pickup(pan)
            , accent(aan)
            , spell(san)
            , item(ima)
            , level_info(lin)
            , player(pin)
            , player_display(pid)
        {}

        const Settings & settings;
        sf::Window & window;
        util::Random & random;
        util::SoundPlayer & sfx;
        StateManager & state;
        FontManager & font;
        Avatar & avatar;
        AvatarTextureManager & avatar_texture;
        ScreenLayout & layout;
        LevelFileLoader & level_loader;
        Level & level;
        MapTextures & map_texture;
        BackgroundImages & bg_image;
        PickupAnimations & pickup;
        AccentAnimations & accent;
        SpellAnimations & spell;
        ItemImages & item;
        LevelInfo & level_info;
        PlayerInfo & player;
        PlayerInfoDisplay & player_display;
    };

} // namespace platformer

#endif // CONTEXT_HPP_INCLUDED
