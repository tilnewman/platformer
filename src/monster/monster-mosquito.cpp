//
// monster-mosquito.cpp
//
#include "monster-mosquito.hpp"

#include "bramblefore/settings.hpp"
#include "map/level.hpp"
#include "monster/monster-manager.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/filesystem-util.hpp"
#include "util/random.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

#include <filesystem>

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    Mosquito::Mosquito(const Context & t_context, const sf::FloatRect & t_region)
        : m_anim{ MosquitoAnim::Idle }
        , m_animTextures{}
        , m_sprite{ util::SfmlDefaults::instance().texture() }
        , m_elapsedTimeSec{ 0.0f }
        , m_flyBounds{ t_region }
        , m_spawnPosition{}
    {
        // pick a random spawn postion in the top half
        m_spawnPosition.x =
            t_context.random.fromTo(m_flyBounds.position.x, util::right(m_flyBounds));

        m_spawnPosition.y = t_context.random.fromTo(
            m_flyBounds.position.y, (m_flyBounds.position.y + (m_flyBounds.size.y * 0.5f)));
    }

    Mosquito::~Mosquito() {}

    void Mosquito::setup(const Context & t_context)
    {
        // load all textures
        // exact size to prevent re-allocations
        m_animTextures.reserve(static_cast<std::size_t>(MosquitoAnim::Count));

        for (std::size_t i{ 0 }; i < static_cast<std::size_t>(MosquitoAnim::Count); ++i)
        {
            const MosquitoAnim anim{ static_cast<MosquitoAnim>(i) };

            std::vector<sf::Texture> & textures = m_animTextures.emplace_back();

            const std::filesystem::path imageDirPath{ t_context.settings.media_path / "image" /
                                                      "monster" / "mosquito" / toString(anim) };

            const std::vector<std::filesystem::path> files{ util::findFilesInDirectory(
                imageDirPath, ".png") };

            M_CHECK(
                !files.empty(),
                "Failed to find any MosquitoAnim::" << toString(anim) << " images!");

            textures.reserve(files.size()); // exact size to prevent re-allocations

            for (const std::filesystem::path & path : files)
            {
                sf::Texture & texture{ textures.emplace_back() };
                util::TextureLoader::load(texture, path, true);
            }
        }

        // setup the sprite
        m_sprite.setTexture(
            m_animTextures.at(static_cast<std::size_t>(MosquitoAnim::Idle)).front(), true);

        m_sprite.setPosition(m_spawnPosition);
    }

    void Mosquito::update(const Context &, const float) {}

    void Mosquito::draw(
        const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        const sf::FloatRect wholeRect{ t_context.layout.wholeRect() };
        if (wholeRect.findIntersection(m_sprite.getGlobalBounds()))
        {
            t_target.draw(m_sprite, t_states);
            util::drawRectangleShape(t_target, collisionRect(), false, sf::Color::Red);
        }
    }

    void Mosquito::move(const sf::Vector2f & t_move)
    {
        m_sprite.move(t_move);
        m_flyBounds.position += t_move;
    }

    bool Mosquito::avatarAttack(const Context & t_context, const AttackInfo & t_attackInfo)
    {
        // TODO
        return false;
    }

    const Harm Mosquito::avatarCollide(const sf::FloatRect & t_avatarRect)
    {
        // TODO
        return Harm();
    }

    const sf::FloatRect Mosquito::collisionRect() const
    {
        sf::FloatRect rect{ m_sprite.getGlobalBounds() };
        util::scaleRectInPlace(rect, { 0.5f, 0.5f });

        const float vertOffset{ rect.size.y * 0.5f };
        rect.position.y += vertOffset;
        rect.size.y -= vertOffset;

        return rect;
    }

    const sf::FloatRect Mosquito::attackCollisionRect() const
    {
        sf::FloatRect rect{ collisionRect() };
        rect.position -= sf::Vector2f(2.0f, 2.0f);
        rect.size += sf::Vector2f(4.0f, 4.0f);
        return rect;
    }

} // namespace bramblefore
