#ifndef WATER_ANIM_LAYER_HPP_INCLUDED
#define WATER_ANIM_LAYER_HPP_INCLUDED
//
// water-anim-layer.hpp
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

    struct WaterAnim
    {
        sf::Sprite sprite{ util::SfmlDefaults::instance().texture() };
        float elapsed_time_sec{ 0.0f };
        float time_per_frame_sec{ 0.0f };
        std::size_t frame_index{ 0 };
    };

    //

    class WaterAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        WaterAnimationLayer(Context & t_context, const std::vector<sf::FloatRect> & t_rects);
        virtual ~WaterAnimationLayer() override;

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

        [[nodiscard]] Harm
            avatarCollide(Context & t_context, const sf::FloatRect & t_avatarRect) override;

      private:
        [[nodiscard]] std::size_t frameCount() const noexcept;
        [[nodiscard]] sf::IntRect textureRect(const std::size_t frame) const noexcept;

      private:
        sf::Texture m_texture;
        std::vector<WaterAnim> m_anims;
    };

} // namespace bramblefore

#endif // WATER_ANIM_LAYER_HPP_INCLUDED
