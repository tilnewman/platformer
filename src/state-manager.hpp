#ifndef STATE_MANAGER_HPP_INCLUDED
#define STATE_MANAGER_HPP_INCLUDED
//
// state-manager.hpp
//
#include "states.hpp"

#include <memory>
#include <optional>

namespace platformer
{

    struct Context;

    //

    class StateManager
    {
      public:
        StateManager();

        [[nodiscard]] inline IState & current() const { return *m_currentStateUPtr; }

        void setChangePending(const State t_newState);
        
        [[nodiscard]] inline bool isChangePending() const { return m_changePendingOpt.has_value(); }
        
        // only coordinator.cpp should call this after all updates are finished()
        void changeIfPending(Context & context);

      private:
        static std::unique_ptr<IState> factory(const State t_state);

      private:
        std::unique_ptr<IState> m_currentStateUPtr;
        std::optional<State> m_changePendingOpt;
    };

} // namespace platformer

#endif // STATE_MANAGER_HPP_INCLUDED
