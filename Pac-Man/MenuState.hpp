#pragma once
#include <SFML\Graphics.hpp>
#include "Game.hpp"
#include "State.hpp"
#include "GameState.hpp"

namespace Game {

	class MenuState : public State
	{
	public:
		MenuState(gameDataRef data);

		void handleInput();
		void update();
		void draw();

	private:
		gameDataRef data;

		sf::Sprite logo;

		sf::Text playText;

	};

}