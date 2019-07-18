#include "MenuState.hpp"

namespace Game {

	MenuState::MenuState(gameDataRef data)
		: data(data),
		  logo(data->assets.getTexture("Logo")),
		  playText("PLAY", data->assets.getFont("Font"), 52)
	{

		logo.setOrigin(logo.getGlobalBounds().width / 2, logo.getGlobalBounds().height / 2);
		logo.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 6);

		playText.setOrigin(playText.getGlobalBounds().width / 2, playText.getGlobalBounds().height / 2);
		playText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	}


	void MenuState::init() {
		data->assets.getSound("Intro").play();
	}

	void MenuState::handleInput() {
		sf::Event event;
		while (data->window.pollEvent(event)) {
			if (sf::Event::Closed == event.type) {
				data->window.close();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			data->window.close();
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || 
			((sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) &&
			playText.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(data->window)))) {
			data->machine.addState(stateRef(new GameState(data)), true);
		}

	}

	void MenuState::update() {
		if (playText.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(data->window))) {
			playText.setFillColor(sf::Color::Cyan);
		} else {
			playText.setFillColor(sf::Color::White);
		}
	}

	void MenuState::draw() {
		data->window.clear();

		data->window.draw(logo);
		data->window.draw(playText);

		data->window.display();
	}


}