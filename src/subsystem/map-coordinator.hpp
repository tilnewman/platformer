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

    class MapCoordinator
    {
      public:
        MapCoordinator();

        void respawn(const Context & t_context);

        void deathBeforeDelay(const Context & t_context);
        void deathAfterDelay(const Context & t_context);

        [[nodiscard]] const std::string mapName() const;
        void mapNameAdvance();

      private:
        std::vector<std::string> m_mapNames;
        std::vector<std::string>::const_iterator m_mapNameIter;
    };

} // namespace bramblefore

#endif // MAP_COORDINATAOR_HPP_INCLUDED
