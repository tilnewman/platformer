#ifndef BACKGROUNDIMAGES_HPP_INCLUDED
#define BACKGROUNDIMAGES_HPP_INCLUDED
//
// background-images.hpp
//
#include <filesystem>
#include <string>
#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace sf
{
    class RenderTarget;
    class RenderStates;
} // namespace sf

namespace platformer
{
    struct Context;

    //

    struct SlidingImageInfo
    {
        SlidingImageInfo()
            : move_ratio(0.0f)
            , path()
        {}

        SlidingImageInfo(const float moveRatio, const std::filesystem::path & filePath)
            : move_ratio(moveRatio)
            , path(filePath)
        {}

        float move_ratio;
        std::filesystem::path path;
    };

    //

    struct BackgroundImagesInfo
    {
        BackgroundImagesInfo(
            const sf::Uint8 fadeAlpha,
            const std::filesystem::path & backgroundPath,
            const std::vector<SlidingImageInfo> & slidingImages,
            const std::filesystem::path & overlayPath)
            : fade_alpha(fadeAlpha)
            , background_path(backgroundPath)
            , sliding_images(slidingImages)
            , overlay_path(overlayPath)
        {}

        sf::Uint8 fade_alpha;
        std::filesystem::path background_path;
        std::vector<SlidingImageInfo> sliding_images;
        std::filesystem::path overlay_path;
    };

    //

    struct SlidingImage
    {
        SlidingImageInfo info{};
        sf::Texture texture{};
        sf::Sprite sprite_left{};
        sf::Sprite sprite_right{};
    };

    //

    class BackgroundImages
    {
      public:
        BackgroundImages();

        void setup(const Context & context, const std::string & name);
        void draw(sf::RenderTarget & target, sf::RenderStates states) const;
        void move(const float amount);

      private:
        const BackgroundImagesInfo infoFactory(const Context & context, const std::string & name);

      private:
        std::string m_loadedSetName;
        sf::Texture m_backgroundTexture;
        sf::Texture m_overlayTexture;

        sf::Sprite m_backgroundSprite;
        sf::Sprite m_overlaySprite;

        std::vector<SlidingImage> m_slidingImages;

        std::vector<sf::Vertex> m_fadeQuads;
    };

} // namespace platformer

#endif // BACKGROUNDIMAGES_HPP_INCLUDED
