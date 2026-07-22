//
// blood-splat.cpp
//
#include "blood-splat.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

namespace bramblefore
{

    BloodSplatAnim::BloodSplatAnim(
        const Context & t_context,
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const sf::Vector2f & t_position)
        : is_alive{ true }
        , sprite(t_texture, t_textureRect)
        , elapsed_sec{ 0.0f }
        , duration_sec{ t_context.random.fromTo(1.0f, 2.0f) }
    {
        sprite.setColor(sf::Color(255, 0, 0));
        util::setOriginToCenter(sprite);
        sprite.setPosition(t_position);
        sprite.setRotation(sf::degrees(t_context.random.fromTo(0.0f, 360.0f)));

        // calc the scale
        const float scaleOrig{ t_context.random.fromTo(0.1f, 0.25f) };

        const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, scaleOrig) *
                           t_context.settings.map_scale };

        sprite.scale({ scale, scale });
    }

    //

    BloodSplatManager::BloodSplatManager()
        : m_texture{}
        , m_anims{}
    {
        m_anims.reserve(8); // just a harmless guess
    }

    void BloodSplatManager::setup(const Context & t_context)
    {
        util::TextureLoader::load(
            m_texture, t_context.settings.media_path / "image" / "particle" / "splat.png");
    }

    void BloodSplatManager::update(const float t_elapsedTimeSec)
    {
        bool anyToRemove{ false };

        for (BloodSplatAnim & anim : m_anims)
        {
            anim.elapsed_sec += t_elapsedTimeSec;
            if (anim.elapsed_sec < anim.duration_sec)
            {
                sf::Color color{ anim.sprite.getColor() };

                color.a = static_cast<uint8_t>(
                    255 - util::map(anim.elapsed_sec, 0.0f, anim.duration_sec, 0, 255));

                anim.sprite.setColor(color);
            }
            else
            {
                anim.elapsed_sec -= anim.duration_sec;
                anim.is_alive = false;
                anyToRemove   = true;
            }
        }

        if (anyToRemove)
        {
            std::erase_if(m_anims, [](const BloodSplatAnim & anim) { return !anim.is_alive; });
        }
    }

    void BloodSplatManager::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const BloodSplatAnim & anim : m_anims)
        {
            // no way this will ever be offscreen, so don't check before drawing
            t_target.draw(anim.sprite, t_states);
        }
    }

    void BloodSplatManager::add(const Context & t_context, const sf::Vector2f & t_position)
    {
        m_anims.emplace_back(t_context, m_texture, getRandomTextureRect(t_context), t_position);
    }

    void BloodSplatManager::move(const sf::Vector2f & t_move)
    {
        for (BloodSplatAnim & anim : m_anims)
        {
            anim.sprite.move(t_move);
        }
    }

    const sf::IntRect BloodSplatManager::getRandomTextureRect(const Context & t_context)
    {
        // there are four blood spalt images in 2x2 formation in the texture
        const sf::Vector2i size{ m_texture.getSize() / 2u };

        const int which{ t_context.random.fromTo(1, 4) };
        if (1 == which)
        {
            return { { 0, 0 }, size };
        }
        else if (2 == which)
        {
            return { { size.x, 0 }, size };
        }
        else if (3 == which)
        {
            return { { 0, size.y }, size };
        }
        else
        {
            return { size, size };
        }
    }

} // namespace bramblefore
