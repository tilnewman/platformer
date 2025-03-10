// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-lava.cpp
//
#include "anim-layer-lava.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    LavaAnimationLayer::LavaAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_texture{}
        , m_sprites{}
        , m_elapsedTimeSec{ 0.0f }
        , m_timePerFrameSec{ 0.2f }
        , m_frameIndex{ 0 }
    {
        HarmCollisionManager::instance().addOwner(*this);

        util::TextureLoader::load(
            m_texture, (t_context.settings.media_path / "image/anim/lava-surface.png"));

        m_sprites.reserve(t_rects.size());
        for (const sf::FloatRect & rect : t_rects)
        {
            sf::Sprite & sprite{ m_sprites.emplace_back(m_texture) };
            sprite.setTextureRect(textureRect(0));
            util::fitAndCenterInside(sprite, rect);
        }
    }

    LavaAnimationLayer::~LavaAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void LavaAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const sf::Sprite & sprite : m_sprites)
        {
            if (wholeScreenRect.findIntersection(sprite.getGlobalBounds()))
            {
                t_target.draw(sprite, t_states);
            }
        }
    }

    void LavaAnimationLayer::move(const Context &, const float t_amount)
    {
        for (sf::Sprite & sprite : m_sprites)
        {
            sprite.move({ t_amount, 0.0f });
        }
    }

    std::size_t LavaAnimationLayer::frameCount() const noexcept
    {
        if (m_texture.getSize().y > 0)
        {
            return static_cast<std::size_t>(m_texture.getSize().x / m_texture.getSize().y);
        }
        else
        {
            return 0;
        }
    }

    sf::IntRect LavaAnimationLayer::textureRect(const std::size_t frame) const noexcept
    {
        sf::IntRect rect;
        rect.size.x     = static_cast<int>(m_texture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(frame) * rect.size.x);
        return rect;
    }

    void LavaAnimationLayer::update(Context &, const float t_frameTimeSec)
    {
        m_elapsedTimeSec += t_frameTimeSec;
        if (m_elapsedTimeSec > m_timePerFrameSec)
        {
            m_elapsedTimeSec -= m_timePerFrameSec;

            ++m_frameIndex;
            if (m_frameIndex >= frameCount())
            {
                m_frameIndex = 0;
            }

            for (sf::Sprite & sprite : m_sprites)
            {
                sprite.setTextureRect(textureRect(m_frameIndex));
            }
        }
    }

    Harm LavaAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        for (const sf::Sprite & sprite : m_sprites)
        {
            const sf::FloatRect acidRect{ sprite.getGlobalBounds() };
            if (t_avatarRect.findIntersection(acidRect))
            {
                harm.rect   = acidRect;
                harm.damage = 99999; // TOOD lookup real max health somewhere
                harm.sfx    = "acid";
                break;
            }
        }

        return harm;
    }

    void LavaAnimationLayer::dumpInfo() const
    {
        std::clog << "\tLava Animation Layer: x" << m_sprites.size() << '\n';
    }

} // namespace bramblefore
