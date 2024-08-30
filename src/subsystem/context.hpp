#ifndef CONTEXT_HPP_INCLUDED
#define CONTEXT_HPP_INCLUDED
//
// context.hpp
//

namespace util
{
    class Random;
    // class MusicPlayer;
    class SoundPlayer;
} // namespace util

namespace bramblefore
{

    struct Level;
    struct Settings;

    class Avatar;
    class LevelInfo;
    class PlayerInfo;
    class ItemImages;
    class FontManager;
    class ScreenLayout;
    class StateManager;
    class FloatingText;
    class MapCoordinator;
    class SpellAnimations;
    class LevelFileLoader;
    class BackgroundImages;
    class PickupAnimations;
    class AccentAnimations;
    class PlayerInfoDisplay;

    //

    struct Context
    {
        Context(
            const Settings & set,
            util::Random & ran,
            util::SoundPlayer & spl,
            StateManager & sma,
            FontManager & fma,
            Avatar & ava,
            ScreenLayout & scl,
            LevelFileLoader & lfl,
            Level & lvl,
            BackgroundImages & bim,
            PickupAnimations & pan,
            AccentAnimations & aan,
            SpellAnimations & san,
            ItemImages & ima,
            LevelInfo & lin,
            PlayerInfo & pin,
            PlayerInfoDisplay & pid,
            FloatingText & flt,
            MapCoordinator & mco)
            : settings(set)
            , random(ran)
            , sfx(spl)
            , state(sma)
            , font(fma)
            , avatar(ava)
            , layout(scl)
            , level_loader(lfl)
            , level(lvl)
            , bg_image(bim)
            , pickup(pan)
            , accent(aan)
            , spell(san)
            , item(ima)
            , level_info(lin)
            , player(pin)
            , player_display(pid)
            , float_text(flt)
            , map_coord(mco)
        {}

        const Settings & settings;
        util::Random & random;
        util::SoundPlayer & sfx;
        StateManager & state;
        FontManager & font;
        Avatar & avatar;
        ScreenLayout & layout;
        LevelFileLoader & level_loader;
        Level & level;
        BackgroundImages & bg_image;
        PickupAnimations & pickup;
        AccentAnimations & accent;
        SpellAnimations & spell;
        ItemImages & item;
        LevelInfo & level_info;
        PlayerInfo & player;
        PlayerInfoDisplay & player_display;
        FloatingText & float_text;
        MapCoordinator & map_coord;
    };

} // namespace bramblefore

#endif // CONTEXT_HPP_INCLUDED
