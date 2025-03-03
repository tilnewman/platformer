#ifndef ANIM_LAYER_DART_TRAP_HPP_INCLUDED
#define ANIM_LAYER_DART_TRAP_HPP_INCLUDED
//
// anim-layer-dart-trap.hpp
//
#include "map/tile-layer.hpp"
#include "subsystem/harm-collision-manager.hpp"

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sf
{
    class RenderTarget;
} // namespace sf

namespace bramblefore
{

    struct Context;

    //

    struct DartRectDir
    {
        explicit DartRectDir(const bool t_isLeft, const sf::FloatRect & t_rect)
            : is_left{ t_isLeft }
            , rect{ t_rect }

        {}

        bool is_left;
        sf::FloatRect rect;
    };

    //

    struct Shooter
    {
        explicit Shooter(
            const bool t_isLeft,
            const sf::Texture & t_texture,
            const sf::FloatRect & t_rect,
            const float t_timeBetweenShotsSec)
            : is_left{ t_isLeft }
            , sprite{ t_texture }
            , region{ t_rect }
            , time_between_shots_sec{ t_timeBetweenShotsSec }
            , elapsed_time_sec{ 0.0 }
        {}

        bool is_left;
        sf::Sprite sprite;
        sf::FloatRect region;
        float time_between_shots_sec;
        float elapsed_time_sec;
    };

    //

    struct Dart
    {
        explicit Dart(
            const bool t_isLeft, const sf::Texture & t_texture, const sf::FloatRect & t_rect)
            : is_alive{ true }
            , is_left{ t_isLeft }
            , sprite{ t_texture }
            , region{ t_rect }
        {}

        bool is_alive;
        bool is_left;
        sf::Sprite sprite;
        sf::FloatRect region; // determines where and how far the darts can fly
    };

    //

    class DartTrapAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        DartTrapAnimationLayer(Context & t_context, const std::vector<DartRectDir> & t_rectDirs);
        virtual ~DartTrapAnimationLayer() override;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const override;

        void move(const Context & t_context, const float t_amount) override;
        void dumpInfo() const override {}

        void update(Context & t_context, const float t_frameTimeSec) override;

        void appendVertLayer(
            const Context &,
            const sf::Vector2f &,
            const sf::Vector2i &,
            const sf::Vector2i &,
            const sf::Vector2f &) override
        {}

        Harm avatarCollide(Context & t_context, const sf::FloatRect & t_avatarRect) override;

      private:
        sf::Texture m_shooterTexture;
        sf::Texture m_dartTexture;
        std::vector<Shooter> m_shooters;
        std::vector<Dart> m_darts;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_DART_TRAP_HPP_INCLUDED
