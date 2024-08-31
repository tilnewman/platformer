// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-char-select.hpp
//
#include "state/state-char-select.hpp"

#include "avatar/avatar-textures.hpp"
#include "bramblefore/settings.hpp"
#include "state/state-manager.hpp"
#include "subsystem/context.hpp"
#include "subsystem/font.hpp"
#include "subsystem/screen-layout.hpp"
#include "ui/text-layout.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace bramblefore
{

    CharacterSelectState::CharacterSelectState()
        : m_paperTexture{}
        , m_paperSprite{}
        , m_avatarType{ AvatarType::BlueKnight }
        , m_avatarSprite{}
        , m_paperInnerRect{ 90, 64, 368, 384 }
        , m_avatarTypeText{}
        , m_descriptionTexts{}
        , m_avatarPoseSprites{}
    {}

    void CharacterSelectState::update(Context & t_context, const float t_frameTimeSec) {}

    void CharacterSelectState::draw(
        Context & t_context, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_target.draw(m_paperSprite, t_states);
        t_target.draw(m_avatarSprite, t_states);
        t_target.draw(m_avatarTypeText, t_states);

        for (const sf::Text & text : m_descriptionTexts)
        {
            t_target.draw(text, t_states);
        }

        for (const sf::Sprite & poseSprite : m_avatarPoseSprites)
        {
            t_target.draw(poseSprite, t_states);
        }
    }

    void CharacterSelectState::handleEvent(Context & t_context, const sf::Event & t_event)
    {
        if (t_event.type != sf::Event::KeyPressed)
        {
            return;
        }

        if (t_event.key.code == sf::Keyboard::Enter)
        {
            t_context.state.setChangePending(State::Play);
        }
        else if (t_event.key.code == sf::Keyboard::Left)
        {
            AvatarTextureManager::instance().release(m_avatarType);

            std::size_t typeIndex{ static_cast<std::size_t>(m_avatarType) };
            if (0 == typeIndex)
            {
                typeIndex = (static_cast<std::size_t>(AvatarType::Count) - 1_st);
            }
            else
            {
                --typeIndex;
            }

            m_avatarType = static_cast<AvatarType>(typeIndex);
            t_context.sfx.play("ui-select-thock-slide");
            setup(t_context);
        }
        else if (t_event.key.code == sf::Keyboard::Right)
        {
            AvatarTextureManager::instance().release(m_avatarType);

            std::size_t typeIndex{ static_cast<std::size_t>(m_avatarType) };
            if (typeIndex < (static_cast<std::size_t>(AvatarType::Count) - 1_st))
            {
                ++typeIndex;
            }
            else
            {
                typeIndex = 0;
            }

            m_avatarType = static_cast<AvatarType>(typeIndex);
            t_context.sfx.play("ui-select-thock-slide");
            setup(t_context);
        }
    }

    void CharacterSelectState::onEnter(Context & t_context)
    {
        m_paperTexture.loadFromFile(
            (t_context.settings.media_path / "image/ui/paper-runes.png").string());

        m_paperSprite.setTexture(m_paperTexture);

        util::fitAndCenterInside(
            m_paperSprite, util::scaleRectInPlaceCopy(t_context.layout.wholeRect(), 0.5f));

        const float paperScale{ m_paperSprite.getScale().x };
        m_paperInnerRect.left *= paperScale;
        m_paperInnerRect.left += m_paperSprite.getPosition().x;
        m_paperInnerRect.top *= paperScale;
        m_paperInnerRect.top += m_paperSprite.getPosition().y;
        m_paperInnerRect.width *= paperScale;
        m_paperInnerRect.height *= paperScale;

        setup(t_context);
    }

    void CharacterSelectState::onExit(Context & t_context)
    {
        AvatarTextureManager::instance().release(m_avatarType);
    }

    void CharacterSelectState::setup(Context & t_context)
    {
        AvatarTextureManager::instance().acquire(t_context, m_avatarType);

        m_avatarSprite.setTexture(AvatarTextureManager::instance().getDefault(m_avatarType));

        const float avatarScale{ t_context.layout.calScaleBasedOnResolution(t_context, 3.0f) };
        m_avatarSprite.setScale(avatarScale, avatarScale);
        m_avatarSprite.setColor(sf::Color(255, 255, 255, 75));
        util::centerInside(m_avatarSprite, m_paperInnerRect);

        //

        m_avatarTypeText = t_context.font.makeText(
            Font::Default,
            FontSize::Large,
            std::string(toName(m_avatarType)),
            sf::Color(0, 0, 0, 160),
            sf::Text::Bold);

        m_avatarTypeText.setPosition(
            (util::center(m_paperInnerRect).x - (m_avatarTypeText.getGlobalBounds().width * 0.5f)),
            m_paperInnerRect.top);

        //

        sf::FloatRect descriptionRect{ m_paperInnerRect };

        descriptionRect.top =
            (util::bottom(m_avatarTypeText) + m_avatarTypeText.getGlobalBounds().height);

        const TextDetails descriptionTextDeatils{ Font::Default,
                                                  FontSize::Small,
                                                  sf::Color(0, 0, 0, 220) };

        m_descriptionTexts = TextLayout::layout(
            t_context, avatarDescription(m_avatarType), descriptionRect, descriptionTextDeatils);

        //

        const float poseScale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.0f) };

        m_avatarPoseSprites.clear();
        m_avatarPoseSprites.reserve(8);

        sf::Sprite & poseSprite1{ m_avatarPoseSprites.emplace_back() };
        AvatarTextureManager::instance().set(poseSprite1, m_avatarType, AvatarAnim::Attack, 3);
        poseSprite1.setScale(poseScale, poseScale);

        poseSprite1.setPosition(
            m_paperInnerRect.left,
            (util::bottom(m_paperInnerRect) - (poseSprite1.getGlobalBounds().height * 0.6f)));

        const float poseSpriteHorizPad{ poseSprite1.getGlobalBounds().width * 0.07f };

        sf::Sprite & poseSprite2{ m_avatarPoseSprites.emplace_back() };
        AvatarTextureManager::instance().set(poseSprite2, m_avatarType, AvatarAnim::Death, 2);
        poseSprite2.setScale(poseScale, poseScale);

        poseSprite2.setPosition(
            (util::right(poseSprite1) + poseSpriteHorizPad), poseSprite1.getPosition().y);

        sf::Sprite & poseSprite3{ m_avatarPoseSprites.emplace_back() };
        AvatarTextureManager::instance().set(poseSprite3, m_avatarType, AvatarAnim::Jump, 1);
        poseSprite3.setScale(poseScale, poseScale);

        poseSprite3.setPosition(
            (util::right(poseSprite2) + poseSpriteHorizPad), poseSprite1.getPosition().y);

        sf::Sprite & poseSprite4{ m_avatarPoseSprites.emplace_back() };
        AvatarTextureManager::instance().set(poseSprite4, m_avatarType, AvatarAnim::Jump, 3);
        poseSprite4.setScale(poseScale, poseScale);

        poseSprite4.setPosition(
            (util::right(poseSprite3) + poseSpriteHorizPad), poseSprite1.getPosition().y);
    }

    std::string CharacterSelectState::avatarDescription(const AvatarType type) const
    {
        if (AvatarType::Assassin == type)
        {
            return "The land of Bramblefore has always had villains, and Assassins are among the "
                   "worst...";
        }
        else if (AvatarType::BlueKnight == type)
        {
            return "The knights of castle Bluehorn are famed for their steadfast honor...";
        }
        else if (AvatarType::Druid == type)
        {
            return "Druids are wizards of earth, fire, water, and air...";
        }
        else if (AvatarType::Enchantress == type)
        {
            return "Enchantress";
        }
        else if (AvatarType::Ninja == type)
        {
            return "Ninjas...";
        }
        else if (AvatarType::RedKnight == type)
        {
            return "Crimson, or fallen knights, have abandoned their codes of chivalry and "
                   "honor...";
        }
        else if (AvatarType::Rogue == type)
        {
            return "Rogues...";
        }
        else if (AvatarType::Viking == type)
        {
            return "Following the old Norse gods of valor can lead one to become a sword wielding "
                   "maniac...";
        }
        else
        {
            return "Witches...";
        }
    }

} // namespace bramblefore
