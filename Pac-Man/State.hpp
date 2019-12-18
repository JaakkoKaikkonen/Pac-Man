#pragma once

namespace Game {

	class State {

	public:
		virtual void handleInput() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;

		virtual ~State() {};

	};

}