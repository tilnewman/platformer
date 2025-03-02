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

        void respawn(Context & t_context);

        void deathBeforeDelay(Context & t_context);
        void deathAfterDelay(Context & t_context);

        [[nodiscard]] std::string mapName() const;
        void mapNameAdvance();

      private:
        std::vector<std::string> m_mapNames;
        std::vector<std::string>::const_iterator m_mapNameIter;
    };

} // namespace bramblefore

#endif // MAP_COORDINATAOR_HPP_INCLUDED
