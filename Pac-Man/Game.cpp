#include "Game.hpp"
#include "GameState.hpp"
#include "MenuState.hpp"
#include "DEFINITIONS.hpp"

#include <iostream>

namespace Game {

	Game::Game(int width, int height, std::string title) {

		sf::Image icon;
		icon.loadFromFile(TOP_ICON_FILEPATH);

		data->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);

		data->window.setPosition(sf::Vector2i(500, 100));

		data->window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		srand((unsigned int)time(NULL));

		//Load Resources ----------------------------------------------------------------------

		//Textures
		data->assets.loadTexture("Map", MAP_FILEPATH);
		data->assets.loadTexture("Map_white", MAP_WHITE_FILEPATH);
		data->assets.loadTexture("Pac-man", PACMAN_FILEPATH);
		data->assets.loadTexture("Point", POINT_FILEPATH);
		data->assets.loadTexture("Big_point", BIG_POINT_FILEPATH);
		data->assets.loadTexture("Ghost", GHOST_FILEPATH);
		data->assets.loadTexture("Logo", LOGO_FILEPATH);

		//Sounds
		data->assets.loadSound("Intro", INTRO_MUSIC_FILEPATH);

		//Fonts
		data->assets.loadFont("Font", FONT_FILEPATH);

		//-------------------------------------------------------------------------------------

		data->state = new MenuState(data);

		this->run();
	}

	void Game::run() {
		float newTime, frameTime;

		float currentTime = clock.getElapsedTime().asSeconds();

		float accumulator = dt;

		
		while (this->data->window.isOpen()) {

			//Check state changes
			if (data->newState) {
				delete data->state;
				data->state = data->newState;
				data->newState = nullptr;
			}


			newTime = clock.getElapsedTime().asSeconds();

			frameTime = newTime - currentTime;

			currentTime = newTime;

			if (frameTime > 0.15f) {
				frameTime = 0.15f;
			}

			accumulator += frameTime;

			while (accumulator >= dt) {
				data->state->handleInput();
				data->state->update();

				data->state->draw();

				accumulator -= dt;
			}
			
		}


	}


}