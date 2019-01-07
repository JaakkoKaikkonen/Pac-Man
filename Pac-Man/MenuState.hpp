#pragma once
#include <SFML\Graphics.hpp>
#include "Game.hpp"
#include "State.hpp"
#include "GameState.hpp"

namespace engine {

	class MenuState : public State
	{
	public:
		MenuState(gameDataRef data);
		~MenuState();

		void init();

		void handleInput();
		void update();
		void draw();

	private:
		gameDataRef _data;

		sf::Sprite _logo;

		sf::Text _playText;

	};

}