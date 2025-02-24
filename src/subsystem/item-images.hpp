#ifndef ITEM_IMAGES_HPP_INCLUDED
#define ITEM_IMAGES_HPP_INCLUDED
//
// item-images.hpp
//
#include <string_view>
#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace bramblefore
{
    struct Context;
    struct Settings;

    //

    enum class Item : std::size_t
    {
        Bone = 0,
        Berries,
        BlackPearl,
        Bottle1,
        Bottle2,
        Bottle3,
        Bottle4,
        Bottle5,
        Bottle6,
        Bottle7,
        Bottle8,
        Bottle9,
        Bottle10,
        Bottle11,
        Bottle12,
        Bottle13,
        Bottle14,
        Bottle15,
        Bottle16,
        Bottle17,
        Bottle18,
        Bottle19,
        Bottle20,
        Bottle21,
        Bottle22,
        Bottle23,
        Bottle24,
        ButterflyWing,
        Container1,
        Container2,
        Cotton,
        Crown,
        Crystal1,
        Crystal2,
        Crystal3,
        Crystal4,
        Crystal5,
        DriedHerb,
        EagleFeather,
        Egg,
        FlowerDaisy,
        FlowerRose,
        FlowerSunflower,
        Fruit,
        Goblet,
        Herb1,
        Herb2,
        Herb3,
        Jevellery1,
        Jevellery2,
        Key,
        Leaf,
        Meat,
        Mint,
        Mushroom1,
        Mushroom2,
        Mushroom3,
        Mushroom4,
        Nuts,
        PineNeedles,
        RedPepper,
        Ring1,
        Ring2,
        Seed,
        Count
    };

    [[nodiscard]] inline constexpr std::string_view toString(const Item t_item) noexcept
    {
        // clang-format off
        switch (t_item)
        {
            case Item::Bone:            { return "Bone";            }
            case Item::Berries:         { return "Berries";         }
            case Item::BlackPearl:      { return "BlackPearl";      }
            case Item::Bottle1:         { return "Bottle1";         }
            case Item::Bottle2:         { return "Bottle2";         }
            case Item::Bottle3:         { return "Bottle3";         }
            case Item::Bottle4:         { return "Bottle4";         }
            case Item::Bottle5:         { return "Bottle5";         }
            case Item::Bottle6:         { return "Bottle6";         }
            case Item::Bottle7:         { return "Bottle7";         }
            case Item::Bottle8:         { return "Bottle8";         }
            case Item::Bottle9:         { return "Bottle9";         }
            case Item::Bottle10:        { return "Bottle10";        }
            case Item::Bottle11:        { return "Bottle11";        }
            case Item::Bottle12:        { return "Bottle12";        }
            case Item::Bottle13:        { return "Bottle13";        }
            case Item::Bottle14:        { return "Bottle14";        }
            case Item::Bottle15:        { return "Bottle15";        }
            case Item::Bottle16:        { return "Bottle16";        }
            case Item::Bottle17:        { return "Bottle17";        }
            case Item::Bottle18:        { return "Bottle18";        }
            case Item::Bottle19:        { return "Bottle19";        }
            case Item::Bottle20:        { return "Bottle20";        }
            case Item::Bottle21:        { return "Bottle21";        }
            case Item::Bottle22:        { return "Bottle22";        }
            case Item::Bottle23:        { return "Bottle23";        }
            case Item::Bottle24:        { return "Bottle24";        }
            case Item::ButterflyWing:   { return "ButterflyWing";   }
            case Item::Container1:      { return "Container1";      }
            case Item::Container2:      { return "Container2";      }
            case Item::Cotton:          { return "Cotton";          }
            case Item::Crown:           { return "Crown";           }
            case Item::Crystal1:        { return "Crystal1";        }
            case Item::Crystal2:        { return "Crystal2";        }
            case Item::Crystal3:        { return "Crystal3";        }
            case Item::Crystal4:        { return "Crystal4";        }
            case Item::Crystal5:        { return "Crystal5";        }
            case Item::DriedHerb:       { return "DriedHerb";       }
            case Item::EagleFeather:    { return "EagleFeather";    }
            case Item::Egg:             { return "Egg";             }
            case Item::FlowerDaisy:     { return "FlowerDaisy";     }
            case Item::FlowerRose:      { return "FlowerRose";      }
            case Item::FlowerSunflower: { return "FlowerSunflower"; }
            case Item::Fruit:           { return "Fruit";           }
            case Item::Goblet:          { return "Goblet";          }
            case Item::Herb1:           { return "Herb1";           }
            case Item::Herb2:           { return "Herb2";           }
            case Item::Herb3:           { return "Herb3";           }
            case Item::Jevellery1:      { return "Jevellery1";      }
            case Item::Jevellery2:      { return "Jevellery2";      }
            case Item::Key:             { return "Key";             }
            case Item::Leaf:            { return "Leaf";            }
            case Item::Meat:            { return "Meat";            }
            case Item::Mint:            { return "Mint";            }
            case Item::Mushroom1:       { return "Mushroom1";       }
            case Item::Mushroom2:       { return "Mushroom2";       }
            case Item::Mushroom3:       { return "Mushroom3";       }
            case Item::Mushroom4:       { return "Mushrum4";        }
            case Item::Nuts:            { return "Nuts";            }
            case Item::PineNeedles:     { return "PineNeedles";     }
            case Item::RedPepper:       { return "RedPepper";       }
            case Item::Ring1:           { return "Ring1";           }
            case Item::Ring2:           { return "Ring2";           }
            case Item::Seed:            { return "Seed";            }
            case Item::Count:           [[fallthrough]];
            default:      { return "Error_toString()_unknown_Item"; }
        }
        // clang-format on
    }

    [[nodiscard]] inline sf::IntRect textureRect(const Item item) noexcept
    {
        // clang-format off
        switch (item)
        {
            case Item::Bone:            { return {   0,   0, 32, 32 }; }
            case Item::Berries:         { return {  32,   0, 32, 32 }; }
            case Item::BlackPearl:      { return {  64,   0, 32, 32 }; }
            case Item::Bottle1:         { return {  96,   0, 32, 32 }; }
            case Item::Bottle2:         { return { 128,   0, 32, 32 }; }
            case Item::Bottle3:         { return { 160,   0, 32, 32 }; }
            case Item::Bottle4:         { return { 192,   0, 32, 32 }; }
            case Item::Bottle5:         { return { 224,   0, 32, 32 }; }
            case Item::Bottle6:         { return {   0,  32, 32, 32 }; }
            case Item::Bottle7:         { return {  32,  32, 32, 32 }; }
            case Item::Bottle8:         { return {  64,  32, 32, 32 }; }
            case Item::Bottle9:         { return {  96,  32, 32, 32 }; }
            case Item::Bottle10:        { return { 128,  32, 32, 32 }; }
            case Item::Bottle11:        { return { 160,  32, 32, 32 }; }
            case Item::Bottle12:        { return { 192,  32, 32, 32 }; }
            case Item::Bottle13:        { return { 224,  32, 32, 32 }; }
            case Item::Bottle14:        { return {   0,  64, 32, 32 }; }
            case Item::Bottle15:        { return {  32,  64, 32, 32 }; }
            case Item::Bottle16:        { return {  64,  64, 32, 32 }; }
            case Item::Bottle17:        { return {  96,  64, 32, 32 }; }
            case Item::Bottle18:        { return { 128,  64, 32, 32 }; }
            case Item::Bottle19:        { return { 160,  64, 32, 32 }; }
            case Item::Bottle20:        { return { 192,  64, 32, 32 }; }
            case Item::Bottle21:        { return { 224,  64, 32, 32 }; }
            case Item::Bottle22:        { return {   0,  96, 32, 32 }; }
            case Item::Bottle23:        { return {  32,  96, 32, 32 }; }
            case Item::Bottle24:        { return {  64,  96, 32, 32 }; }
            case Item::ButterflyWing:   { return {  96,  96, 32, 32 }; }
            case Item::Container1:      { return { 128,  96, 32, 32 }; }
            case Item::Container2:      { return { 160,  96, 32, 32 }; }
            case Item::Cotton:          { return { 192,  96, 32, 32 }; }
            case Item::Crown:           { return { 224,  96, 32, 32 }; }
            case Item::Crystal1:        { return {   0, 128, 32, 32 }; }
            case Item::Crystal2:        { return {  32, 128, 32, 32 }; }
            case Item::Crystal3:        { return {  64, 128, 32, 32 }; }
            case Item::Crystal4:        { return {  96, 128, 32, 32 }; }
            case Item::Crystal5:        { return { 128, 128, 32, 32 }; }
            case Item::DriedHerb:       { return { 160, 128, 32, 32 }; }
            case Item::EagleFeather:    { return { 192, 128, 32, 32 }; }
            case Item::Egg:             { return { 224, 128, 32, 32 }; }
            case Item::FlowerDaisy:     { return {   0, 160, 32, 32 }; }
            case Item::FlowerRose:      { return {  32, 160, 32, 32 }; }
            case Item::FlowerSunflower: { return {  64, 160, 32, 32 }; }
            case Item::Fruit:           { return {  96, 160, 32, 32 }; }
            case Item::Goblet:          { return { 128, 160, 32, 32 }; }
            case Item::Herb1:           { return { 160, 160, 32, 32 }; }
            case Item::Herb2:           { return { 192, 160, 32, 32 }; }
            case Item::Herb3:           { return { 224, 160, 32, 32 }; }
            case Item::Jevellery1:      { return {   0, 192, 32, 32 }; }
            case Item::Jevellery2:      { return {  32, 192, 32, 32 }; }
            case Item::Key:             { return {  64, 192, 32, 32 }; }
            case Item::Leaf:            { return {  96, 192, 32, 32 }; }
            case Item::Meat:            { return { 128, 192, 32, 32 }; }
            case Item::Mint:            { return { 160, 192, 32, 32 }; }
            case Item::Mushroom1:       { return { 192, 192, 32, 32 }; }
            case Item::Mushroom2:       { return { 224, 192, 32, 32 }; }
            case Item::Mushroom3:       { return {   0, 224, 32, 32 }; }
            case Item::Mushroom4:       { return {  32, 224, 32, 32 }; }
            case Item::Nuts:            { return {  64, 224, 32, 32 }; }
            case Item::PineNeedles:     { return {  96, 224, 32, 32 }; }
            case Item::RedPepper:       { return { 128, 224, 32, 32 }; }
            case Item::Ring1:           { return { 160, 224, 32, 32 }; }
            case Item::Ring2:           { return { 192, 224, 32, 32 }; }
            case Item::Seed:            { return { 224, 224, 32, 32 }; }
            case Item::Count:           [[fallthrough]];
            default:                    { return {   0,   0, 32, 32 }; }
        }
        // clang-format on
    }

    //

    class ItemImages
    {
      public:
        ItemImages();

        void setup(const Settings & t_settings);

        [[nodiscard]] sf::Sprite sprite(const Item t_item) const noexcept
        {
            return sf::Sprite(m_iconsTexture, textureRect(t_item));
        }

        [[nodiscard]] sf::Sprite background() const noexcept
        {
            return sf::Sprite(m_backgroundTexture);
        }

      private:
        sf::Texture m_iconsTexture;
        sf::Texture m_backgroundTexture;
    };

} // namespace bramblefore

#endif // ITEM_IMAGES_HPP_INCLUDED
