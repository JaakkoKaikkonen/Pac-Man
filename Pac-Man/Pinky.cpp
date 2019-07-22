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
		homeCorner = PINKYS_CORNER;

		animationFramesRight[0] = PINKY_RIGHT_01;
		animationFramesRight[1] = PINKY_RIGHT_02;

		animationFramesLeft[0] = PINKY_LEFT_01;
		animationFramesLeft[1] = PINKY_LEFT_02;

		animationFramesUp[0] = PINKY_UP_01;
		animationFramesUp[1] = PINKY_UP_02;

		animationFramesDown[0] = PINKY_DOWN_01;
		animationFramesDown[1] = PINKY_DOWN_02;
	}


	void Pinky::chase(const sf::Vector2f& pacmanPosition, const Dir& pacmandir, const sf::Vector2f& blinkyPosition, const int map[MAP_RES_Y][MAP_RES_X]) {
		
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
			target = homeCorner;
			if (homeCorner == (sf::Vector2i)ghost.getPosition() / TILESIZE) {
				target = sf::Vector2i(homeCorner.x + 2, homeCorner.y);
			}
		}

	}

	void Pinky::scatter() {
		if (((sf::Vector2i)ghost.getPosition() / TILESIZE) == homeCorner) {
			target = sf::Vector2i(homeCorner.x + 2, homeCorner.y);
		} else {
			target = homeCorner;
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
				target = homeCorner;
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