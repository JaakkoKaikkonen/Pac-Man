#include "Blinky.hpp"
#include "Utility.hpp"
#include <iostream>

namespace engine {

	Blinky::Blinky(gameDataRef data) :Ghost(data)
	{
		_ghost.setTexture(_data->assets.getTexture("Ghost"));
		_ghost.setTextureRect(BLINKY_RIGHT_01);
		_ghost.setOrigin(_ghost.getGlobalBounds().width / 2, _ghost.getGlobalBounds().height / 2);
		_ghost.setScale(1.25f, 1.25f);
		_ghost.setPosition(TILESIZE * 14, TILESIZE * 14 + TILESIZE / 2);

		_animationFramesRight[0] = BLINKY_RIGHT_01;
		_animationFramesRight[1] = BLINKY_RIGHT_02;

		_animationFramesLeft[0] = BLINKY_LEFT_01;
		_animationFramesLeft[1] = BLINKY_LEFT_02;

		_animationFramesUp[0] = BLINKY_UP_01;
		_animationFramesUp[1] = BLINKY_UP_02;

		_animationFramesDown[0] = BLINKY_DOWN_01;
		_animationFramesDown[1] = BLINKY_DOWN_02;
	}


	void Blinky::setTarget(const sf::Vector2f& pacmanPosition, const Dir& pacmandir, const sf::Vector2f& blinkyPosition, const int map[MAP_RES_Y][MAP_RES_X]) {

		if(!_eyes) {

			_previousTarget = _target;

			if (Mode::Chase == mode) {
			
				_target = sf::Vector2i((int)(pacmanPosition.x / TILESIZE), (int)(pacmanPosition.y / TILESIZE));

				//Clamp target to be on the grid
				_target = utility::clamp(_target, 1, MAP_RES_X - 2, 4, MAP_RES_Y - 4);

			} else if (Mode::Scatter == mode) {

				if (((sf::Vector2i)_ghost.getPosition() / TILESIZE) == BLINKYS_CORNER) {
					_target.x = BLINKYS_CORNER.x - 2;
					_target.y = BLINKYS_CORNER.y;
				} else {
					_target = BLINKYS_CORNER;
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
						_target = BLINKYS_CORNER;
					}

				}

			}

		} else if ((sf::Vector2i)_ghost.getPosition() / TILESIZE == _target) {
			_spot = Dir::Right;
			_go = true;
			_start = false;
			_boxCounter = 0;
			
		} else if ((int)_ghost.getPosition().x / TILESIZE == _target.x - 1 && (int)_ghost.getPosition().y / TILESIZE == _target.y) {
			_spot = Dir::Left;
			_go = true;
			_start = false;
			_boxCounter = 0;
		}

		/*std::cout << (int)_ghost.getPosition().x / TILESIZE << ", " << (int)_ghost.getPosition().y / TILESIZE << ", ";
		std::cout << _target.x << ", " << _target.y << std::endl;*/

	}

	
	void Blinky::boxAnimation() {
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
			} else if (_boxCounter == 42) {
				_dirVector = sf::Vector2f(0.0f, 0.0f);
				_target = BLINKYS_CORNER;
				_eyes = false;
				_go = false;
				_boxCounter = 0;
			}
		} else if (_start) {
			if (!_go) {
				_dirVector = sf::Vector2f(0.0f, 0.0f);
			} else if (_boxCounter < 6) {
				_dirVector = sf::Vector2f(2.0f, 0.0f);
			}
		} else {
			if (!_go) {
				_dirVector = sf::Vector2f(0.0f, 0.0f);
			} else if (_boxCounter < 36) {
				_dirVector = sf::Vector2f(0.0f, -2.0f);
			} else {
				_dirVector = sf::Vector2f(2.0f, 0.0f);
			}
		}

		_boxCounter++;
	}


	bool Blinky::isFree() {
		if (_start) {
			return _boxCounter > 6;
		} else {
			return _boxCounter > 42;
		}
	}


}