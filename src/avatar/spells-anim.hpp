#ifndef SPELLS_ANIM_HPP_INCLUDED
#define SPELLS_ANIM_HPP_INCLUDED
//
// spells-anim.hpp
//
#include "player/player-info.hpp"

#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RenderTarget;
} // namespace sf

namespace bramblefore
{
    struct Context;
    struct Settings;

    //

    enum class SpellFramePhase
    {
        Start,
        Flying,
        Finish
    };

    struct SpellFrames
    {
        SpellFrames()
            : start{}
            , flying{}
            , finish{}
        {}

        SpellFrames(
            const std::vector<std::size_t> & t_startFrames,
            const std::vector<std::size_t> & t_flyingFrames,
            const std::vector<std::size_t> & t_finishFrame)
            : start{ t_startFrames }
            , flying{ t_flyingFrames }
            , finish{ t_finishFrame }
        {}

        [[nodiscard]] constexpr bool willFly() const noexcept
        {
            return (!start.empty() && !flying.empty() && !finish.empty());
        }

        std::vector<std::size_t> start;
        std::vector<std::size_t> flying;
        std::vector<std::size_t> finish;
    };

    [[nodiscard]] inline const SpellFrames toFrames(const Spell t_spell) noexcept
    {
        if (Spell::Comet == t_spell)
        {
            return SpellFrames({ 0, 1, 2 }, { 3, 4, 5 }, { 6, 7, 8, 9, 10, 11, 12, 13 });
        }
        else if (Spell::Fire == t_spell)
        {
            return SpellFrames({ 0, 1, 2 }, { 3, 4, 5 }, { 6, 7, 8, 9 });
        }
        else if (Spell::Freeze == t_spell)
        {
            return SpellFrames({ 0, 1 }, { 2, 3, 4 }, { 5, 6, 7, 8, 9, 10, 11, 12 });
        }
        else if (Spell::Water == t_spell)
        {
            return SpellFrames({ 0, 1 }, { 2, 3, 4 }, { 5, 6, 7, 8, 9, 10 });
        }
        else
        {
            return SpellFrames();
        }
    }

    //

    struct SpellAnim
    {
        explicit SpellAnim(
            const Spell t_spell,
            const sf::Texture & t_texture,
            const float t_timePerFrameSec,
            const sf::Vector2f & t_scale,
            const sf::Vector2f & t_position,
            const bool t_isFacingRight);

        bool is_alive;
        Spell spell;
        std::size_t frame_index;
        float elapsed_time_sec;
        float time_per_frame_sec;
        bool is_facing_right;
        sf::Sprite sprite;
        SpellFrames frames;
        SpellFramePhase phase;
    };

    //

    struct SpellTextures
    {
        sf::Texture icon_texture{};
        std::vector<sf::Texture> textures{};
    };

    //

    class SpellAnimations
    {
      public:
        SpellAnimations();

        void setup(const Context & t_context);
        void add(const sf::Vector2f & t_pos, const Spell t_spell, const bool t_isFacingRight);
        void update(const Context & t_context, const float t_frameTimeSec);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        void move(const sf::Vector2f & t_move);

        [[nodiscard]] inline const sf::Texture & iconTexture(const Spell spell) const
        {
            return m_textureSets.at(static_cast<std::size_t>(spell)).icon_texture;
        }

        constexpr void clear() noexcept { m_anims.clear(); }

      private:
        void updateNonFlyingAnimation(
            const Context & t_context, const float t_frameTimeSec, SpellAnim & anim);

        void updateFlyingAnimation(
            const Context & t_context, const float t_frameTimeSec, SpellAnim & anim);

      private:
        std::vector<SpellTextures> m_textureSets;
        std::vector<SpellAnim> m_anims;
        sf::Vector2f m_scale;
    };

} // namespace bramblefore

#endif // SPELLS_ANIM_HPP_INCLUDED
