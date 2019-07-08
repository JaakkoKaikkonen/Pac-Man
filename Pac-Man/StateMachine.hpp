#pragma once


#include <memory>
#include <stack>

#include "State.hpp"

namespace Game {

	typedef std::unique_ptr<State> stateRef;

	class StateMachine {
	public:
		StateMachine() {}
		~StateMachine() {}

		void addState(stateRef newState, bool isReplacing = true);
		void removeState();

		void processStateChanges();

		stateRef& getActiveState();

	private:
		std::stack<stateRef> states;
		stateRef newState;

		bool isRemoving;
		bool isAdding;
		bool isReplacing;

	};

}
