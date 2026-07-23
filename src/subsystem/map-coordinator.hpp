#ifndef MAP_COORDINATAOR_HPP_INCLUDED
#define MAP_COORDINATAOR_HPP_INCLUDED
//
// map-coordinator.hpp
//
#include <string>
#include <vector>

namespace bramblefore
{

    struct Context;

    //

    struct MapNames
    {
        MapNames(const std::string & t_file, const std::string & t_display)
            : file{t_file}
            , display{t_display}
        {}

        std::string file{};
        std::string display{};
    };

    //

    class MapCoordinator
    {
      public:
        MapCoordinator();

        void respawn(const Context & t_context);

        void deathBeforeDelay(const Context & t_context);
        void deathAfterDelay(const Context & t_context);

        [[nodiscard]] const std::string filename() const;
        [[nodiscard]] const std::string displayName() const;
        void advance();

      private:
        std::vector<MapNames> m_names;
        std::vector<MapNames>::const_iterator m_namesIter;
    };

} // namespace bramblefore

#endif // MAP_COORDINATAOR_HPP_INCLUDED
