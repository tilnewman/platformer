// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// anim-layer-chest.cpp
//
#include "anim-layer-chest.hpp"

#include "bramblefore/settings.hpp"
#include "player/player-info.hpp"
#include "subsystem/context.hpp"
#include "subsystem/floating-text.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    ChestAnim::ChestAnim(
        const sf::Texture & t_texture,
        const Chest t_chest,
        const float t_scale,
        const sf::FloatRect & t_screenRect)
        : chest{ t_chest }
        , is_open{ false }
        , sprite{ t_texture }
    {
        sprite.setScale({ t_scale, t_scale });

        sprite.setPosition(
            { (util::center(t_screenRect).x - (sprite.getGlobalBounds().size.x * 0.5f)),
              (util::bottom(t_screenRect) - (sprite.getGlobalBounds().size.y) * 0.9f) });
    }

    //

    ChestAnimationLayer::ChestAnimationLayer(const Context & t_context)
        : m_textureSets{}
        , m_animations{}
    {
        HarmCollisionManager::instance().addOwner(*this);
        loadTextures(t_context);
    }

    ChestAnimationLayer::~ChestAnimationLayer()
    {
        HarmCollisionManager::instance().removeOwner(*this);
    }

    void ChestAnimationLayer::add(
       const Context & t_context, const Chest t_chest, const sf::FloatRect & t_rect)
    {
        m_animations.emplace_back(
            m_textureSets.at(static_cast<std::size_t>(t_chest)).closed,
            t_chest,
            t_context.layout.calScaleBasedOnResolution(t_context, 2.5f),
            t_rect);
    }

    void ChestAnimationLayer::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeScreenRect{ t_context.layout.wholeRect() };

        for (const ChestAnim & anim : m_animations)
        {
            if (wholeScreenRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                t_target.draw(anim.sprite, t_states);
            }
        }
    }

    void ChestAnimationLayer::move(const Context &, const float t_amount)
    {
        for (ChestAnim & anim : m_animations)
        {
            anim.sprite.move({ t_amount, 0.0f });
        }
    }

    Harm ChestAnimationLayer::avatarCollide(Context & t_context, const sf::FloatRect & t_avatarRect)
    {
        for (ChestAnim & anim : m_animations)
        {
            if (!anim.is_open && t_avatarRect.findIntersection(anim.sprite.getGlobalBounds()))
            {
                // chests don't really harm anyone
                // i'm just using all this as a trigger to open the chests
                t_context.sfx.play("chest-open");
                anim.is_open = true;

                const sf::Texture & texture{
                    m_textureSets.at(static_cast<std::size_t>(anim.chest)).open
                };

                anim.sprite.setTexture(texture);

                // calculating coin count will have to become more complicated later
                const Coin_t coinCount{ t_context.random.fromTo(20, 40) };

                std::string coinString{ "+" };
                coinString += std::to_string(coinCount);
                coinString += " coins";

                t_context.float_text.add(
                    t_context, coinString, sf::Color(255, 215, 0), util::center(anim.sprite));

                t_context.player.coinAdjust(t_context, coinCount);
                break;
            }
        }

        return Harm{};
    }

    void ChestAnimationLayer::loadTextures(const Context & t_context)
    {
        // this reserve call needed to prevent reallocations
        m_textureSets.reserve(static_cast<std::size_t>(Chest::Count));

        for (std::size_t chestIndex{ 1 }; chestIndex <= static_cast<std::size_t>(Chest::Count);
             ++chestIndex)
        {
            ChestTextures & textures{ m_textureSets.emplace_back() };

            util::TextureLoader::load(
                textures.open,
                (t_context.settings.media_path / std::string("image/anim/chest")
                                                     .append(std::to_string(chestIndex))
                                                     .append("-open.png")));

            util::TextureLoader::load(
                textures.closed,
                (t_context.settings.media_path / std::string("image/anim/chest")
                                                     .append(std::to_string(chestIndex))
                                                     .append(".png")));
        }
    }

    void ChestAnimationLayer::dumpInfo() const
    {
        std::clog << "\tChest Animation Layer: x" << m_animations.size() << '\n';
    }

} // namespace bramblefore
