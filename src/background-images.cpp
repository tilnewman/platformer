// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// background-images.cpp
//
#include "background-images.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "screen-layout.hpp"
#include "settings.hpp"
#include "sfml-util.hpp"
#include "texture-stats.hpp"

namespace platformer
{

    BackgroundImages::BackgroundImages()
        : m_loadedSetName()
        , m_backgroundTexture()
        , m_overlayTexture()
        , m_backgroundSprite()
        , m_overlaySprite()
        , m_slidingImages()
        , m_fadeQuads()
    {}

    void BackgroundImages::setup(const Context & context, const std::string & name)
    {
        if (name == m_loadedSetName)
        {
            return;
        }

        m_loadedSetName = name;

        const BackgroundImagesInfo infoPack{ infoFactory(context, name) };

        // there won't always be a static background image
        if (!infoPack.background_path.empty())
        {
            m_backgroundTexture.loadFromFile(infoPack.background_path.string());
            TextureStats::instance().process(m_backgroundTexture);
            m_backgroundSprite.setTexture(m_backgroundTexture);
            util::scaleAndCenterInside(m_backgroundSprite, context.layout.wholeRect());
        }

        // there won't always be a static overlay image
        if (!infoPack.overlay_path.empty())
        {
            m_overlayTexture.loadFromFile(infoPack.overlay_path.string());
            TextureStats::instance().process(m_overlayTexture);
            m_overlaySprite.setTexture(m_overlayTexture);
            util::scaleAndCenterInside(m_overlaySprite, context.layout.wholeRect());
        }

        // doubt there will ever be more than eight of these
        m_slidingImages.clear();
        m_slidingImages.reserve(16);

        for (const SlidingImageInfo & info : infoPack.sliding_images)
        {
            M_CHECK(
                std::filesystem::exists(info.path),
                "Error:  BackgroundImages::setup() given a path that doesn't exist: " << info.path);

            SlidingImage & slidingImage{ m_slidingImages.emplace_back() };
            slidingImage.info = info;
            slidingImage.texture.loadFromFile(info.path.string());
            TextureStats::instance().process(slidingImage.texture);
            slidingImage.sprite_left.setTexture(slidingImage.texture);
            slidingImage.sprite_right.setTexture(slidingImage.texture);
            util::scaleAndCenterInside(slidingImage.sprite_left, context.layout.wholeRect());
            util::scaleAndCenterInside(slidingImage.sprite_right, context.layout.wholeRect());
            slidingImage.sprite_right.move(slidingImage.sprite_left.getGlobalBounds().width, 0.0f);
        }

        if (infoPack.fade_alpha > 0)
        {
            m_fadeQuads.clear();
            util::appendQuadVerts(
                context.layout.wholeRect(), m_fadeQuads, sf::Color(0, 0, 0, infoPack.fade_alpha));
        }
    }

