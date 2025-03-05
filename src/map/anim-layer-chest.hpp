#ifndef ANIM_LAYER_CHEST_HPP_INCLUDED
#define ANIM_LAYER_CHEST_HPP_INCLUDED
//
// anim-layer-chest.hpp
//
#include "map/tile-layer.hpp"
#include "subsystem/harm-collision-manager.hpp"
#include "util/sfml-defaults.hpp"

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

    enum class Chest
    {
        Chest1 = 0,
        Chest2,
        Chest3,
        Chest4,
        Chest5,
        Count
    };

    [[nodiscard]] inline constexpr Chest chestFromString(const std::string_view name) noexcept
    {
        // clang-format off
        if      ("chest1" == name) { return Chest::Chest1; }
        else if ("chest2" == name) { return Chest::Chest2; }
        else if ("chest3" == name) { return Chest::Chest3; }
        else if ("chest4" == name) { return Chest::Chest4; }
        else if ("chest5" == name) { return Chest::Chest5; }
        else                       { return Chest::Count;  }
        // clang-format on
    }

    //

    struct ChestTextures
    {
        sf::Texture open{};
        sf::Texture closed{};
    };

    //

    struct ChestAnim
    {
        explicit ChestAnim(
            const sf::Texture & t_texture,
            const Chest t_chest,
            const float t_scale,
            const sf::FloatRect & t_screenRect);

        Chest chest;
        bool is_open;
        sf::Sprite sprite;
    };

    //

    // this class inherits from IHarmCollisionOwner even though chests don't harm the player
    // the harm mechanic is just being used as a trigger to know when to open the chests
    class ChestAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        ChestAnimationLayer(Context & t_context);
        virtual ~ChestAnimationLayer() override;

        void add(Context & t_context, const Chest t_chest, const sf::FloatRect & t_rect);

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void move(const Context & t_context, const float t_amount) final;
        void dumpInfo() const final {}
        void update(Context &, const float) final {}

        void appendVertLayer(
            const Context &,
            const sf::Vector2f &,
            const sf::Vector2i &,
            const sf::Vector2i &,
            const sf::Vector2f &) final
        {}

        Harm avatarCollide(Context &, const sf::FloatRect & t_avatarRect) override;

      private:
        void loadTextures(Context & t_context);

      private:
        std::vector<ChestTextures> m_textureSets;
        std::vector<ChestAnim> m_animations;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_CHEST_HPP_INCLUDED
