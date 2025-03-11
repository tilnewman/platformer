// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-water.cpp
//
#include "anim-layer-water.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    WaterTypeRect::WaterTypeRect(const bool t_isSurface, const sf::FloatRect & t_rect)
        : is_surface{ t_isSurface }
        , rect{ t_rect }
    {}

    //

    WaterAnim::WaterAnim(
        const WaterType t_type,
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const float t_timePerFrameSec,
        const sf::FloatRect & t_screenRect)
        : type{ t_type }
        , sprite{ t_texture, t_textureRect }
        , elapsed_time_sec{ 0.0f }
        , time_per_frame_sec{ t_timePerFrameSec }
        , frame_index{ 0 }
    {
        util::fitAndCenterInside(sprite, t_screenRect);
    }

    //

    WaterAnimationLayer::WaterAnimationLayer(
        const Context & t_context, const std::vector<WaterTypeRect> & t_typeRects)
        : m_textures{}
        , m_anims{}
    {
        HarmCollisionManager::instance().addOwner(*this);

        //

        m_textures.resize(static_cast<std::size_t>(WaterType::Count));
        for (std::size_t index{ 0 }; index < static_cast<std::size_t>(WaterType::Count); ++index)
        {
            util::TextureLoader::load(
                m_textures.at(index),
                (t_context.settings.media_path / "image/anim" /
                 toFilename(static_cast<WaterType>(index))));
        }

        //

        m_anims.reserve(t_typeRects.size());
        for (const WaterTypeRect & typeRect : t_typeRects)
        {
            const WaterType type = [&]() {
                if (typeRect.is_surface)
                {
                    if (t_context.random.boolean())
                    {
                        return WaterType::Surface1;
                    }
                    else
                    {
                        return WaterType::Surface2;
                    }
                }
                else
                {
                    if (t_context.random.boolean())
                    {
                        return WaterType::Basin1;
                    }
                    else
                    {
                        return WaterType::Basin2;
                    }
                }
            }();

            m_anims.emplace_back(
                type,
                getTexture(type),
                textureRect(type, 0),
                t_context.random.fromTo(0.3f, 2.0f),
                typeRect.rect);
        }
    }

    WaterAnimationLayer::~WaterAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void WaterAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const WaterAnim & anim : m_anims)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void WaterAnimationLayer::move(const Context &, const float amount)
    {
        for (WaterAnim & anim : m_anims)
        {
            anim.sprite.move({ amount, 0.0f });
        }
    }

    const sf::Texture & WaterAnimationLayer::getTexture(const WaterType t_type) const
    {
        const std::size_t typeIndex{ static_cast<std::size_t>(t_type) };

        M_CHECK(
            (typeIndex < m_textures.size()),
            "typeIndex=" << typeIndex << " >= texture.size()=" << m_textures.size());

        return m_textures.at(typeIndex);
    }

    std::size_t WaterAnimationLayer::frameCount(const WaterType t_type) const
    {
        const sf::Texture & texture{ getTexture(t_type) };

        if (texture.getSize().y > 0)
        {
            return static_cast<std::size_t>(texture.getSize().x / texture.getSize().y);
        }
        else
        {
            return 0;
        }
    }

    sf::IntRect
        WaterAnimationLayer::textureRect(const WaterType t_type, const std::size_t t_frame) const
    {
        const sf::Texture & texture{ getTexture(t_type) };

        sf::IntRect rect;
        rect.size.x     = static_cast<int>(texture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(t_frame) * rect.size.x);

        return rect;
    }

    void WaterAnimationLayer::update(Context &, const float t_frameTimeSec)
    {
        for (WaterAnim & anim : m_anims)
        {
            anim.elapsed_time_sec += t_frameTimeSec;
            if (anim.elapsed_time_sec > anim.time_per_frame_sec)
            {
                anim.elapsed_time_sec -= anim.time_per_frame_sec;

                ++anim.frame_index;
                if (anim.frame_index >= frameCount(anim.type))
                {
                    anim.frame_index = 0;
                }

                anim.sprite.setTextureRect(textureRect(anim.type, anim.frame_index));
            }
        }
    }

    Harm WaterAnimationLayer::avatarCollide(Context &, const sf::FloatRect & t_avatarRect)
    {
        Harm harm;

        for (const WaterAnim & anim : m_anims)
        {
            const sf::FloatRect waterRect{ anim.sprite.getGlobalBounds() };
            if (t_avatarRect.findIntersection(waterRect))
            {
                harm.rect   = waterRect;
                harm.damage = 0;
                harm.sfx    = "dunk";

                break;
            }
        }

        return harm;
    }

    void WaterAnimationLayer::dumpInfo() const
    {
        std::clog << "\tWater Animation Layer: x" << m_anims.size() << '\n';
    }

} // namespace bramblefore
