// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-saw.cpp
//
#include "anim-layer-saw.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "map/level.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "subsystem/texture-stats.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    SawAnimationLayer::SawAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_texture{}
        , m_anims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        m_texture.loadFromFile((t_context.settings.media_path / "image/map-anim/saw.png").string());

        TextureStats::instance().process(m_texture);

        for (const sf::FloatRect & rect : t_rects)
        {
            sf::Sprite & sprite{ m_anims.emplace_back() };
            sprite.setTexture(m_texture);
            util::setOriginToCenter(sprite);
            sprite.setPosition(util::center(rect));

            const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 0.5f) };
            sprite.scale(scale, scale);
        }
    }

    SawAnimationLayer::~SawAnimationLayer() { HarmCollisionManager::instance().removeOwner(*this); }

    void SawAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const sf::Sprite & sprite : m_anims)
        {
            if (wholeScreenRect.intersects(sprite.getGlobalBounds()))
            {
                t_target.draw(sprite, t_states);
            }
        }
    }

    void SawAnimationLayer::move(const Context &, const float t_amount)
    {
        for (sf::Sprite & sprite : m_anims)
        {
            sprite.move(t_amount, 0.0f);
        }
    }

    void SawAnimationLayer::update(Context &, const float t_frameTimeSec)
    {
        for (sf::Sprite & sprite : m_anims)
        {
            sprite.rotate(t_frameTimeSec * -80.0f);
        }
    }

    Harm SawAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        const sf::Vector2f avatarCenter{ util::center(t_avatarRect) };

        Harm harm;

        for (sf::Sprite & sprite : m_anims)
        {
            const sf::FloatRect globalBounds{ sprite.getGlobalBounds() };

            if (util::distance(util::center(globalBounds), avatarCenter) <
                (globalBounds.width * 0.5f))
            {
                harm.rect   = globalBounds;
                harm.damage = 16;
                harm.sfx    = "";
                break;
            }
        }

        return harm;
    }

} // namespace bramblefore
