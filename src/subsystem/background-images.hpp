#ifndef BACKGROUNDIMAGES_HPP_INCLUDED
#define BACKGROUNDIMAGES_HPP_INCLUDED
//
// background-images.hpp
//
#include <filesystem>
#include <string>
#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace sf
{
    class RenderTarget;
} // namespace sf

namespace bramblefore
{
    struct Context;

    //

    struct SlidingImageInfo
    {
        SlidingImageInfo();

        explicit SlidingImageInfo(
            const float t_moveRatio, const std::filesystem::path & t_filePath);

        float move_ratio;
        std::filesystem::path path;
    };

    //

    struct BackgroundImagesInfo
    {
        explicit BackgroundImagesInfo(
            const std::uint8_t t_fadeAlpha,
            const std::filesystem::path & t_backgroundPath,
            const std::vector<SlidingImageInfo> & t_slidingImages,
            const std::filesystem::path & t_overlayPath);

        std::uint8_t fade_alpha;
        std::filesystem::path background_path;
        std::vector<SlidingImageInfo> sliding_images;
        std::filesystem::path overlay_path;
    };

    //

    struct SlidingImage
    {
        explicit SlidingImage(
            const SlidingImageInfo & t_info, const sf::FloatRect & t_wholeScreenRect);

        SlidingImageInfo info;
        sf::Texture texture;
        sf::Sprite sprite_left;
        sf::Sprite sprite_right;
    };

    //

    class BackgroundImages
    {
      public:
        BackgroundImages();

        void setup(const Context & t_context, const std::string & t_name);
        void draw(sf::RenderTarget & t_target, sf::RenderStates t_states) const;
        void move(const float t_amount);

      private:
        [[nodiscard]] BackgroundImagesInfo
            infoFactory(const Context & t_context, const std::string & t_name);

      private:
        std::string m_loadedSetName;
        sf::Texture m_backgroundTexture;
        sf::Texture m_overlayTexture;

        sf::Sprite m_backgroundSprite;
        sf::Sprite m_overlaySprite;

        std::vector<SlidingImage> m_slidingImages;

        // the whole background is covered in faded black to tone it down
        std::vector<sf::Vertex> m_fadeVerts;
    };

} // namespace bramblefore

#endif // BACKGROUNDIMAGES_HPP_INCLUDED
