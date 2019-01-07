#include <string>
#include "GameState.hpp"
#include "MenuState.hpp"
#include "DEFINITIONS.hpp"
#include <iostream>
#include "Utility.hpp"
#include "Collision.hpp"



namespace engine {

	GameState::GameState(gameDataRef data)
		: _data(data), _map(_data->assets.getTexture("Map")), _pacman(_data), _hud(_data), _scoreText("200", _data->assets.getFont("Font"), 18)
	{
		_data->window.setMouseCursorVisible(false);

		//Setup map
		_map.setPosition(0.0f, TILESIZE*3);

		
		//Init points
		for (int i = 0; i < _points.size(); i++) {
			_points[i].init(_data, "Point");
		}

		for (int i = 0; i < _bigPoints.size(); i++) {
			_bigPoints[i].init(_data, "Big_point");
		}

		//Set up score sprite
		_scoreText.setOrigin(_scoreText.getGlobalBounds().width / 2, _scoreText.getGlobalBounds().height / 2);
		_scoreText.setFillColor(sf::Color::Cyan);
		
	}

	GameState::~GameState() {
		for (int i = 0; i < _ghosts.size(); i++) {
			delete _ghosts[i];
		}
	}

	void GameState::init() {
		std::cout << "Game state" << std::endl;


		//Setup ghosts
		_ghosts[0] = new Clyde(_data);
		_ghosts[1] = new Inky(_data);
		_ghosts[2] = new Pinky(_data);
		_ghosts[3] = new Blinky(_data);

		
	}

	void GameState::handleInput() {
		sf::Event event;
		while (_data->window.pollEvent(event)) {
			if (sf::Event::Closed == event.type) {
				_data->window.close();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			_data->window.close();
		}

		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
			_pacman.reset();
		}*/

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			_pacman.move(Dir::Right);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			_pacman.move(Dir::Left);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			_pacman.move(Dir::Up);
		} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
			_pacman.move(Dir::Down);
		}

	}

	void GameState::update() {

		
		if (_stopTimer.getElapsedTime().asSeconds() > 0.5f) {

			_stop = false;

			if (_pacman.isDead(_ghosts) || _win) {

				if (!_highScoreSaved) {
					_hud.saveHighScore();
					_highScoreSaved = true;
					_mapTimer.restart();
					_endTimer.restart();
				}

				if (_win) {
					_pacman.getSprite().setTextureRect(PACMAN_03);
					if (_mapTimer.getElapsedTime().asSeconds() < 0.25f) {
						_map.setTexture(_data->assets.getTexture("Map"));
					} else if (_mapTimer.getElapsedTime().asSeconds() < 0.50f) {
						_map.setTexture(_data->assets.getTexture("Map_white"));
					} else {
						_mapTimer.restart();
					}
				}

				if (_endTimer.getElapsedTime().asSeconds() > 2.0f) {
					_data->machine.addState(stateRef(new MenuState(_data)), true);
				}

			} else {

				if (_modeTimer.getElapsedTime().asSeconds() > _modeTimes[_modeIndex]) {
					_modeIndex++;
					_modeTimer.restart();
				}

				if (_pacman.power()) {
					_mode = Mode::Flee;
					for (int i = 0; i < _ghosts.size(); i++) {
						if (Collision::checkSpriteCollision(_pacman.getSprite(), 0.7f, _ghosts[i]->getSprite(), 1.0f) && !_ghosts[i]->getEyes()) {
							_ghosts[i]->reset();
							_stop = true;
							_ghostIndex = i;
							_hud.addToScore(200);
							_stopTimer.restart();
						}
					}
				} else {
					_mode = _modes[_modeIndex];
				}


				_pacman.update(_grid);


				Ghost::setMode(_mode);

				if (Ghost::findNewPath()) {
					for (int i = 0; i < _ghosts.size(); i++) {
						if (_ghosts[i]->isFree()) {
							_ghosts[i]->setTarget(_pacman.getPosition(), _pacman.getDir(), _ghosts[3]->getPosition(), _grid);
							_ghosts[i]->findPath(_grid);
						}
						_ghosts[i]->disableTurn();
					}
				}

				for (int i = 0; i < _ghosts.size(); i++) {
					_ghosts[i]->update();
				}

			}

		}

	}

	void GameState::draw() {

		_data->window.clear();

		_data->window.draw(_map);

		//Draw grid lines
		float y = 0;

		for (int i = 0; i < MAP_RES_Y; i++) {

			_line[0] = sf::Vertex(sf::Vector2f(0.0, y));
			_line[1] = sf::Vertex(sf::Vector2f(SCREEN_WIDTH, y));

			y += TILESIZE;

			_data->window.draw(_line, 2, sf::Lines);

		}

		float x = 0;

		for (int i = 0; i < MAP_RES_X; i++) {

			_line[0] = sf::Vertex(sf::Vector2f(x, 0.0));
			_line[1] = sf::Vertex(sf::Vector2f(x, SCREEN_HEIGHT));

			x += TILESIZE;

			_data->window.draw(_line, 2, sf::Lines);

		}

		//Draw points
		int pointCount = 0;
		int bigPointCount = 0;

		for (int i = 0; i < MAP_RES_Y; i++) {
			for (int j = 0; j < MAP_RES_X; j++) {
				if (_grid[i][j] == 2) {
					_points[pointCount].draw(j * TILESIZE + TILESIZE / 2, i * TILESIZE + TILESIZE / 2);
					pointCount++;
				} else if (_grid[i][j] == 3) {
					if (_bigPointFalshTimer.getElapsedTime().asSeconds() < 0.25f) {
						_bigPoints[bigPointCount].draw(j * TILESIZE + TILESIZE / 2, i * TILESIZE + TILESIZE / 2);
					}
					bigPointCount++;
				}
			}
		}

		//Big point flash timer
		if (_bigPointFalshTimer.getElapsedTime().asSeconds() > 0.5f) {
			_bigPointFalshTimer.restart();
		}

		if (pointCount + bigPointCount == 0) {
			_win = true;
		}

		_hud.drawScore(pointCount, bigPointCount);

		_hud.drawHighScore();

		if (!_stop) {
			_pacman.draw();
		}

		if (!_win) {
			for (int i = 0; i < _ghosts.size(); i++) {
				if (_stop && _ghostIndex == i) {
					_scoreText.setPosition(_ghosts[i]->getPosition());
					_data->window.draw(_scoreText);
				} else {
					_ghosts[i]->draw();
				}
			}
		}


		_data->window.display();

	}


}


