#ifndef ANIM_LAYER_LAVA_DRIP_HPP_INCLUDED
#define ANIM_LAYER_LAVA_DRIP_HPP_INCLUDED
//
// anim-layer-lava-drip.hpp
//
#include "map/tile-layer.hpp"
#include "subsystem/harm-collision-manager.hpp"

#include <string>
#include <string_view>
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

    enum class DripSize
    {
        Small,
        Medium,
        Large
    };

    [[nodiscard]] inline constexpr std::string_view toString(const DripSize t_size) noexcept
    {
        if (t_size == DripSize::Small)
        {
            return "small";
        }
        else if (t_size == DripSize::Medium)
        {
            return "medium";
        }
        else
        {
            return "large";
        }
    }

    [[nodiscard]] inline DripSize stringToDripSize(const std::string & t_string) noexcept
    {
        if (t_string == "small")
        {
            return DripSize::Small;
        }
        else if (t_string == "medium")
        {
            return DripSize::Medium;
        }
        else
        {
            return DripSize::Large;
        }
    }

    //

    struct LavaRectSize
    {
        sf::FloatRect rect{};
        DripSize size{ DripSize::Small }; // any works here
    };

    //

    struct LavaDripperAnim
    {
        explicit LavaDripperAnim(
            const DripSize t_size,
            const sf::Texture & t_texture,
            const sf::IntRect & t_textureRect,
            const sf::FloatRect & t_screenRect,
            const float t_timeBetweenDripSec,
            const float t_scale);

        bool is_dripping;
        DripSize size;
        float elapsed_time_sec;
        float time_between_drip_sec;
        float time_between_frames_sec;
        std::size_t frame_index;
        sf::Sprite sprite;
        sf::FloatRect region;
    };

    //

    struct LavaSplatAnim
    {
        explicit LavaSplatAnim(
            const DripSize t_size,
            const sf::Texture & t_texture,
            const sf::IntRect & t_textureRect,
            const float t_scale,
            const sf::FloatRect & t_screenRect);

        bool is_alive;
        DripSize size;
        float elapsed_time_sec;
        float time_between_frames_sec;
        std::size_t frame_index;
        sf::Sprite sprite;
    };

    //

    struct LavaDripAnim
    {
        explicit LavaDripAnim(
            const DripSize t_size,
            const sf::Texture & t_texture,
            const sf::FloatRect & t_screenRect,
            const float t_scale);

        bool is_alive;
        DripSize size;
        float velocity;
        sf::Sprite sprite;
        sf::FloatRect region;
    };

    //

    class LavaDripAnimationLayer
        : public ITileLayer
        , public IHarmCollisionOwner
    {
      public:
        LavaDripAnimationLayer(Context & t_context, const std::vector<LavaRectSize> & t_rectSizes);
        virtual ~LavaDripAnimationLayer() override;

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
        void updateDrippers(const float t_frameTimeSec);
        void updateDrips(const Context & t_context, const float t_frameTimeSec);
        void updateSplats(const float t_frameTimeSec);

        [[nodiscard]] std::size_t frameCount(const sf::Texture & t_texture) const noexcept;

        [[nodiscard]] sf::IntRect
            textureRect(const sf::Texture & t_texture, const std::size_t t_frame) const noexcept;

        [[nodiscard]] Harm makeHarm(const sf::FloatRect & t_rect) const noexcept;

        [[nodiscard]] inline const sf::Texture & dripperTexture(const DripSize t_size) noexcept
        {
            if (t_size == DripSize::Large)
            {
                return m_dripperTexture1;
            }
            else if (t_size == DripSize::Medium)
            {
                return m_dripperTexture2;
            }
            else
            {
                return m_dripperTexture3;
            }
        }

        [[nodiscard]] inline const sf::Texture & dripTexture(const DripSize t_size) noexcept
        {
            if (t_size == DripSize::Large)
            {
                return m_dripTexture1;
            }
            else if (t_size == DripSize::Medium)
            {
                return m_dripTexture2;
            }
            else
            {
                return m_dripTexture3;
            }
        }

        [[nodiscard]] inline const sf::Texture & splatTexture(const DripSize t_size) noexcept
        {
            if (t_size == DripSize::Large)
            {
                return m_splatTexture1;
            }
            else if (t_size == DripSize::Medium)
            {
                return m_splatTexture2;
            }
            else
            {
                return m_splatTexture3;
            }
        }

      private:
        float m_scale;

        sf::Texture m_dripperTexture1;
        sf::Texture m_dripperTexture2;
        sf::Texture m_dripperTexture3;

        sf::Texture m_dripTexture1;
        sf::Texture m_dripTexture2;
        sf::Texture m_dripTexture3;

        sf::Texture m_splatTexture1;
        sf::Texture m_splatTexture2;
        sf::Texture m_splatTexture3;

        std::vector<LavaDripperAnim> m_dripperAnims;
        std::vector<LavaDripAnim> m_dripAnims;
        std::vector<LavaSplatAnim> m_splatAnims;
    };

} // namespace bramblefore

#endif // ANIM_LAYER_LAVA_DRIP_HPP_INCLUDED
