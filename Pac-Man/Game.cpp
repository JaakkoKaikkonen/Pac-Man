#include "Game.hpp"
#include "GameState.hpp"
#include "MenuState.hpp"
#include "DEFINITIONS.hpp"

#include <iostream>

namespace engine {

	Game::Game(int width, int height, std::string title) {

		sf::Image icon;
		icon.loadFromFile(TOP_ICON_FILEPATH);

		_data->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);

		_data->window.setPosition(sf::Vector2i(500, 100));

		_data->window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		srand(time(NULL));

		//Load Resources ----------------------------------------------------------------------

		//Textures
		_data->assets.loadTexture("Map", MAP_FILEPATH);
		_data->assets.loadTexture("Map_white", MAP_WHITE_FILEPATH);
		_data->assets.loadTexture("Pac-man", PACMAN_FILEPATH);
		_data->assets.loadTexture("Point", POINT_FILEPATH);
		_data->assets.loadTexture("Big_point", BIG_POINT_FILEPATH);
		_data->assets.loadTexture("Ghost", GHOST_FILEPATH);
		_data->assets.loadTexture("Logo", LOGO_FILEPATH);

		//Sounds
		_data->assets.loadSound("Intro", INTRO_MUSIC_FILEPATH);

		//Fonts
		_data->assets.loadFont("Font", FONT_FILEPATH);

		//-------------------------------------------------------------------------------------

		_data->machine.addState(stateRef(new MenuState(_data)), true);

		this->run();
	}

	void Game::run() {
		float newTime, frameTime;

		float currentTime = this->_clock.getElapsedTime().asSeconds();

		float accumulator = dt;

		float interpolation = 0.0f;

		
		while (this->_data->window.isOpen()) {

			this->_data->machine.processStateChanges();

			newTime = this->_clock.getElapsedTime().asSeconds();

			frameTime = newTime - currentTime;

			currentTime = newTime;

			if (frameTime > 0.15f) {
				frameTime = 0.15f;
			}

			accumulator += frameTime;

			while (accumulator >= dt)
			{
				this->_data->machine.getActiveState()->handleInput();
				this->_data->machine.getActiveState()->update();

				accumulator -= dt;
			
			}

			this->_data->machine.getActiveState()->draw();

			//FPS
			//std::cout << 1 / frameTime << std::endl;
			
		}


	}


}