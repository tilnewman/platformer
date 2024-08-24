#ifndef ACID_SPOUT_ANIM_LAYER_HPP_INCLUDED
#define ACID_SPOUT_ANIM_LAYER_HPP_INCLUDED
//
// acid-spout-anim-layer.hpp
//
#include "harm-collision-manager.hpp"
#include "tile-layer.hpp"

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

    //

    struct AcidSplashAnim
    {
        bool is_alive{ true };
        float elapsed_time_sec{ 0.0f };
        float time_between_frames_sec{ 0.1f };
        std::size_t frame_index{ 0 };
        sf::Sprite sprite{};
    };

    //

    struct AcidDropAnim
    {
        bool is_alive{ true };
        float velocity{ 0.0f };
        sf::Sprite sprite{};
        sf::FloatRect region{};
    };

    //

    struct AcidSpoutAnim
    {
        bool is_dripping{ false };
        float elapsed_time_sec{ 0.0f };
        float time_between_drips{ 0.0f };
        float time_between_frames_sec{ 0.15f };
        std::size_t frame_index{ 0 };
        sf::Sprite sprite{};
        sf::FloatRect region{};
    };

    //

    class AcidSpoutAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        AcidSpoutAnimationLayer(Context & t_context, const std::vector<sf::FloatRect> & t_rects);
        virtual ~AcidSpoutAnimationLayer() override;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void move(const Context & t_context, const float t_amount) final;
        inline float findFarthestHorizVert() const final { return 0.0f; }
        inline void dumpInfo() const final {}

        void update(Context & t_context, const float t_frameTimeSec) final;

        inline void appendVertLayer(
            const Context &,
            const sf::Vector2f &,
            const sf::Vector2i &,
            const sf::Vector2i &,
            const sf::Vector2f &) final
        {}

        Harm avatarCollide(Context & t_context, const sf::FloatRect & t_avatarRect) final;

      private:
        std::size_t frameCount(const sf::Texture & t_texture) const;
        sf::IntRect textureRect(const sf::Texture & t_texture, const std::size_t t_frame) const;

      private:
        float m_scale;
        sf::Texture m_spoutTexture;
        sf::Texture m_dropTexture;
        sf::Texture m_splashTexture;
        std::vector<AcidSpoutAnim> m_spoutAnims;
        std::vector<AcidDropAnim> m_dropAnims;
        std::vector<AcidSplashAnim> m_splashAnims;
    };

} // namespace platformer

#endif // ACID_SPOUT_ANIM_LAYER_HPP_INCLUDED