    void BackgroundImages::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_backgroundSprite, states);

        for (const SlidingImage & image : m_slidingImages)
        {
            target.draw(image.sprite_left, states);
            target.draw(image.sprite_right, states);
        }

        target.draw(m_overlaySprite, states);

        target.draw(&m_fadeQuads[0], m_fadeQuads.size(), sf::Quads, states);
    }

    void BackgroundImages::move(const float amount)
    {
        for (SlidingImage & image : m_slidingImages)
        {
            image.sprite_left.move((amount * image.info.move_ratio), 0.0f);
            image.sprite_right.move((amount * image.info.move_ratio), 0.0f);

            if (image.sprite_right.getPosition().x < 0.0f)
            {
                image.sprite_left.setPosition(0.0f, image.sprite_left.getPosition().y);

                image.sprite_right.setPosition(
                    image.sprite_left.getGlobalBounds().width, image.sprite_right.getPosition().y);
            }
        }
    }

    const BackgroundImagesInfo
        BackgroundImages::infoFactory(const Context & context, const std::string & name)
    {
        const sf::Uint8 fadeAlpha{ 32 };

        if (name == "forest")
        {
            std::vector<SlidingImageInfo> slidingImages;

            slidingImages.push_back(
                { 0.2f,
                  (context.settings.media_path / "image/background/forest/clouds-back.png") });

            slidingImages.push_back(
                { 0.4f,
                  (context.settings.media_path / "image/background/forest/clouds-front.png") });

            slidingImages.push_back(
                { 0.6f, (context.settings.media_path / "image/background/forest/mountains.png") });

            slidingImages.push_back(
                { 0.7f, (context.settings.media_path / "image/background/forest/mist.png") });

            slidingImages.push_back(
                { 0.8f, (context.settings.media_path / "image/background/forest/trees.png") });

            BackgroundImagesInfo info(
                fadeAlpha,
                (context.settings.media_path / "image/background/forest/sky.png"),
                slidingImages,
                {});

            return info;
        }
        else if (name == "underground-swamp")
        {
            std::vector<SlidingImageInfo> slidingImages;

            slidingImages.push_back({ 0.2f,
                                      (context.settings.media_path /
                                       "image/background/underground-swamp/ruins-back.png") });

            slidingImages.push_back({ 0.4f,
                                      (context.settings.media_path /
                                       "image/background/underground-swamp/ruins-front.png") });

            slidingImages.push_back({ 0.6f,
                                      (context.settings.media_path /
                                       "image/background/underground-swamp/floor-back.png") });

            slidingImages.push_back({ 0.8f,
                                      (context.settings.media_path /
                                       "image/background/underground-swamp/floor-front.png") });

            slidingImages.push_back({ 0.8f,
                                      (context.settings.media_path /
                                       "image/background/underground-swamp/chains.png") });

            BackgroundImagesInfo info(
                fadeAlpha,
                (context.settings.media_path / "image/background/underground-swamp/background.png"),
                slidingImages,
                {});

            return info;
        }
        else if (name == "underground-cave")
        {
            std::vector<SlidingImageInfo> slidingImages;

            slidingImages.push_back({ 0.2f,
                                      (context.settings.media_path /
                                       "image/background/underground-cave/rock-back.png") });

            slidingImages.push_back({ 0.4f,
                                      (context.settings.media_path /
                                       "image/background/underground-cave/rock-middle.png") });

            slidingImages.push_back(
                { 0.6f,
                  (context.settings.media_path / "image/background/underground-cave/mist.png") });

            slidingImages.push_back({ 0.8f,
                                      (context.settings.media_path /
                                       "image/background/underground-cave/rock-front1.png") });

            slidingImages.push_back({ 0.8f,
                                      (context.settings.media_path /
                                       "image/background/underground-cave/rock-front2.png") });

            BackgroundImagesInfo info(
                fadeAlpha,
                (context.settings.media_path / "image/background/underground-cave/background.png"),
                slidingImages,
                {});

            return info;
        }
        else if (name == "mountains")
        {
            std::vector<SlidingImageInfo> slidingImages;

            slidingImages.push_back(
                { 0.1f, (context.settings.media_path / "image/background/mountains/clouds1.png") });

            slidingImages.push_back(
                { 0.2f, (context.settings.media_path / "image/background/mountains/clouds2.png") });

            slidingImages.push_back(
                { 0.4f,
                  (context.settings.media_path / "image/background/mountains/rocks-back.png") });

            slidingImages.push_back(
                { 0.6f,
                  (context.settings.media_path / "image/background/mountains/rocks-middle.png") });

            slidingImages.push_back(
                { 0.7f, (context.settings.media_path / "image/background/mountains/mist.png") });

            slidingImages.push_back(
                { 0.8f,
                  (context.settings.media_path / "image/background/mountains/rocks-front.png") });

            BackgroundImagesInfo info(
                fadeAlpha,
                (context.settings.media_path / "image/background/mountains/sky.png"),
                slidingImages,
                {});

            return info;
        }
        else if (name == "castle")
        {
            std::vector<SlidingImageInfo> slidingImages;

            slidingImages.push_back(
                { 0.1f, (context.settings.media_path / "image/background/castle/sky.png") });

            slidingImages.push_back(
                { 0.2f, (context.settings.media_path / "image/background/castle/trees.png") });

            slidingImages.push_back(
                { 0.4f, (context.settings.media_path / "image/background/castle/wall.png") });

            slidingImages.push_back(
                { 0.6f, (context.settings.media_path / "image/background/castle/floor.png") });

            slidingImages.push_back(
                { 0.8f, (context.settings.media_path / "image/background/castle/pillars.png") });

            BackgroundImagesInfo info(fadeAlpha, {}, slidingImages, {});

            return info;
        }
        else
        {
            std::cout << "Error: BackgroundImages::infoFactory(\"" << name
                      << "\") given unknown name.  No background will be shown on this map.\n";

            return BackgroundImagesInfo{ 0, {}, {}, {} };
        }
    }

} // namespace platformer
