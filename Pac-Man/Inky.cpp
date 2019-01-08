#include "Inky.hpp"
#include "Utility.hpp"
#include <iostream>

namespace engine {

	Inky::Inky(gameDataRef data) :Ghost(data)
	{
		_ghost.setTexture(_data->assets.getTexture("Ghost"));
		_ghost.setTextureRect(INKY_RIGHT_01);
		_ghost.setOrigin(_ghost.getGlobalBounds().width / 2, _ghost.getGlobalBounds().height / 2);
		_ghost.setScale(1.25f, 1.25f);
		_ghost.setPosition(TILESIZE * 12, TILESIZE * 17 + TILESIZE / 2);

		_animationFramesRight[0] = INKY_RIGHT_01;
		_animationFramesRight[1] = INKY_RIGHT_02;

		_animationFramesLeft[0] = INKY_LEFT_01;
		_animationFramesLeft[1] = INKY_LEFT_02;

		_animationFramesUp[0] = INKY_UP_01;
		_animationFramesUp[1] = INKY_UP_02;

		_animationFramesDown[0] = INKY_DOWN_01;
		_animationFramesDown[1] = INKY_DOWN_02;
	}


	void Inky::setTarget(const sf::Vector2f& pacmanPosition, const Dir& pacmandir, const sf::Vector2f& blinkyPosition, const int map[MAP_RES_Y][MAP_RES_X]) {

		if (!_eyes) {

			if (Mode::Chase == mode) {

				sf::Vector2i point;
				if (Dir::Right == pacmandir) {
					point = sf::Vector2i((int)((pacmanPosition.x + 4 * TILESIZE) / TILESIZE), (int)(pacmanPosition.y / TILESIZE));
				} else if (Dir::Left == pacmandir) {
					point = sf::Vector2i((int)((pacmanPosition.x - 4 * TILESIZE) / TILESIZE), (int)(pacmanPosition.y / TILESIZE));
				} else if (Dir::Up == pacmandir) {
					point = sf::Vector2i((int)(pacmanPosition.x / TILESIZE), (int)((pacmanPosition.y - 4 * TILESIZE) / TILESIZE));
				} else if (Dir::Down == pacmandir) {
					point = sf::Vector2i((int)(pacmanPosition.x / TILESIZE), (int)((pacmanPosition.y + 4 * TILESIZE) / TILESIZE));
				}
				sf::Vector2i vector = (sf::Vector2i)blinkyPosition / TILESIZE - point;
				vector *= -1;
				_target = point + vector;

				//Clamp target to be on the grid
				_target = utility::clamp(_target, 1, MAP_RES_X - 2, 4, MAP_RES_Y - 4);

				//If target is wall, fix it
				if (map[_target.y][_target.x] == 1) {
					this->fixTarget(map);
				}

				//If target == its own position -> set new target
				if (_target == (sf::Vector2i)_ghost.getPosition() / TILESIZE) {
					_target = INKYS_CORNER;
				}

			} else if (Mode::Scatter == mode) {

				if (((sf::Vector2i)_ghost.getPosition() / TILESIZE) == INKYS_CORNER) {
					_target.x = INKYS_CORNER.x - 2;
					_target.y = INKYS_CORNER.y;
				} else {
					_target = INKYS_CORNER;
				}

			} else if (Mode::Flee == mode) {

				if (turn) {

					if (Dir::Right == _dir) {
						_target = sf::Vector2i((int)_ghost.getPosition().x / TILESIZE - 1, (int)_ghost.getPosition().y / TILESIZE);
					} else if (Dir::Left == _dir) {
						_target = sf::Vector2i((int)_ghost.getPosition().x / TILESIZE + 1, (int)_ghost.getPosition().y / TILESIZE);
					} else if (Dir::Down == _dir) {
						_target = sf::Vector2i((int)_ghost.getPosition().x / TILESIZE, (int)_ghost.getPosition().y / TILESIZE - 1);
					} else if (Dir::Up == _dir) {
						_target = sf::Vector2i((int)_ghost.getPosition().x / TILESIZE, (int)_ghost.getPosition().y / TILESIZE + 1);
					}

				} else {

					if (((sf::Vector2i)_ghost.getPosition() / TILESIZE) == _target) {
						_target = sf::Vector2i(rand() % 25 + 1, rand() % 28 + 4);
						if (map[_target.y][_target.x] == 1) {
							this->fixTarget(map);
						}
					}

					if (_teleport) {
						_target = INKYS_CORNER;
					}

				}
			}

		} else if ((sf::Vector2i)_ghost.getPosition() / TILESIZE == _target) {
			_spot = Dir::Right;
			_go = true;
			_boxCounter = 0;

		} else if ((int)_ghost.getPosition().x / TILESIZE == _target.x - 1 && (int)_ghost.getPosition().y / TILESIZE == _target.y) {
			_spot = Dir::Left;
			_go = true;
			_boxCounter = 0;
		}
		
	}


	void Inky::boxAnimation() {
	
		if (_startTimer.getElapsedTime().asSeconds() > 0.9f) {

			if (!_go && counter == 6) {
				_go = true;
				_boxCounter = 0;
			}

			if (_eyes) {
				if (_boxCounter < 6) {
					if (_spot == Dir::Right) {
						_dirVector = sf::Vector2f(-2.0f, 0.0f);
					} else {
						_dirVector = sf::Vector2f(2.0f, 0.0f);
					}
				} else if (_boxCounter < 42) {
					_dirVector = sf::Vector2f(0.0f, 2.0f);
				} else if (_boxCounter < 66) {
					_dirVector = sf::Vector2f(-2.0f, 0.0f);
				} else if (_boxCounter == 66) {
					_dirVector = sf::Vector2f(0.0f, 0.0f);
					_target = INKYS_CORNER;
					_eyes = false;
					_go = false;
					_boxCounter = 0;
				}
			} else if (!_go) {
				_dirVector = sf::Vector2f(0.0f, 0.0f);
			} else if (_boxCounter < 24) {
				_dirVector = sf::Vector2f(2.0f, 0.0f);
			} else if (_boxCounter < 60) {
				_dirVector = sf::Vector2f(0.0f, -2.0f);
			} else if (_boxCounter < 66) {
				_dirVector = sf::Vector2f(2.0f, 0.0f);
			}

			_boxCounter++;
		}
	}


	bool Inky::isFree() {
		return _boxCounter > 66;
	}
		

}