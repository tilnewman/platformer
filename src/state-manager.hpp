#ifndef STATE_MANAGER_HPP_INCLUDED
#define STATE_MANAGER_HPP_INCLUDED
//
// state-manager.hpp
//
#include "states.hpp"

#include <memory>

namespace platformer
{

    struct Context;

    //

    class StateManager
    {
      public:
        StateManager();

        inline IState & current() const { return *m_currentStateUPtr; }
        void changeTo(Context & context, const State newState);

      private:
        static std::unique_ptr<IState> factory(const State state);

      private:
        std::unique_ptr<IState> m_currentStateUPtr;
    };

} // namespace platformer

#endif // STATE_MANAGER_HPP_INCLUDED
