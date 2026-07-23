#ifndef STATE_PLAY_HPP_INCLUDED
#define STATE_PLAY_HPP_INCLUDED
//
// state-play.hpp
//
#include "state/states.hpp"
#include "ui/gui-window.hpp"
#include "ui/spell-select-menu.hpp"

#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace sf
{
    class RenderTarget;
    class Event;
} // namespace sf

namespace bramblefore
{

    struct Context;

    //

    class PlayState : public IState
    {
      public:
        PlayState();
        virtual ~PlayState() override = default;

        [[nodiscard]] State which() const final { return State::Play; }
        void update(const Context & t_context, const float t_frameTimeSec) final;

        void draw(const Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void handleEvent(const Context & t_context, const sf::Event & t_event) final;
        void onEnter(const Context & t_context) final;
        void onExit(const Context & t_context) final;

      private:
        SpellSelectMenu m_spellSelectMenu;

        bool m_isPaused;
        sf::Text m_pauseText;
        std::vector<sf::Vertex> m_pauseFadeVerts;

        // the quit window needs no fade verts because it is built into GuiWindow
        bool m_isQuitting;
        GuiWindow m_quitWindow;

        float m_idleTimeSec;
    };

} // namespace bramblefore

#endif // STATE_PLAY_HPP_INCLUDED
