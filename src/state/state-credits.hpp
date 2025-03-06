#ifndef STATE_CREDITS_HPP_INCLUDED
#define STATE_CREDITS_HPP_INCLUDED
//
// state-credits.hpp
//
#include "state/states.hpp"
#include "ui/spell-select-menu.hpp"

#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sf
{
    class RenderTarget;
    class Event;
} // namespace sf

namespace bramblefore
{

    struct Context;

    //

    struct CreditAnim
    {
        CreditAnim(
            const Context & t_context,
            const std::string & t_imageFilePath,
            const float t_imageScale,
            const std::string & t_name,
            const std::string & t_description,
            const float t_vertPos);

        void move(const float t_amount);
        float bottom() const;
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;

        sf::Texture texture;
        sf::Sprite sprite;
        sf::Text name;
        sf::Text description;
    };

    //

    class CreditsState : public IState
    {
      public:
        CreditsState();
        virtual ~CreditsState() override = default;

        State which() const final { return State::Credits; }
        void update(Context & t_context, const float t_frameTimeSec) final;

        void draw(Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void handleEvent(Context & t_context, const sf::Event & t_event) final;
        void onEnter(Context & t_context) final;
        void onExit(Context & t_context) final;

      private:
        sf::Text m_titleText;
        std::vector<CreditAnim> m_credits;
    };

} // namespace bramblefore

#endif // STATE_CREDITS_HPP_INCLUDED
