// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// spells.cpp
//
#include "spells.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "random.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

namespace platformer
{

    SpellAnimations::SpellAnimations()
        : m_textures()
        , m_iconTextures()
        , m_anims()
        , m_elapsedTimeSec(0.0f)
        , m_timePerFrameSec(0.0f)
        , m_scale(0.0f, 0.0f)
    {
        m_anims.reserve(256);
    }

    void SpellAnimations::setup(const Settings & settings)
    {
        m_scale.x = settings.spell_scale;
        m_scale.y = settings.spell_scale;

        m_timePerFrameSec = settings.spell_time_per_frame;

        m_textures.reserve(static_cast<std::size_t>(Spell::Count));
        m_iconTextures.reserve(static_cast<std::size_t>(Spell::Count));

        for (std::size_t i(0); i < static_cast<std::size_t>(Spell::Count); ++i)
        {
            const Spell spell{ static_cast<Spell>(i) };

            sf::Texture & texture{ m_textures.emplace_back() };

            texture.loadFromFile((settings.media_path / "image/spell-anim" /
                                  std::string(toName(spell)).append(".png"))
                                     .string());

            TextureStats::instance().process(texture);

            texture.setSmooth(true);

            sf::Texture & iconTexture{ m_iconTextures.emplace_back() };

            iconTexture.loadFromFile((settings.media_path / "image/spell-anim" /
                                      std::string(toName(spell)).append("-icon.png"))
                                         .string());

            TextureStats::instance().process(iconTexture);

            iconTexture.setSmooth(true);
        }
    }

    void SpellAnimations::add(const Context & context, const sf::Vector2f & pos, const Spell spell)
    {
        if (Spell::Count == spell)
        {
            std::cout << "Error: SpellAnimations::add() given an unknown spell.\n";
            return;
        }

        SpellAnim & anim{ m_anims.emplace_back() };
        anim.is_alive   = true;
        anim.which      = spell;
        anim.anim_index = context.random.zeroToOneLessThan(frameCount(anim.which));
        anim.sprite.setTexture(m_textures.at(static_cast<std::size_t>(spell)), true);
        anim.sprite.setTextureRect(textureRect(spell, 0));
        anim.sprite.setScale(m_scale);
        util::setOriginToCenter(anim.sprite);
        anim.sprite.setPosition(pos);
    }

    std::size_t SpellAnimations::frameCount(const Spell which) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(which)) };
        return static_cast<std::size_t>(texture.getSize().x / texture.getSize().y);
    }

    sf::IntRect SpellAnimations::textureRect(const Spell which, const std::size_t frame) const
    {
        const sf::Texture & texture{ m_textures.at(static_cast<std::size_t>(which)) };

        sf::IntRect rect;
        rect.width  = static_cast<int>(texture.getSize().y);
        rect.height = rect.width;
        rect.top    = 0;
        rect.left   = (static_cast<int>(frame) * rect.width);

        return rect;
    }

    void SpellAnimations::update(Context &, const float frameTimeSec)
    {
        m_elapsedTimeSec += frameTimeSec;
        if (m_elapsedTimeSec > m_timePerFrameSec)
        {
            m_elapsedTimeSec -= m_timePerFrameSec;

            bool areAnyFinished = false;
            for (SpellAnim & anim : m_anims)
            {
                ++anim.anim_index;
                if (anim.anim_index >= frameCount(anim.which))
                {
                    anim.anim_index = 0;
                    areAnyFinished  = true;
                    anim.is_alive   = false;
                }

                anim.sprite.setTextureRect(textureRect(anim.which, anim.anim_index));
            }

            if (areAnyFinished)
            {
                m_anims.erase(
                    std::remove_if(
                        std::begin(m_anims),
                        std::end(m_anims),
                        [](const SpellAnim & anim) { return !anim.is_alive; }),
                    std::end(m_anims));
            }
        }
    }

    void SpellAnimations::draw(
        const Context & context, sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const SpellAnim & anim : m_anims)
        {
            if (context.layout.wholeRect().intersects(anim.sprite.getGlobalBounds()))
            {
                target.draw(anim.sprite, states);
            }
        }
    }

    const sf::Texture SpellAnimations::iconTexture(const Spell spell) const
    {
        return m_iconTextures.at(static_cast<std::size_t>(spell));
    }

} // namespace platformer
