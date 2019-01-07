#include "Clyde.hpp"
#include "Utility.hpp"

namespace engine {

	Clyde::Clyde(gameDataRef data) :Ghost(data)
	{
		_ghost.setTexture(_data->assets.getTexture("Ghost"));
		_ghost.setTextureRect(CLYDE_RIGHT_01);
		_ghost.setOrigin(_ghost.getGlobalBounds().width / 2, _ghost.getGlobalBounds().height / 2);
		_ghost.setScale(1.25f, 1.25f);
		_ghost.setPosition(TILESIZE * 16, TILESIZE * 17 + TILESIZE / 2);

		_animationFramesRight[0] = CLYDE_RIGHT_01;
		_animationFramesRight[1] = CLYDE_RIGHT_02;

		_animationFramesLeft[0] = CLYDE_LEFT_01;
		_animationFramesLeft[1] = CLYDE_LEFT_02;

		_animationFramesUp[0] = CLYDE_UP_01;
		_animationFramesUp[1] = CLYDE_UP_02;

		_animationFramesDown[0] = CLYDE_DOWN_01;
		_animationFramesDown[1] = CLYDE_DOWN_02;
	}


	void Clyde::setTarget(const sf::Vector2f& pacmanPosition, const Dir& pacmandir, const sf::Vector2f& blinkyPosition, const int map[MAP_RES_Y][MAP_RES_X]) {

		if (!_eyes) {

			_previousTarget = _target;

			if (Mode::Chase == mode) {

				sf::Vector2i distance((int)abs(_ghost.getPosition().x / TILESIZE - pacmanPosition.x / TILESIZE), (int)abs(_ghost.getPosition().y / TILESIZE - pacmanPosition.y / TILESIZE));

				if ((distance.x + distance.y) > 8) {
					_target = sf::Vector2i((int)(pacmanPosition.x / TILESIZE), (int)(pacmanPosition.y / TILESIZE));
				} else {
					_target = CLYDES_CORNER;
				}

				//Clamp target to be on the grid
				_target = utility::clamp(_target, 1, MAP_RES_X - 2, 4, MAP_RES_Y - 4);

				//If target == its own position -> set new target
				if (_target == (sf::Vector2i)_ghost.getPosition() / TILESIZE) {
					_target.x = CLYDES_CORNER.x + 2;
					_target.y = CLYDES_CORNER.y;
				}

			} else if (Mode::Scatter == mode) {

				if (((sf::Vector2i)_ghost.getPosition() / TILESIZE) == CLYDES_CORNER) {
					_target.x = CLYDES_CORNER.x + 2;
					_target.y = CLYDES_CORNER.y;
				} else {
					_target = CLYDES_CORNER;
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
						_target = CLYDES_CORNER;
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


	void Clyde::boxAnimation() {

		if (_startTimer.getElapsedTime().asSeconds() > 1.75f) {

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
					_dirVector = sf::Vector2f(2.0f, 0.0f);
				} else if (_boxCounter == 66) {
					_dirVector = sf::Vector2f(0.0f, 0.0f);
					_target = CLYDES_CORNER;
					_eyes = false;
					_go = false;
					_boxCounter = 0;
				}
			} else if (!_go) {
				_dirVector = sf::Vector2f(0.0f, 0.0f);
			} else if (_boxCounter < 24) {
				_dirVector = sf::Vector2f(-2.0f, 0.0f);
			} else if (_boxCounter < 60) {
				_dirVector = sf::Vector2f(0.0f, -2.0f);
			} else if (_boxCounter < 66) {
				_dirVector = sf::Vector2f(-2.0f, 0.0f);
			}

			_boxCounter++;
		}
	}


	bool Clyde::isFree() {
		return _boxCounter > 66;
	}


}
