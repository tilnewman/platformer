// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// avatar.hpp
//
#include "avatar.hpp"

#include "context.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"

#include <filesystem>

namespace platformer
{

    Avatar::Avatar()
        : m_sprite()
        , m_type(AvatarType::Assassin) // anything works here
        , m_anim(AvatarAnim::Idle)
        , m_elapsedTimeSec(0.0f)
        , m_animIndex(0)
        , m_velocity()
    {}

    void Avatar::setup(const Context & context, const AvatarType & type)
    {
        m_type = type;
        m_anim = AvatarAnim::Idle;
        m_sprite.setTexture(context.avatar_textures.get(m_type, m_anim).textures.at(0), true);
        m_sprite.setScale(context.settings.avatar_scale, context.settings.avatar_scale);
    }

    void Avatar::update(Context & context, const float frameTimeSec)
    {
        const AnimTextures & textureSet{ context.avatar_textures.get(m_type, m_anim) };

        m_elapsedTimeSec += frameTimeSec;
        if (m_elapsedTimeSec > textureSet.time_per_frame_sec)
        {
            m_elapsedTimeSec -= textureSet.time_per_frame_sec;

            ++m_animIndex;
            if (m_animIndex >= textureSet.textures.size())
            {
                m_animIndex = 0;
            }

            if (textureSet.textures.empty())
            {
                m_sprite.setTexture(context.avatar_textures.getDefault(m_type), true);
            }
            else
            {
                m_sprite.setTexture(textureSet.textures.at(m_animIndex), true);
            }
        }
    }

    void Avatar::draw(sf::RenderTarget & target, sf::RenderStates states)
    {
        target.draw(m_sprite, states);
        util::drawRectangleShape(target, collisionRect(), false, sf::Color::Red);
    }

    void Avatar::setPosition(const sf::FloatRect & rect)
    {
        m_sprite.setPosition(util::center(rect).x, util::bottom(rect));
        m_sprite.move(0.0f, (-110.0f * m_sprite.getScale().y));
    }

    void Avatar::cycleType()
    {
        std::size_t index{ static_cast<std::size_t>(m_type) };

        ++index;
        if (index >= static_cast<std::size_t>(AvatarType::Count))
        {
            index = 0;
        }

        m_type = static_cast<AvatarType>(index);
    }

    void Avatar::cycleAnim()
    {
        std::size_t index{ static_cast<std::size_t>(m_anim) };

        ++index;
        if (index >= static_cast<std::size_t>(AvatarAnim::Count))
        {
            index = 0;
        }

        m_anim = static_cast<AvatarAnim>(index);
    }

    const sf::FloatRect Avatar::collisionRect() const
    {
        const sf::FloatRect bounds{ m_sprite.getGlobalBounds() };
        sf::FloatRect rect{ bounds };
        util::scaleRectInPlace(rect, { 0.25f, 0.35f });
        rect.left -= (bounds.width * 0.15f);
        rect.top += (bounds.width * 0.175f);
        return rect;
    }

} // namespace platformer
