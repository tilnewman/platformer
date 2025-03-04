#ifndef ANIM_LAYER_ACID_SPOUT_HPP_INCLUDED
#define ANIM_LAYER_ACID_SPOUT_HPP_INCLUDED
//
// anim-layer-acid-spout.hpp
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

    struct AcidSplashAnim
    {
        explicit AcidSplashAnim(const sf::Texture & t_texture)
            : is_alive{ true }
            , elapsed_time_sec{ 0.0f }
            , time_between_frames_sec{ 0.1f }
            , frame_index{ 0 }
            , sprite{ t_texture }
        {}

        bool is_alive;
        float elapsed_time_sec;
        float time_between_frames_sec;
        std::size_t frame_index;
        sf::Sprite sprite;
    };

    //

    struct AcidDropAnim
    {
        explicit AcidDropAnim(const sf::Texture & t_texture)
            : is_alive{ true }
            , velocity{ 0.0f }
            , sprite{ t_texture }
            , region{}
        {}

        bool is_alive;
        float velocity;
        sf::Sprite sprite;

        // the rect drawn on the map that includes the vert distance this drop will travel
        sf::FloatRect region;
    };

    //

    struct AcidSpoutAnim
    {
        explicit AcidSpoutAnim(const sf::Texture & t_texture)
            : is_dripping{ false }
            , elapsed_time_sec{ 0.0f }
            , time_between_drips{ 0.0f }
            , time_between_frames_sec{ 0.15f }
            , frame_index{ 0 }
            , sprite{ t_texture }
            , region{}
        {}

        bool is_dripping;
        float elapsed_time_sec;
        float time_between_drips;
        float time_between_frames_sec;
        std::size_t frame_index;
        sf::Sprite sprite;

        // the rect drawn on the map that includes the vert distance the drops will travel
        // given to drops so they know how far to fall, see above
        sf::FloatRect region;
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
        void updateSpouts(const float t_frameTimeSec);
        void updateDrops(const Context & t_context, const float t_frameTimeSec);
        void updateSplashes(const float t_frameTimeSec);

        [[nodiscard]] std::size_t frameCount(const sf::Texture & t_texture) const noexcept;

        [[nodiscard]] sf::IntRect
            textureRect(const sf::Texture & t_texture, const std::size_t t_frame) const noexcept;

        [[nodiscard]] Harm makeHarm(const sf::FloatRect & t_rect) const noexcept;

      private:
        float m_scale;
        sf::Texture m_spoutTexture;
        sf::Texture m_dropTexture;
        sf::Texture m_splashTexture;
        std::vector<AcidSpoutAnim> m_spoutAnims;
        std::vector<AcidDropAnim> m_dropAnims;
        std::vector<AcidSplashAnim> m_splashAnims;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_ACID_SPOUT_HPP_INCLUDED
