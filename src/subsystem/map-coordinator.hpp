#ifndef MAP_COORDINATAOR_HPP_INCLUDED
#define MAP_COORDINATAOR_HPP_INCLUDED
//
// map-coordinator.hpp
//

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
    };

} // namespace bramblefore

#endif // MAP_COORDINATAOR_HPP_INCLUDED
