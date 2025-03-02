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
#include "util/check-macros.hpp"
#include "util/sfml-defaults.hpp"
#include "util/sfml-util.hpp"
#include "util/sound-player.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace bramblefore
{

    CharacterSelectState::CharacterSelectState()
        : m_titleText{ util::SfmlDefaults::instance().font() }
        , m_instructionsText{ util::SfmlDefaults::instance().font() }
        , m_paperTexture{}
        , m_paperSprite{ m_paperTexture }
        , m_avatarType{ AvatarType::BlueKnight }
        , m_avatarSprite{ util::SfmlDefaults::instance().texture() }
        , m_paperInnerRect{ { 90, 64 }, { 368, 384 } }
        , m_avatarTypeText{ util::SfmlDefaults::instance().font() }
        , m_descriptionTexts{}
        , m_avatarClassText{ util::SfmlDefaults::instance().font() }
        , m_classDescriptionTexts{}
        , m_avatarPoseSprites{}
    {}

    void CharacterSelectState::update(Context &, const float) {}

    void CharacterSelectState::draw(
        Context &, sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_target.draw(m_titleText, t_states);
        t_target.draw(m_instructionsText, t_states);
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

        t_target.draw(m_avatarClassText, t_states);

        for (const sf::Text & text : m_classDescriptionTexts)
        {
            t_target.draw(text, t_states);
        }
    }

    void CharacterSelectState::handleEvent(Context & t_context, const sf::Event & t_event)
    {
        if (const auto * const keyPtr = t_event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPtr->scancode == sf::Keyboard::Scancode::Enter)
            {
                t_context.player.setup(t_context, m_avatarType);
                t_context.state.setChangePending(State::Play);
            }
            else if (keyPtr->scancode == sf::Keyboard::Scancode::Left)
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
            else if (keyPtr->scancode == sf::Keyboard::Scancode::Right)
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
    }

    void CharacterSelectState::onEnter(Context & t_context)
    {
        m_titleText = t_context.font.makeText(
            Font::Title, FontSize::Huge, "Character Selection", sf::Color(220, 220, 220));

        m_titleText.setPosition({ (util::center(t_context.layout.wholeRect()).x -
                                   (m_titleText.getGlobalBounds().size.x * 0.5f)),
                                  (t_context.layout.wholeSize().y * 0.15f) });

        m_instructionsText = t_context.font.makeText(
            Font::General,
            FontSize::Small,
            "(Use left and right arrow keys to select a character, then press enter.)",
            sf::Color(160, 160, 160),
            sf::Text::Italic);

        m_instructionsText.setPosition(
            { (util::center(t_context.layout.wholeRect()).x -
               (m_instructionsText.getGlobalBounds().size.x * 0.5f)),
              (util::bottom(m_titleText) + (m_instructionsText.getGlobalBounds().size.y * 0.5f)) });

        util::TextureLoader::load(
            m_paperTexture, (t_context.settings.media_path / "image/ui/paper-runes.png"), true);

        m_paperSprite.setTexture(m_paperTexture, true);

        util::fitAndCenterInside(
            m_paperSprite, util::scaleRectInPlaceCopy(t_context.layout.wholeRect(), 0.5f));

        const float paperScale{ m_paperSprite.getScale().x };
        m_paperInnerRect.position.x *= paperScale;
        m_paperInnerRect.position.x += m_paperSprite.getPosition().x;
        m_paperInnerRect.position.y *= paperScale;
        m_paperInnerRect.position.y += m_paperSprite.getPosition().y;
        m_paperInnerRect.size.x *= paperScale;
        m_paperInnerRect.size.y *= paperScale;

        setup(t_context);
    }

    void CharacterSelectState::onExit(Context &)
    {
        AvatarTextureManager::instance().release(m_avatarType);
    }

    void CharacterSelectState::setup(Context & t_context)
    {
        AvatarTextureManager::instance().acquire(t_context, m_avatarType);

        m_avatarSprite.setTexture(AvatarTextureManager::instance().getDefault(m_avatarType), true);

        const float avatarScale{ t_context.layout.calScaleBasedOnResolution(t_context, 3.0f) };
        m_avatarSprite.setScale({ avatarScale, avatarScale });
        m_avatarSprite.setColor(sf::Color(255, 255, 255, 75));
        util::centerInside(m_avatarSprite, m_paperInnerRect);

        //

        m_avatarTypeText = t_context.font.makeText(
            Font::Title,
            FontSize::Large,
            std::string(toName(m_avatarType)),
            sf::Color(0, 0, 0, 160),
            sf::Text::Bold);

        m_avatarTypeText.setPosition({ (util::center(m_paperInnerRect).x -
                                        (m_avatarTypeText.getGlobalBounds().size.x * 0.5f)),
                                       m_paperInnerRect.position.y });

        //

        sf::FloatRect descriptionRect{ m_paperInnerRect };

        descriptionRect.position.y =
            (util::bottom(m_avatarTypeText) + m_avatarTypeText.getGlobalBounds().size.y);

        const TextDetails descriptionTextDeatils{ Font::General,
                                                  FontSize::Small,
                                                  sf::Color(0, 0, 0, 220) };

        m_descriptionTexts = TextLayout::layout(
            t_context, avatarDescription(m_avatarType), descriptionRect, descriptionTextDeatils);

        //

        const std::string classNameString = [&]() {
            if (isWizard(m_avatarType))
            {
                return "Wizard";
            }
            else if (isRaider(m_avatarType))
            {
                return "Raider";
            }
            else
            {
                return "Warrior";
            }
        }();

        m_avatarClassText = t_context.font.makeText(
            Font::Title, FontSize::Medium, classNameString, sf::Color(0, 0, 0, 200));

        m_avatarClassText.setPosition({ (util::center(m_paperInnerRect).x -
                                         (m_avatarClassText.getGlobalBounds().size.x * 0.5f)),
                                        (util::bottom(m_descriptionTexts.back()) +
                                         m_avatarTypeText.getGlobalBounds().size.y) });

        //

        descriptionRect.position.y =
            (util::bottom(m_avatarClassText) + m_avatarClassText.getGlobalBounds().size.y);

        m_classDescriptionTexts = TextLayout::layout(
            t_context,
            avatarClassDescription(m_avatarType),
            descriptionRect,
            descriptionTextDeatils);

        //

        const sf::Color poseColor{ 255, 255, 255, 200 };
        const float poseScale{ t_context.layout.calScaleBasedOnResolution(t_context, 1.0f) };

        m_avatarPoseSprites.clear();
        m_avatarPoseSprites.reserve(8);

        sf::Sprite & poseSprite1{ m_avatarPoseSprites.emplace_back(
            util::SfmlDefaults::instance().texture()) };

        AvatarTextureManager::instance().set(poseSprite1, m_avatarType, AvatarAnim::Attack, 3);
        poseSprite1.setScale({ poseScale, poseScale });
        poseSprite1.setColor(poseColor);

        poseSprite1.setPosition(
            { m_paperInnerRect.position.x,
              (util::bottom(m_paperInnerRect) - (poseSprite1.getGlobalBounds().size.y * 0.6f)) });

        const float poseSpriteHorizPad{ poseSprite1.getGlobalBounds().size.x * 0.07f };

        sf::Sprite & poseSprite2{ m_avatarPoseSprites.emplace_back(
            util::SfmlDefaults::instance().texture()) };

        AvatarTextureManager::instance().set(poseSprite2, m_avatarType, AvatarAnim::Death, 2);
        poseSprite2.setScale({ poseScale, poseScale });
        poseSprite2.setColor(poseColor);

        poseSprite2.setPosition(
            { (util::right(poseSprite1) + poseSpriteHorizPad), poseSprite1.getPosition().y });

        sf::Sprite & poseSprite3{ m_avatarPoseSprites.emplace_back(
            util::SfmlDefaults::instance().texture()) };

        AvatarTextureManager::instance().set(poseSprite3, m_avatarType, AvatarAnim::Jump, 1);
        poseSprite3.setScale({ poseScale, poseScale });
        poseSprite3.setColor(poseColor);

        poseSprite3.setPosition(
            { (util::right(poseSprite2) + poseSpriteHorizPad), poseSprite1.getPosition().y });

        sf::Sprite & poseSprite4{ m_avatarPoseSprites.emplace_back(
            util::SfmlDefaults::instance().texture()) };

        AvatarTextureManager::instance().set(poseSprite4, m_avatarType, AvatarAnim::Jump, 3);
        poseSprite4.setScale({ poseScale, poseScale });
        poseSprite4.setColor(poseColor);

        poseSprite4.setPosition(
            { (util::right(poseSprite3) + poseSpriteHorizPad), poseSprite1.getPosition().y });
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

    std::string CharacterSelectState::avatarClassDescription(const AvatarType type) const
    {
        if (isWizard(type))
        {
            return "Being a wizard is all about casting exotic and powerful spells. Wizards do "
                   "have a melee attack they can use, but that is more of a last resort when their "
                   "mana is exhausted. While adventuring you will find mana potions to help keep "
                   "your mana high. You will only have one spell available when your journey "
                   "begins, but don't worry, there are a variety of ways to learn new spells. Each "
                   "of the three types of Wizards has a unique book of spells they can learn.";
        }
        else if (isRaider(type))
        {
            return "The Raiders are a class of adventurer that are sneaky and obsessed with "
                   "treasure. They are the only class that can pilfer gold coins from the enemies "
                   "they kill. They gain experience from vanquishing monsters just like all other "
                   "classes, but Raiders will have more weapons and armor to choose from that can "
                   "be bought from the extra gold coins they will collect.";
        }
        else
        {
            return "Warriors are a simple breed that focus on close quarters combat. Their attacks "
                   "with mighty swords reach farther and do more damage than Raiders or Wizards. "
                   "They cannot cast spells or steal coins, but they will have the best weapons "
                   "and armor to choose from. Warriors are the slowest with the lowest jumps.";
        }
    }

} // namespace bramblefore
