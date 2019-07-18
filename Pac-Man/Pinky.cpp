#include "Pinky.hpp"
#include "Utility.hpp"
#include <iostream>

namespace Game {

	Pinky::Pinky(gameDataRef data) : Ghost(data)
	{
		ghost.setTexture(data->assets.getTexture("Ghost"));
		ghost.setTextureRect(PINKY_RIGHT_01);
		ghost.setOrigin(ghost.getGlobalBounds().width / 2, ghost.getGlobalBounds().height / 2);
		ghost.setScale(1.25f, 1.25f);
		ghost.setPosition(TILESIZE * 14, TILESIZE * 17 + TILESIZE / 2);

		ghostColor = sf::Color::Magenta;

		animationFramesRight[0] = PINKY_RIGHT_01;
		animationFramesRight[1] = PINKY_RIGHT_02;

		animationFramesLeft[0] = PINKY_LEFT_01;
		animationFramesLeft[1] = PINKY_LEFT_02;

		animationFramesUp[0] = PINKY_UP_01;
		animationFramesUp[1] = PINKY_UP_02;

		animationFramesDown[0] = PINKY_DOWN_01;
		animationFramesDown[1] = PINKY_DOWN_02;
	}


	void Pinky::setTarget(const sf::Vector2f& pacmanPosition, const Dir& pacmandir, const sf::Vector2f& blinkyPosition, const int map[MAP_RES_Y][MAP_RES_X]) {
		
		if (!eyes) {

			if (Mode::Chase == mode) {

				if (Dir::Right == pacmandir) {
					target = sf::Vector2i((int)((pacmanPosition.x + 4 * TILESIZE) / TILESIZE), (int)(pacmanPosition.y / TILESIZE));
				} else if (Dir::Left == pacmandir) {
					target = sf::Vector2i((int)((pacmanPosition.x - 4 * TILESIZE) / TILESIZE), (int)(pacmanPosition.y / TILESIZE));
				} else if (Dir::Up == pacmandir) {
					target = sf::Vector2i((int)(pacmanPosition.x / TILESIZE), (int)((pacmanPosition.y - 4 * TILESIZE) / TILESIZE));
				} else if (Dir::Down == pacmandir) {
					target = sf::Vector2i((int)(pacmanPosition.x / TILESIZE), (int)((pacmanPosition.y + 4 * TILESIZE) / TILESIZE));
				}

				//Clamp target to be on the grid
				target = utility::clamp(target, 1, MAP_RES_X - 2, 4, MAP_RES_Y - 4);

				//If target is wall, fix it
				if (map[target.y][target.x] == 1) {
					this->fixTarget(map);
				}

				//If target == its own position -> set new target
				if (target == (sf::Vector2i)ghost.getPosition() / TILESIZE) {
					target = PINKYS_CORNER;
				}

			} else if (Mode::Scatter == mode) {

				if (((sf::Vector2i)ghost.getPosition() / TILESIZE) == PINKYS_CORNER) {
					target.x = PINKYS_CORNER.x + 2;
					target.y = PINKYS_CORNER.y;
				} else {
					target = PINKYS_CORNER;
				}

			} else if (Mode::Flee == mode) {

				if (turn) {

					if (Dir::Right == dir) {
						target = sf::Vector2i((int)ghost.getPosition().x / TILESIZE - 1, (int)ghost.getPosition().y / TILESIZE);
					} else if (Dir::Left == dir) {
						target = sf::Vector2i((int)ghost.getPosition().x / TILESIZE + 1, (int)ghost.getPosition().y / TILESIZE);
					} else if (Dir::Down == dir) {
						target = sf::Vector2i((int)ghost.getPosition().x / TILESIZE, (int)ghost.getPosition().y / TILESIZE - 1);
					} else if (Dir::Up == dir) {
						target = sf::Vector2i((int)ghost.getPosition().x / TILESIZE, (int)ghost.getPosition().y / TILESIZE + 1);
					}

				} else {

					if (target == ((sf::Vector2i)ghost.getPosition() / TILESIZE)) {

						do {
							target = sf::Vector2i(rand() % 25 + 1, rand() % 28 + 4);
						} while (target == ((sf::Vector2i)ghost.getPosition() / TILESIZE));

						if (map[target.y][target.x] == 1) {
							this->fixTarget(map);
						}

					}

					if (teleporting) {
						target = PINKYS_CORNER;
					}

				}
			}

		} else if ((sf::Vector2i)ghost.getPosition() / TILESIZE == target) {
			spot = Dir::Right;
			go = true;
			boxCounter = 0;

		} else if ((int)ghost.getPosition().x / TILESIZE == target.x - 1 && (int)ghost.getPosition().y / TILESIZE == target.y) {
			spot = Dir::Left;
			go = true;
			boxCounter = 0;
		}
		
	}


	void Pinky::boxAnimation() {
		if (!go && counter == 6) {
			go = true;
			boxCounter = 0;
		}

		if (eyes) {
			if (boxCounter < 6) {
				if (spot == Dir::Right) {
					dirVector = sf::Vector2f(-2.0f, 0.0f);
				} else {
					dirVector = sf::Vector2f(2.0f, 0.0f);
				}
			} else if (boxCounter < 42) {
				dirVector = sf::Vector2f(0.0f, 2.0f);
			} else if (boxCounter == 42) {
				dirVector = sf::Vector2f(0.0f, 0.0f);
				target = PINKYS_CORNER;
				eyes = false;
				go = false;
				boxCounter = 0;
			}
		} else if (!go) {
			dirVector = sf::Vector2f(0.0f, 0.0f);
		} else if (boxCounter < 36) {
			dirVector = sf::Vector2f(0.0f, -2.0f);
		} else {
			dirVector = sf::Vector2f(-2.0f, 0.0f);
		}

		boxCounter++;
	}


	bool Pinky::isFree() {
		return boxCounter > 42;
	}


}
