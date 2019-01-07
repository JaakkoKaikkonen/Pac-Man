#include "MenuState.hpp"

namespace engine {

	MenuState::MenuState(gameDataRef data)
		: _data(data), _logo(_data->assets.getTexture("Logo")), _playText("PLAY", _data->assets.getFont("Font"), 52)
	{
		_data->window.setMouseCursorVisible(true);

		_logo.setOrigin(_logo.getGlobalBounds().width / 2, _logo.getGlobalBounds().height / 2);
		_logo.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5);

		_playText.setOrigin(_playText.getGlobalBounds().width / 2, _playText.getGlobalBounds().height / 2);
		_playText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	}


	MenuState::~MenuState()
	{
	}


	void MenuState::init() {
		_data->assets.getSound("Intro").play();
	}

	void MenuState::handleInput() {
		sf::Event event;
		while (_data->window.pollEvent(event)) {
			if (sf::Event::Closed == event.type) {
				_data->window.close();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			_data->window.close();
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || 
			((sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) &&
			_playText.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(_data->window)))) {
			_data->machine.addState(stateRef(new GameState(_data)), true);
		}

	}

	void MenuState::update() {
		if (_playText.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(_data->window))) {
			_playText.setFillColor(sf::Color::Cyan);
		} else {
			_playText.setFillColor(sf::Color::White);
		}
	}

	void MenuState::draw() {
		_data->window.clear();

		_data->window.draw(_logo);
		_data->window.draw(_playText);

		_data->window.display();
	}


}