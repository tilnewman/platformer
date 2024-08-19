#ifndef GOBLIN_HPP_INCLUDED
#define GOBLIN_HPP_INCLUDED
//
// goblin.hpp
//
#include "monster.hpp"

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
} // namespace sf

namespace platformer
{

    struct Context;
    struct Settings;

    //

    class Goblin : public IMonster
    {
      public:
        Goblin(Context & context, const sf::FloatRect & region);
        virtual ~Goblin() override = default;

        void update(Context & context, const float frameTimeSec) override;
        void draw(const Context & c, sf::RenderTarget & t, sf::RenderStates s) const override;
        void move(const float amount) override;
        const Harm avatarCollide(const sf::FloatRect & avatarRect) override;
        const sf::FloatRect collisionRect() const override;
        const sf::FloatRect attackCollisionRect() const;

      protected:
        bool animate(const float frameTimeSec); // returns true if animation is finished
        void changeStateBeforeSeeingPlayer(Context & context);
        void changeStateAfterSeeingPlayer(Context & context);

        void loadTextures(const Settings & settings);
        void initialSpriteSetup(Context & context);
        std::size_t frameCount(const MonsterAnim anim) const;
        const sf::IntRect textureRect(const MonsterAnim anim, const std::size_t frame) const;
        void setTexture(sf::Sprite & sprite, const MonsterAnim anim, const std::size_t frame) const;
        void turnToFacePlayer(Context & context);
        void handleWalking(Context & context, const float frameTimeSec);
        float timePerFrameSec(const MonsterAnim anim) const;

      protected:
        sf::FloatRect m_region;
        MonsterAnim m_anim;
        std::size_t m_animFrame;
        sf::Sprite m_sprite;
        float m_elapsedTimeSec;
        bool m_isFacingRight;
        float m_stateElapsedTimeSec;
        float m_stateTimeUntilChangeSec;
        bool m_hasSpottedPlayer;

        static std::vector<sf::Texture> m_textures;
    };

} // namespace platformer

#endif // GOBLIN_HPP_INCLUDED
