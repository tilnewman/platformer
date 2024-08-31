#ifndef STATE_CHAR_SELECT_HPP_INCLUDED
#define STATE_CHAR_SELECT_HPP_INCLUDED
//
// state-char-select.hpp
//
#include "player/player-info.hpp"
#include "state/states.hpp"

#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sf
{
    class RenderTarget;
    class Event;
} // namespace sf

namespace bramblefore
{

    struct Context;

    //

    class CharacterSelectState : public IState
    {
      public:
        CharacterSelectState();
        virtual ~CharacterSelectState() override = default;

        State which() const final { return State::CharacterSelect; }
        void update(Context & t_context, const float t_frameTimeSec) final;

        void draw(Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states)
            const final;

        void handleEvent(Context & t_context, const sf::Event & t_event) final;
        void onEnter(Context & t_context) final;
        void onExit(Context & t_context) final;

        private:
        void setup(Context & t_context);
        std::string avatarDescription(const AvatarType type)const;

      private:
        sf::Texture m_paperTexture;
        sf::Sprite m_paperSprite;
        AvatarType m_avatarType;
        sf::Sprite m_avatarSprite;
        sf::FloatRect m_paperInnerRect;
        sf::Text m_avatarTypeText;
        std::vector<sf::Text> m_descriptionTexts;
        std::vector<sf::Sprite> m_avatarPoseSprites;
    };

} // namespace bramblefore

#endif // STATE_CHAR_SELECT_HPP_INCLUDED
