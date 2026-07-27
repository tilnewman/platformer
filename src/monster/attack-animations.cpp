//
// attack-animations.cpp
//
#include "attack-animations.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

#include <filesystem>

namespace bramblefore
{

    std::size_t AttackAnimTexturePack::frameCount() const
    {
        if (texture.getSize().y > 0)
        {
            return static_cast<std::size_t>(texture.getSize().x / texture.getSize().y);
        }
        else
        {
            return 0;
        }
    }

    const sf::IntRect AttackAnimTexturePack::textureRect(const std::size_t t_frame) const
    {
        sf::IntRect rect;
        rect.size.x     = static_cast<int>(texture.getSize().y);
        rect.size.y     = rect.size.x;
        rect.position.y = 0;
        rect.position.x = (static_cast<int>(t_frame) * rect.size.x);

        return rect;
    }

    //

    AttackAnimTextureManager::AttackAnimTextureManager()
        : m_texturePacks{}
    {}

    AttackAnimTextureManager & AttackAnimTextureManager::instance()
    {
        static AttackAnimTextureManager instance;
        return instance;
    }

    void AttackAnimTextureManager::setup()
    {
        // one time size to prevent any reallocations
        m_texturePacks.resize(static_cast<std::size_t>(MonsterType::Count));
    }

    void AttackAnimTextureManager::teardown() { m_texturePacks.clear(); }

    void AttackAnimTextureManager::acquire(const Context & t_context, const MonsterType t_type)
    {
        M_CHECK((t_type != MonsterType::Count), "t_type given was MonsterType::Count!");

        AttackAnimTexturePack & pack{ m_texturePacks.at(static_cast<std::size_t>(t_type)) };

        if (pack.ref_count == 0)
        {
            const std::filesystem::path path{ t_context.settings.media_path / "image" / "monster" /
                                              toString(t_type) / "attack-anim.png" };

            util::TextureLoader::load(pack.texture, path);
        }

        ++pack.ref_count;
    }

    void AttackAnimTextureManager::release(const MonsterType t_type)
    {
        M_CHECK((t_type != MonsterType::Count), "t_type given was MonsterType::Count!");

        AttackAnimTexturePack & pack{ m_texturePacks.at(static_cast<std::size_t>(t_type)) };

        M_CHECK(
            (pack.ref_count > 0),
            "release() called while ref_count was zero for monster=" << toString(t_type));

        if (pack.ref_count == 1)
        {
            pack.texture = sf::Texture();
        }

        --pack.ref_count;
    }

    //

    AttackAnimPack::AttackAnimPack(
        const MonsterType t_monster,
        const sf::Texture & t_texture,
        const sf::IntRect & t_textureRect,
        const float t_scale,
        const bool t_isFacingRight,
        const sf::Vector2f & t_position)
        : is_alive{ true }
        , is_facing_right{ t_isFacingRight }
        , elapsed_time_sec{ 0.0f }
        , frame_index{ 0 }
        , monster{ t_monster }
        , sprite{ t_texture, t_textureRect }
    {
        sprite.setScale({ t_scale, t_scale });
        sprite.setPosition(t_position);

        if (!t_isFacingRight)
        {
            sprite.scale({ -1.0f, 1.0f });
        }
    }

    //

    AttackAnimManager::AttackAnimManager()
        : m_animPacks{}
    {}

    void AttackAnimManager::update(const Context &, const float t_elapsedTimeSec)
    {
        bool didAnyFinish{ false };
        for (AttackAnimPack & pack : m_animPacks)
        {
            pack.elapsed_time_sec += t_elapsedTimeSec;
            const float timeBetweenFramesSec{ 0.125f };
            if (pack.elapsed_time_sec > timeBetweenFramesSec)
            {
                pack.elapsed_time_sec -= timeBetweenFramesSec;

                const AttackAnimTexturePack & texturePack{
                    AttackAnimTextureManager::instance().texturePacks().at(
                        static_cast<std::size_t>(pack.monster))
                };

                if (pack.frame_index >= texturePack.frameCount())
                {
                    pack.frame_index = 0;
                    pack.is_alive    = false;
                    didAnyFinish     = true;
                }

                pack.sprite.setTextureRect(texturePack.textureRect(pack.frame_index));
                ++pack.frame_index;
            }

            // TODO if animation collides with player then hurt the player
        }

        if (didAnyFinish)
        {
            std::erase_if(m_animPacks, [](const AttackAnimPack & p) { return !p.is_alive; });
        }
    }

    void AttackAnimManager::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        for (const AttackAnimPack & pack : m_animPacks)
        {
            // don't bother checking if on screen, let monster.cpp do that
            t_target.draw(pack.sprite, t_states);
        }
    }

    void AttackAnimManager::move(const sf::Vector2f & t_move)
    {
        for (AttackAnimPack & pack : m_animPacks)
        {
            pack.sprite.move(t_move);
        }
    }

    void AttackAnimManager::add(
        const Context & t_context,
        const MonsterType t_type,
        const sf::Vector2f & t_position,
        const bool t_isFacingRight)
    {
        M_CHECK((t_type != MonsterType::Count), "t_type was MonsterType::Count!");

        const AttackAnimTexturePack & texturePack{
            AttackAnimTextureManager::instance().texturePacks().at(static_cast<std::size_t>(t_type))
        };

        const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.0f) *
                           t_context.settings.monster_scale };

        m_animPacks.emplace_back(
            t_type,
            texturePack.texture,
            texturePack.textureRect(0),
            scale,
            t_isFacingRight,
            t_position);
    }

} // namespace bramblefore
