#ifndef TILE_BACKGROUND_HPP_INCLUDED
#define TILE_BACKGROUND_HPP_INCLUDED
//
// tile-background.hpp
//
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace bramblefore
{

    struct Context;

    //

    class TileBackground : public sf::Drawable
    {
      public:
        TileBackground();

        void setup(const Context & t_context);
        void update(const Context & t_context, const float t_elapsedTimeSec);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const final;

      private:
        sf::Texture m_texture;
        sf::Sprite m_sprite;
    };

} // namespace bramblefore

#endif // TILE_BACKGROUND_HPP_INCLUDED
