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
    }

} // namespace platformer
