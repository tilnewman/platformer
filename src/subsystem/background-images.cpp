// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// background-images.cpp
//
#include "subsystem/background-images.hpp"

#include "bramblefore/settings.hpp"
#include "subsystem/context.hpp"
#include "subsystem/screen-layout.hpp"
#include "util/check-macros.hpp"
#include "util/sfml-util.hpp"
#include "util/texture-loader.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace bramblefore
{

    BackgroundImages::BackgroundImages()
        : m_loadedSetName{}
        , m_backgroundTexture{}
        , m_overlayTexture{}
        , m_backgroundSprite{ m_backgroundTexture }
        , m_overlaySprite{ m_overlayTexture }
        , m_slidingImages{}
        , m_fadeVerts{}
    {
        m_fadeVerts.reserve(util::verts_per_quad);
    }

    void BackgroundImages::setup(const Context & t_context, const std::string & t_name)
    {
        if (t_name == m_loadedSetName)
        {
            return;
        }

        m_loadedSetName = t_name;

        const BackgroundImagesInfo infoPack{ infoFactory(t_context, t_name) };

        // there won't always be a static background image
        if (!infoPack.background_path.empty())
        {
            util::TextureLoader::load(m_backgroundTexture, infoPack.background_path);
            m_backgroundSprite.setTexture(m_backgroundTexture, true);
            util::scaleAndCenterInside(m_backgroundSprite, t_context.layout.wholeRect());
        }
        
        // there won't always be a static overlay image
        if (!infoPack.overlay_path.empty())
        {
            util::TextureLoader::load(m_overlayTexture, infoPack.overlay_path);
            m_overlaySprite.setTexture(m_overlayTexture, true);
            util::scaleAndCenterInside(m_overlaySprite, t_context.layout.wholeRect());
        }

        //
       
        m_slidingImages.clear();
        m_slidingImages.reserve(16); // prevents reallocation

        for (const SlidingImageInfo & info : infoPack.sliding_images)
        {
            M_CHECK(
                std::filesystem::exists(info.path),
                "Error:  BackgroundImages::setup() given a path that doesn't exist: " << info.path);

            SlidingImage & slidingImage{ m_slidingImages.emplace_back() };
            slidingImage.info = info;
            util::TextureLoader::load(slidingImage.texture, info.path);
            slidingImage.sprite_left.setTexture(slidingImage.texture, true);
            slidingImage.sprite_right.setTexture(slidingImage.texture, true);
            util::scaleAndCenterInside(slidingImage.sprite_left, t_context.layout.wholeRect());
            util::scaleAndCenterInside(slidingImage.sprite_right, t_context.layout.wholeRect());

            slidingImage.sprite_right.move(
                { slidingImage.sprite_left.getGlobalBounds().size.x, 0.0f });
        }

        //

        m_fadeVerts.clear();
        if (infoPack.fade_alpha > 0)
        {
            util::appendTriangleVerts(
                t_context.layout.wholeRect(), m_fadeVerts, sf::Color(0, 0, 0, infoPack.fade_alpha));
        }
    }

    void BackgroundImages::draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const
    {
        t_target.draw(m_backgroundSprite, t_states);

        for (const SlidingImage & image : m_slidingImages)
        {
            t_target.draw(image.sprite_left, t_states);
            t_target.draw(image.sprite_right, t_states);
        }

        t_target.draw(m_overlaySprite, t_states);

        if (!m_fadeVerts.empty())
        {
            t_target.draw(
                &m_fadeVerts[0], m_fadeVerts.size(), sf::PrimitiveType::Triangles, t_states);
        }
    }

    void BackgroundImages::move(const float t_amount)
    {
        for (SlidingImage & image : m_slidingImages)
        {
            image.sprite_left.move({ (t_amount * image.info.move_ratio), 0.0f });
            image.sprite_right.move({ (t_amount * image.info.move_ratio), 0.0f });

            if (image.sprite_right.getPosition().x < 0.0f)
            {
                image.sprite_left.setPosition({ 0.0f, image.sprite_left.getPosition().y });

                image.sprite_right.setPosition({ image.sprite_left.getGlobalBounds().size.x,
                                                 image.sprite_right.getPosition().y });
            }
        }
    }

    BackgroundImagesInfo
        BackgroundImages::infoFactory(const Context & t_context, const std::string & t_name)
    {
        // behold the magic number that looked best on screen after many tests
        const std::uint8_t fadeAlpha{ 64 };

        if (t_name == "forest")
        {
            std::vector<SlidingImageInfo> slidingImages;

            slidingImages.push_back(
                { 0.2f,
                  (t_context.settings.media_path / "image/background/forest/clouds-back.png") });

            slidingImages.push_back(
                { 0.4f,
                  (t_context.settings.media_path / "image/background/forest/clouds-front.png") });

            slidingImages.push_back(
                { 0.6f,
                  (t_context.settings.media_path / "image/background/forest/mountains.png") });

            slidingImages.push_back(
                { 0.7f, (t_context.settings.media_path / "image/background/forest/mist.png") });

            slidingImages.push_back(
                { 0.8f, (t_context.settings.media_path / "image/background/forest/trees.png") });

            BackgroundImagesInfo info(
                fadeAlpha,
                (t_context.settings.media_path / "image/background/forest/sky.png"),
                slidingImages,
                {});

            return info;
        }
        else if (t_name == "dungeon")
        {
            std::vector<SlidingImageInfo> slidingImages;

            slidingImages.push_back(
                { 0.2f,
                  (t_context.settings.media_path / "image/background/dungeon/ruins-back.png") });

            slidingImages.push_back(
                { 0.4f,
                  (t_context.settings.media_path / "image/background/dungeon/ruins-front.png") });

            slidingImages.push_back(
                { 0.6f,
                  (t_context.settings.media_path / "image/background/dungeon/floor-back.png") });

            slidingImages.push_back(
                { 0.8f,
                  (t_context.settings.media_path / "image/background/dungeon/floor-front.png") });

            slidingImages.push_back(
                { 0.8f, (t_context.settings.media_path / "image/background/dungeon/chains.png") });

            BackgroundImagesInfo info(
                fadeAlpha,
                (t_context.settings.media_path / "image/background/dungeon/background.png"),
                slidingImages,
                {});

            return info;
        }
        else if (t_name == "cave")
        {
            std::vector<SlidingImageInfo> slidingImages;

            slidingImages.push_back(
                { 0.2f, (t_context.settings.media_path / "image/background/cave/rock-back.png") });

            slidingImages.push_back(
                { 0.4f,
                  (t_context.settings.media_path / "image/background/cave/rock-middle.png") });

            slidingImages.push_back(
                { 0.6f, (t_context.settings.media_path / "image/background/cave/mist.png") });

            slidingImages.push_back(
                { 0.8f,
                  (t_context.settings.media_path / "image/background/cave/rock-front1.png") });

            slidingImages.push_back(
                { 0.8f,
                  (t_context.settings.media_path / "image/background/cave/rock-front2.png") });

            BackgroundImagesInfo info(
                fadeAlpha,
                (t_context.settings.media_path / "image/background/cave/background.png"),
                slidingImages,
                {});

            return info;
        }
        else if (t_name == "mountain")
        {
            std::vector<SlidingImageInfo> slidingImages;

            slidingImages.push_back(
                { 0.1f,
                  (t_context.settings.media_path / "image/background/mountain/clouds1.png") });

            slidingImages.push_back(
                { 0.2f,
                  (t_context.settings.media_path / "image/background/mountain/clouds2.png") });

            slidingImages.push_back(
                { 0.4f,
                  (t_context.settings.media_path / "image/background/mountain/rocks-back.png") });

            slidingImages.push_back(
                { 0.6f,
                  (t_context.settings.media_path / "image/background/mountain/rocks-middle.png") });

            slidingImages.push_back(
                { 0.7f, (t_context.settings.media_path / "image/background/mountain/mist.png") });

            slidingImages.push_back(
                { 0.8f,
                  (t_context.settings.media_path / "image/background/mountain/rocks-front.png") });

            BackgroundImagesInfo info(
                fadeAlpha,
                (t_context.settings.media_path / "image/background/mountain/sky.png"),
                slidingImages,
                {});

            return info;
        }
        else if (t_name == "castle")
        {
            std::vector<SlidingImageInfo> slidingImages;

            slidingImages.push_back(
                { 0.1f, (t_context.settings.media_path / "image/background/castle/sky.png") });

            slidingImages.push_back(
                { 0.2f, (t_context.settings.media_path / "image/background/castle/trees.png") });

            slidingImages.push_back(
                { 0.4f, (t_context.settings.media_path / "image/background/castle/wall.png") });

            slidingImages.push_back(
                { 0.6f, (t_context.settings.media_path / "image/background/castle/floor.png") });

            slidingImages.push_back(
                { 0.8f, (t_context.settings.media_path / "image/background/castle/pillars.png") });

            BackgroundImagesInfo info(fadeAlpha, {}, slidingImages, {});

            return info;
        }
        else
        {
            util::log() << "\"" << t_name
                        << "\" is an unknown name.  No background will be shown on this map.\n";

            return BackgroundImagesInfo{ 0, {}, {}, {} };
        }
    }

} // namespace bramblefore
