#include "Inky.hpp"
#include "Utility.hpp"
#include <iostream>

namespace Game {

	Inky::Inky(gameDataRef data) : Ghost(data)
	{
		ghost.setTexture(data->assets.getTexture("Ghost"));
		ghost.setTextureRect(INKY_RIGHT_01);
		ghost.setOrigin(ghost.getGlobalBounds().width / 2, ghost.getGlobalBounds().height / 2);
		ghost.setScale(1.25f, 1.25f);
		ghost.setPosition(TILESIZE * 12, TILESIZE * 17 + TILESIZE / 2);

		ghostColor = sf::Color::Cyan;

		animationFramesRight[0] = INKY_RIGHT_01;
		animationFramesRight[1] = INKY_RIGHT_02;

		animationFramesLeft[0] = INKY_LEFT_01;
		animationFramesLeft[1] = INKY_LEFT_02;

		animationFramesUp[0] = INKY_UP_01;
		animationFramesUp[1] = INKY_UP_02;

		animationFramesDown[0] = INKY_DOWN_01;
		animationFramesDown[1] = INKY_DOWN_02;
	}


	void Inky::setTarget(const sf::Vector2f& pacmanPosition, const Dir& pacmandir, const sf::Vector2f& blinkyPosition, const int map[MAP_RES_Y][MAP_RES_X]) {

		if (!eyes) {

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
				target = point + vector;

				//Clamp target to be on the grid
				target = utility::clamp(target, 1, MAP_RES_X - 2, 4, MAP_RES_Y - 4);

				//If target is wall, fix it
				if (map[target.y][target.x] == 1) {
					this->fixTarget(map);
				}

				//If target == its own position -> set new target
				if (target == (sf::Vector2i)ghost.getPosition() / TILESIZE) {
					target = INKYS_CORNER;
					if (INKYS_CORNER == (sf::Vector2i)ghost.getPosition() / TILESIZE) {
						target = sf::Vector2i(INKYS_CORNER.x - 2, INKYS_CORNER.y);
					}
				}

			} else if (Mode::Scatter == mode) {

				if (((sf::Vector2i)ghost.getPosition() / TILESIZE) == INKYS_CORNER) {
					target = sf::Vector2i(INKYS_CORNER.x - 2, INKYS_CORNER.y);
				} else {
					target = INKYS_CORNER;
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
							if (map[target.y][target.x] == 1) {
								this->fixTarget(map);
							}
						} while (target == ((sf::Vector2i)ghost.getPosition() / TILESIZE));

					}

					if (teleporting) {
						target = INKYS_CORNER;
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


	void Inky::boxAnimation() {
	
		if (startTimer.getElapsedTime().asSeconds() > 0.9f) {

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
				} else if (boxCounter < 66) {
					dirVector = sf::Vector2f(-2.0f, 0.0f);
				} else if (boxCounter == 66) {
					dirVector = sf::Vector2f(0.0f, 0.0f);
					target = INKYS_CORNER;
					eyes = false;
					go = false;
					boxCounter = 0;
				}
			} else if (!go) {
				dirVector = sf::Vector2f(0.0f, 0.0f);
			} else if (boxCounter < 24) {
				dirVector = sf::Vector2f(2.0f, 0.0f);
			} else if (boxCounter < 60) {
				dirVector = sf::Vector2f(0.0f, -2.0f);
			} else if (boxCounter < 66) {
				dirVector = sf::Vector2f(2.0f, 0.0f);
			}

			boxCounter++;
		}
	}


	bool Inky::isFree() {
		return boxCounter > 66;
	}
		

}