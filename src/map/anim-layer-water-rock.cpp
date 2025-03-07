// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-water-rock.cpp
//
#include "anim-layer-water-rock.hpp"

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

    WaterRockRect::WaterRockRect(const WaterRock t_rock, const sf::FloatRect & t_rect)
        : rock{ t_rock }
        , rect{ t_rect }
    {}

    //

    WaterRockAnim::WaterRockAnim(
        const WaterRock t_rock,
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const float t_timePerFrameSec,
        const float t_scale,
        const sf::FloatRect & t_screenRect)
        : rock{ t_rock }
        , sprite{ t_texture, t_textureRect }
        , elapsed_time_sec{ 0.0f }
        , time_per_frame_sec{ t_timePerFrameSec }
        , frame_index{ 0 }
    {
        sprite.setScale({ t_scale, t_scale });

        sprite.setPosition(
            { (util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
              (util::bottom(t_screenRect) - sprite.getGlobalBounds().size.y) });

        if (WaterRock::Flat2 == rock)
        {
            sprite.move({ 0.0f, (sprite.getGlobalBounds().size.y * 0.2f) });
        }
        else if (WaterRock::Medium1 == rock)
        {
            sprite.move({ 0.0f, (sprite.getGlobalBounds().size.y * 0.02f) });
        }
        else if (WaterRock::Pillar == rock)
        {
            sprite.move({ 0.0f, (sprite.getGlobalBounds().size.y * 0.125f) });
        }
    }

    //

    WaterRockAnimationLayer::WaterRockAnimationLayer(
        Context & t_context, const std::vector<WaterRockRect> & t_rockRects)
        : m_textures{}
        , m_rockAnims{}
    {
        m_textures.resize(static_cast<std::size_t>(WaterRock::Count));
        for (std::size_t index{ 0 }; index < static_cast<std::size_t>(WaterRock::Count); ++index)
        {
            std::string pathStr{
                (t_context.settings.media_path / "image/anim/water-rock-").string()
            };

            pathStr += toString(static_cast<WaterRock>(index));
            pathStr += ".png";

            util::TextureLoader::load(m_textures.at(index), pathStr);
        }

        //

        const float rockScale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.5f) };

        m_rockAnims.reserve(t_rockRects.size());
        for (const WaterRockRect & rockRect : t_rockRects)
        {
            WaterRockAnim anim{ m_rockAnims.emplace_back(
                rockRect.rock,
                getTexture(rockRect.rock),
                textureRect(rockRect.rock, 0),
                t_context.random.fromTo(0.5f, 1.25f),
                rockScale,
                rockRect.rect) };

            sf::FloatRect collRect{ anim.sprite.getGlobalBounds() };
            if (rockRect.rock == WaterRock::Flat1)
            {
                util::scaleRectInPlace(collRect, { 0.85f, 0.25f });
            }
            else if (rockRect.rock == WaterRock::Flat2)
            {
                util::scaleRectInPlace(collRect, { 0.675f, 0.2f });
            }
            else if (rockRect.rock == WaterRock::Medium1)
            {
                util::scaleRectInPlace(collRect, { 0.525f, 0.5f });
            }
            else if (rockRect.rock == WaterRock::Medium2)
            {
                util::scaleRectInPlace(collRect, { 0.6f, 0.75f });
            }
            else if (rockRect.rock == WaterRock::Pillar)
            {
                util::scaleRectInPlace(collRect, { 0.15f, 0.55f });
            }

            t_context.level.layer_collisions.push_back(collRect);
        }
    }

    WaterRockAnimationLayer::~WaterRockAnimationLayer() {}

    void WaterRockAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const WaterRockAnim & anim : m_rockAnims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void WaterRockAnimationLayer::move(const Context &, const float t_amount)
    {
        for (WaterRockAnim & anim : m_rockAnims)
        {
            anim.sprite.move({ t_amount, 0.0f });
        }
    }

    const sf::Texture & WaterRockAnimationLayer::getTexture(const WaterRock t_rock) const
    {
        const std::size_t textureIndex{ static_cast<std::size_t>(t_rock) };

        M_CHECK(
            (textureIndex < m_textures.size()),
            "textureIndex=" << textureIndex << " >= m_textures.size()=" << m_textures.size());

        return m_textures.at(textureIndex);
    }

    std::size_t WaterRockAnimationLayer::frameCount(const WaterRock t_rock) const noexcept
    {
        const sf::Texture & texture{ getTexture(t_rock) };

        if (texture.getSize().y > 0)
        {
            return static_cast<std::size_t>(texture.getSize().x / texture.getSize().y);
        }
        else
        {
            return 0;
        }
    }

    sf::IntRect WaterRockAnimationLayer::textureRect(
        const WaterRock t_rock, const std::size_t frame) const noexcept
    {
        const sf::Texture & texture{ getTexture(t_rock) };

        sf::IntRect rect;
        rect.size.x     = static_cast<int>(texture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(frame) * rect.size.x);

        return rect;
    }

    void WaterRockAnimationLayer::update(Context &, const float t_frameTimeSec)
    {
        for (WaterRockAnim & anim : m_rockAnims)
        {
            anim.elapsed_time_sec += t_frameTimeSec;
            if (anim.elapsed_time_sec > anim.time_per_frame_sec)
            {
                anim.elapsed_time_sec -= anim.time_per_frame_sec;

                ++anim.frame_index;
                if (anim.frame_index >= frameCount(anim.rock))
                {
                    anim.frame_index = 0;
                }

                anim.sprite.setTextureRect(textureRect(anim.rock, anim.frame_index));
            }
        }
    }

    void WaterRockAnimationLayer::dumpInfo() const
    {
        std::clog << "\tWater Rock Animation Layer: x" << m_rockAnims.size() << '\n';
    }

} // namespace bramblefore
