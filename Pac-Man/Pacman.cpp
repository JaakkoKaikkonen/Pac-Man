#include "Pacman.hpp"
#include "Collision.hpp"
#include <iostream>

namespace engine {

	Pacman::Pacman(gameDataRef data)
		: _data(data), _pacman(_data->assets.getTexture("Pac-man"), PACMAN_03), _pacmanAnimation(_pacman, _pacmanAniamtionFrames, 4, PACMAN_ANIMATION_TIME),
		_teleport1(-TILESIZE*4, TILESIZE*16 + TILESIZE/2, TILESIZE*4 + 5, TILESIZE*2),
		_teleport2(SCREEN_WIDTH - 5, TILESIZE*16 + TILESIZE/2, TILESIZE*4, TILESIZE*2)
	{
		_pacman.setOrigin(_pacman.getGlobalBounds().width / 2, _pacman.getGlobalBounds().height / 2);
		_pacman.setScale(1.25f, 1.25f);
		_pacman.setPosition(336.0f, 636.0f);
	}

	Pacman::~Pacman()
	{
	}

	void Pacman::update(int map[MAP_RES_Y][MAP_RES_X]) {

		if (!_stopped) {

			//check if pacman should animate
			if (_previousPos == _pacman.getPosition()) {
				_pacman.setTextureRect(PACMAN_01);
			} else {
				_pacmanAnimation.animate();
			}

			//Teleports
			if (_teleport1.contains((int)(_pacman.getPosition().x - _pacman.getGlobalBounds().width / 2), (int)_pacman.getPosition().y)) {
				if (_teleportTimer.getElapsedTime().asSeconds() < 0.4f) {
					_pacman.move(PACMAN_SPEED, 0.0f);
					return;
				}
				_pacman.move(-PACMAN_SPEED, 0.0f);
				if (_pacman.getPosition().x + _pacman.getGlobalBounds().width < 0) {
					_pacman.setPosition(_teleport2.left + _pacman.getGlobalBounds().width, _teleport2.top + _pacman.getGlobalBounds().height / 2 + 4);
					_teleportTimer.restart();
				}
				return;
			}

			if (_teleport2.contains((int)(_pacman.getPosition().x + _pacman.getGlobalBounds().width / 2), (int)_pacman.getPosition().y)) {
				if (_teleportTimer.getElapsedTime().asSeconds() < 0.4f) {
					_pacman.move(-PACMAN_SPEED, 0.0f);
					return;
				}
				_pacman.move(PACMAN_SPEED, 0.0f);
				if (_pacman.getPosition().x - _pacman.getGlobalBounds().width > SCREEN_WIDTH) {
					_pacman.setPosition(_teleport1.left + _pacman.getGlobalBounds().width + 20, _teleport1.top + _pacman.getGlobalBounds().height / 2 + 4);
					_teleportTimer.restart();
				}
				return;
			}


			//Save previous position
			_previousPos = _pacman.getPosition();


			int x, y;

			//Movement
			for (int tries = 0; tries < 2; tries++) {

				switch (_dir1) {

				case Dir::Right:
					_pacman.move(PACMAN_SPEED, 0.0f);
					_pacman.setRotation(0.0f);
					break;
				case Dir::Left:
					_pacman.move(-PACMAN_SPEED, 0.0f);
					_pacman.setRotation(180.0f);
					break;
				case Dir::Up:
					_pacman.move(0.0f, -PACMAN_SPEED);
					_pacman.setRotation(-90.0f);
					break;
				case Dir::Down:
					_pacman.move(0.0f, PACMAN_SPEED);
					_pacman.setRotation(90.0f);
				}


				//Check Collision

				x = (int)(_pacman.getPosition().x + (TILESIZE / 2 - 1)) / TILESIZE;
				y = (int)(_pacman.getPosition().y + (TILESIZE / 2 - 1)) / TILESIZE;

				if (map[y][x] == 1) {
					_pacman.setPosition(_previousPos);
					std::swap(_dir1, _dir2);
					continue;
				}

				x = (int)(_pacman.getPosition().x + (TILESIZE / 2 - 1)) / TILESIZE;
				y = (int)(_pacman.getPosition().y - (TILESIZE / 2 - 1)) / TILESIZE;

				if (map[y][x] == 1) {
					_pacman.setPosition(_previousPos);
					std::swap(_dir1, _dir2);
					continue;
				} 

				x = (int)(_pacman.getPosition().x - (TILESIZE / 2 - 1)) / TILESIZE;
				y = (int)(_pacman.getPosition().y + (TILESIZE / 2 - 1)) / TILESIZE;

				if (map[y][x] == 1) {
					_pacman.setPosition(_previousPos);
					std::swap(_dir1, _dir2);
					continue;
				} 

				x = (int)(_pacman.getPosition().x - (TILESIZE / 2 - 1)) / TILESIZE;
				y = (int)(_pacman.getPosition().y - (TILESIZE / 2 - 1)) / TILESIZE;

				if (map[y][x] == 1) {
					_pacman.setPosition(_previousPos);
					std::swap(_dir1, _dir2);
					continue;
				} 


				if (tries == 1) {
					std::swap(_dir1, _dir2);
				} else {
					_dir2 = _dir1;
				}
				
				break;
			}

			//Check if pacman should eat point
			x = (int)_pacman.getPosition().x / TILESIZE;
			y = (int)_pacman.getPosition().y / TILESIZE;

			if (map[y][x] == 2) {
				map[y][x] = 0;
			}

			if (map[y][x] == 3) {
				map[y][x] = 0;
				_power = true;
				_powerTimer.restart();
			}

			if (_powerTimer.getElapsedTime().asSeconds() > PACMAN_POWER_TIME) {
				_power = false;
			}

			//std::cout << x << "\t" << y << std::endl;

		}

	}

	void Pacman::move(Dir dir) {
		_stopped = false;
		_dir1 = dir;
	}

	void Pacman::draw() {
		_data->window.draw(_pacman);
	}

	bool Pacman::isDead(std::array<Ghost*, 4> ghosts) {
		for (int i = 0; i < ghosts.size(); i++) {
			if (Collision::checkSpriteCollision(_pacman, 0.3f, ghosts[i]->getSprite(), 1.0f) && !ghosts[i]->getEyes()) {
				return true;
			}
		}
		return false;
	}

	void Pacman::reset() {
		_pacman.setPosition(336.0f, 636.0f);
	}

}