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

    class Level;
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
    class BloodSplatManager;

    //

    struct Context
    {
        Context(
            const Settings & t_settings,
            util::Random & t_random,
            util::SoundPlayer & t_soundPlayer,
            StateManager & t_stateManager,
            FontManager & t_fontManager,
            Avatar & t_avatar,
            ScreenLayout & t_screenLayout,
            LevelFileLoader & t_levelFileLoader,
            Level & t_level,
            BackgroundImages & t_backgroundImages,
            PickupAnimations & t_pickupAnimations,
            AccentAnimations & t_accentAnimations,
            SpellAnimations & t_spellAnimations,
            ItemImages & t_itemImages,
            LevelInfo & t_levelInfo,
            PlayerInfo & t_playerInfo,
            PlayerInfoDisplay & t_playerInfoDisplay,
            FloatingText & t_floatingText,
            MapCoordinator & t_mapCoordinator,
            BloodSplatManager & t_bloodSplatManager)
            : settings{ t_settings }
            , random{ t_random }
            , sfx{ t_soundPlayer }
            , state{ t_stateManager }
            , font{ t_fontManager }
            , avatar{ t_avatar }
            , layout{ t_screenLayout }
            , level_loader{ t_levelFileLoader }
            , level{ t_level }
            , bg_image{ t_backgroundImages }
            , pickup{ t_pickupAnimations }
            , accent{ t_accentAnimations }
            , spell{ t_spellAnimations }
            , item{ t_itemImages }
            , level_info{ t_levelInfo }
            , player{ t_playerInfo }
            , player_display{ t_playerInfoDisplay }
            , float_text{ t_floatingText }
            , map_coord{ t_mapCoordinator }
            , blood_splat{ t_bloodSplatManager }
        {}

        const Settings & settings; // TODO no plurals, no biscuit...
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
        BloodSplatManager & blood_splat;
    };

} // namespace bramblefore

#endif // CONTEXT_HPP_INCLUDED
