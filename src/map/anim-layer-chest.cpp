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
#include "util/check-macros.hpp"
#include "util/random.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    ChestAnimationLayer::ChestAnimationLayer(Context & t_context)
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
        Context & t_context, const std::string & t_name, const sf::FloatRect & t_rect)
    {
        const Chest chest{ chestFromName(t_name) };

        M_CHECK(
            (Chest::Count != chest),
            "Error parsing map - Unknown chest name: \"" << t_name << "\"");

        const sf::Texture & texture{ m_textureSets.at(static_cast<std::size_t>(chest)).closed };

        ChestAnim & anim{ m_animations.emplace_back(texture, chest) };

        const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 2.5f) };
        anim.sprite.setScale({ scale, scale });

        anim.sprite.setPosition(
            { (util::center(t_rect).x - (anim.sprite.getGlobalBounds().size.x * 0.5f)),
              (util::bottom(t_rect) - (anim.sprite.getGlobalBounds().size.y) * 0.9f) });
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

    void ChestAnimationLayer::loadTextures(Context & t_context)
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

} // namespace bramblefore
