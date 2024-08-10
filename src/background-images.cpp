// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// background-images.cpp
//
#include "background-images.hpp"

#include "context.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"

namespace platformer
{

    BackgroundImages::BackgroundImages()
        : m_skyTexture()
        , m_cloudsBackTexture()
        , m_cloudsFrontTexture()
        , m_mountainsTexture()
        , m_treesTexture()
        , m_mistsTexture()
        , m_skySprite()
        , m_cloudsBackSprite()
        , m_cloudsFrontSprite()
        , m_mountainsSprite()
        , m_treesSprite()
        , m_mistsSprite()
    {}

    void BackgroundImages::setup(const Context & context)
    {
        const std::string imagePath =
            (context.settings.media_path / "image/background/forest").string();

        m_skyTexture.loadFromFile(imagePath + "/sky.png");
        m_cloudsBackTexture.loadFromFile(imagePath + "/clouds-back.png");
        m_cloudsFrontTexture.loadFromFile(imagePath + "/clouds-front.png");
        m_mountainsTexture.loadFromFile(imagePath + "/mountains.png");
        m_treesTexture.loadFromFile(imagePath + "/trees.png");
        m_mistsTexture.loadFromFile(imagePath + "/mist.png");

        m_skyTexture.setSmooth(true);
        m_cloudsBackTexture.setSmooth(true);
        m_cloudsFrontTexture.setSmooth(true);
        m_mountainsTexture.setSmooth(true);
        m_treesTexture.setSmooth(true);
        m_mistsTexture.setSmooth(true);

        m_skySprite.setTexture(m_skyTexture);
        m_cloudsBackSprite.setTexture(m_cloudsBackTexture);
        m_cloudsFrontSprite.setTexture(m_cloudsFrontTexture);
        m_mountainsSprite.setTexture(m_mountainsTexture);
        m_treesSprite.setTexture(m_treesTexture);
        m_mistsSprite.setTexture(m_mistsTexture);

        util::scaleAndCenterInside(m_skySprite, context.layout.wholeRect());
        util::scaleAndCenterInside(m_cloudsBackSprite, context.layout.wholeRect());
        util::scaleAndCenterInside(m_cloudsFrontSprite, context.layout.wholeRect());
        util::scaleAndCenterInside(m_mountainsSprite, context.layout.wholeRect());
        util::scaleAndCenterInside(m_treesSprite, context.layout.wholeRect());
        util::scaleAndCenterInside(m_mistsSprite, context.layout.wholeRect());
    }

    void BackgroundImages::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_skySprite, states);
        target.draw(m_cloudsBackSprite, states);
        target.draw(m_cloudsFrontSprite, states);
        target.draw(m_mountainsSprite, states);
        target.draw(m_treesSprite, states);
        target.draw(m_mistsSprite, states);
    }

} // namespace platformer
