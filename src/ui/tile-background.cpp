//
// tile-background.cpp
//
#include "tile-background.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/random.hpp"
#include "util/texture-loader.hpp"

namespace bramblefore
{

    TileBackground::TileBackground()
        : m_texture{}
        , m_sprite{ m_texture }
    {}

    void TileBackground::setup(const Context & t_context)
    {
        const sf::FloatRect wholeRect{ t_context.layout.wholeRect() };

        const int randomSelection{ t_context.random.fromTo(1, 3) };

        std::string filename{ "tile" };
        filename += std::to_string(randomSelection);
        filename += ".png";

        util::TextureLoader::load(
            m_texture, (t_context.settings.media_path / "image" / "splash" / filename), false);

        m_texture.setRepeated(true);

        m_sprite.setTexture(m_texture, true);
        m_sprite.setTextureRect({ { 0, 0 }, { sf::Vector2i(wholeRect.size) } });
        m_sprite.setColor(sf::Color(255, 255, 255, 32));
        const float scale{ t_context.layout.calScaleBasedOnResolution(t_context, 4.0f) };
        m_sprite.setScale({ scale, scale });
    }

    void TileBackground::update(const Context &, const float t_elapsedTimeSec)
    {
        const float offset{ -10.0f * t_elapsedTimeSec };
        const sf::Vector2f move({ offset, offset });
        m_sprite.move(move);
    }

    void TileBackground::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_target.draw(m_sprite, t_states);
    }

} // namespace bramblefore
