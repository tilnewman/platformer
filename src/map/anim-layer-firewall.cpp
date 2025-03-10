// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-firewall.cpp
//
#include "anim-layer-firewall.hpp"

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

    FirewallAnimationLayer::FirewallAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_textures{}
        , m_sprites{}
        , m_elapsedTimeSec{ 0.0f }
        , m_timePerFrameSec{ 0.1f }
        , m_frameIndex{ 0 }
    {
        HarmCollisionManager::instance().addOwner(*this);

        //

        m_textures.resize(4); // prevent reallocations

        util::TextureLoader::load(
            m_textures.at(0), (t_context.settings.media_path / "image/anim/firewall-1-center.png"));

        util::TextureLoader::load(
            m_textures.at(1), (t_context.settings.media_path / "image/anim/firewall-2-center.png"));

        util::TextureLoader::load(
            m_textures.at(2), (t_context.settings.media_path / "image/anim/firewall-3-center.png"));

        util::TextureLoader::load(
            m_textures.at(3), (t_context.settings.media_path / "image/anim/firewall-4-center.png"));

        //

        m_sprites.reserve(t_rects.size());

        for (const sf::FloatRect & rect : t_rects)
        {
            sf::Sprite & sprite{ m_sprites.emplace_back(m_textures.at(0)) };
            util::fitAndCenterInside(sprite, rect);
        }
    }

    FirewallAnimationLayer::~FirewallAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void FirewallAnimationLayer::draw(
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

    void FirewallAnimationLayer::move(const Context &, const float t_amount)
    {
        for (sf::Sprite & sprite : m_sprites)
        {
            sprite.move({ t_amount, 0.0f });
        }
    }

    void FirewallAnimationLayer::update(Context &, const float t_frameTimeSec)
    {
        m_elapsedTimeSec += t_frameTimeSec;
        if (m_elapsedTimeSec > m_timePerFrameSec)
        {
            m_elapsedTimeSec -= m_timePerFrameSec;

            ++m_frameIndex;
            if (m_frameIndex >= 4)
            {
                m_frameIndex = 0;
            }

            for (sf::Sprite & sprite : m_sprites)
            {
                sprite.setTexture(m_textures.at(m_frameIndex));
            }
        }
    }

    Harm FirewallAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        for (const sf::Sprite & sprite : m_sprites)
        {
            const sf::FloatRect fireRect{ sprite.getGlobalBounds() };
            if (t_avatarRect.findIntersection(fireRect))
            {
                harm.rect   = fireRect;
                harm.damage = 99999; // TOOD lookup real max health somewhere
                harm.sfx    = "fire-puff";
                break;
            }
        }

        return harm;
    }

    void FirewallAnimationLayer::dumpInfo() const
    {
        std::clog << "\tFirewall Animation Layer: x" << m_sprites.size() << '\n';
    }

} // namespace bramblefore
