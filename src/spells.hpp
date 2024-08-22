#ifndef SPELLS_HPP_INCLUDED
#define SPELLS_HPP_INCLUDED
//
// spells.hpp
//
#include <string_view>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace platformer
{
    struct Context;
    struct Settings;

    //

    enum class Spell : std::size_t
    {
        Comet = 0,
        Explosion,
        Fire,
        Freeze,
        Gypno,
        KillAll,
        Light,
        Lightning1,
        Lightning2,
        MidasHand,
        Spikes1,
        Spikes2,
        SunStrike,
        TeslaBall,
        Tornado,
        Water,
        Count
    };

    inline constexpr std::string_view toName(const Spell spell)
    {
        // clang-format off
        switch (spell)
        {
            case Spell::Comet:      { return "comet"; }
            case Spell::Explosion:  { return "explosion"; }
            case Spell::Fire:       { return "fire"; }
            case Spell::Freeze:     { return "freeze"; }
            case Spell::Gypno:      { return "gypno"; }
            case Spell::KillAll:    { return "kill-all"; }
            case Spell::Light:      { return "light"; }
            case Spell::Lightning1: { return "lightning1"; }
            case Spell::Lightning2: { return "lightning2"; }
            case Spell::MidasHand:  { return "midas-hand"; }
            case Spell::Spikes1:    { return "spikes1"; }
            case Spell::Spikes2:    { return "spikes2"; }
            case Spell::SunStrike:  { return "sun-strike"; }
            case Spell::TeslaBall:  { return "tesla-ball"; }
            case Spell::Tornado:    { return "tornado"; }
            case Spell::Water:      { return "water"; }
            case Spell::Count:      //intentional fallthrough
            default:                { return "Error_Spell_unknown";    }
        }
        // clang-format on
    }

    //

    struct SpellAnim
    {
        bool is_alive{ true };
        Spell which{ Spell::Comet }; // any spell works here
        std::size_t anim_index{ 0 };
        sf::Sprite sprite{};
    };

    //

    class SpellAnimations
    {
      public:
        SpellAnimations();

        void setup(const Settings & settings);
        void add(const Context & context, const sf::Vector2f & pos, const Spell spell);
        void update(Context & context, const float frameTimeSec);
        void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const;
        const sf::Texture iconTexture(const Spell spell) const;
        inline void clear() { m_anims.clear(); }

      private:
        std::size_t frameCount(const Spell which) const;
        sf::IntRect textureRect(const Spell which, const std::size_t frame) const;

      private:
        std::vector<sf::Texture> m_textures;
        std::vector<sf::Texture> m_iconTextures;
        std::vector<SpellAnim> m_anims;
        float m_elapsedTimeSec;
        float m_timePerFrameSec;
        sf::Vector2f m_scale;
    };

} // namespace platformer

#endif // SPELLS_HPP_INCLUDED
