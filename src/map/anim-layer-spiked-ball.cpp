// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-spiked-ball.cpp
//
#include "anim-layer-spiked-ball.hpp"

#include "avatar/avatar.hpp"
#include "bramblefore/settings.hpp"
#include "map/level.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    SpikedBallAnim::SpikedBallAnim(
        const sf::Texture & t_texture, const sf::FloatRect & t_rect, const float t_speed)
        : sprite{ t_texture }
        , region{ t_rect }
        , slider{}
    {
        util::setOriginToCenter(sprite);

        if (isHoriz())
        {
            sprite.setPosition(
                { region.position.x,
                  (util::center(region).y - (sprite.getGlobalBounds().size.y * 0.5f)) });

            slider = util::SliderOscillator<float, float>(0.0f, region.size.x, t_speed);
        }
        else
        {
            sprite.setPosition({
                (util::center(region).x - (sprite.getGlobalBounds().size.x * 0.5f)),
                region.position.y,
            });

            slider = util::SliderOscillator<float, float>(
                region.position.y, util::bottom(region), t_speed);
        }
    }

    //

    SpikedBallAnimationLayer::SpikedBallAnimationLayer(
        Context & t_context, const std::vector<sf::FloatRect> & t_rects)
        : m_speed{ 1.5f }
        , m_texture{}
        , m_anims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        util::TextureLoader::load(
            m_texture, (t_context.settings.media_path / "image/anim/spiked-ball.png"));

        m_anims.reserve(t_rects.size());
        for (const sf::FloatRect & rect : t_rects)
        {
            m_anims.emplace_back(m_texture, rect, m_speed);
        }
    }

    SpikedBallAnimationLayer::~SpikedBallAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void SpikedBallAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const SpikedBallAnim & anim : m_anims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void SpikedBallAnimationLayer::move(const Context &, const float t_amount)
    {
        for (SpikedBallAnim & anim : m_anims)
        {
            anim.sprite.move({ t_amount, 0.0f });
            anim.region.position.x += t_amount;
        }
    }

    void SpikedBallAnimationLayer::update(Context &, const float t_frameTimeSec)
    {
        for (SpikedBallAnim & anim : m_anims)
        {
            const float newPos{ anim.slider.update(t_frameTimeSec) };

            if (anim.isHoriz())
            {
                anim.sprite.setPosition(
                    { (anim.region.position.x + newPos), anim.sprite.getPosition().y });
            }
            else
            {
                anim.sprite.setPosition({ anim.sprite.getPosition().x, newPos });
            }
        }
    }

    Harm SpikedBallAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        for (const SpikedBallAnim & anim : m_anims)
        {
            const sf::FloatRect ballRect{ anim.sprite.getGlobalBounds() };
            if (t_avatarRect.findIntersection(ballRect))
            {
                harm.rect   = ballRect;
                harm.damage = 10;
                harm.sfx    = "";
                break;
            }
        }

        return harm;
    }

    void SpikedBallAnimationLayer::dumpInfo() const
    {
        std::clog << "\tSpiked Ball Animation Layer: x" << m_anims.size() << '\n';
    }

} // namespace bramblefore
