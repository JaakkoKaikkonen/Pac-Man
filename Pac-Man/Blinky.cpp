#include "Blinky.hpp"
#include "Utility.hpp"
#include <iostream>

namespace Game {

	Blinky::Blinky(gameDataRef data) : Ghost(data)
	{
		ghost.setTexture(data->assets.getTexture("Ghost"));
		ghost.setTextureRect(BLINKY_RIGHT_01);
		ghost.setOrigin(ghost.getGlobalBounds().width / 2, ghost.getGlobalBounds().height / 2);
		ghost.setScale(1.25f, 1.25f);
		ghost.setPosition(TILESIZE * 14, TILESIZE * 14 + TILESIZE / 2);

		ghostColor = sf::Color::Red;

		homeCorner = sf::Vector2i(26, 4);

		animationFramesRight[0] = BLINKY_RIGHT_01;
		animationFramesRight[1] = BLINKY_RIGHT_02;

		animationFramesLeft[0] = BLINKY_LEFT_01;
		animationFramesLeft[1] = BLINKY_LEFT_02;

		animationFramesUp[0] = BLINKY_UP_01;
		animationFramesUp[1] = BLINKY_UP_02;

		animationFramesDown[0] = BLINKY_DOWN_01;
		animationFramesDown[1] = BLINKY_DOWN_02;
	}


	void Blinky::chase(const sf::Vector2f& pacmanPosition, const Dir& pacmandir, const sf::Vector2f& blinkyPosition, const int map[MAP_RES_Y][MAP_RES_X]) {
		
		target = sf::Vector2i((int)(pacmanPosition.x / TILESIZE), (int)(pacmanPosition.y / TILESIZE));

		//Clamp target to be on the grid
		target = utility::clamp(target, 1, MAP_RES_X - 2, 4, MAP_RES_Y - 4);

	}

	void Blinky::scatter() {
		if (((sf::Vector2i)ghost.getPosition() / TILESIZE) == homeCorner) {
			target = sf::Vector2i(homeCorner.x - 2, homeCorner.y);
		} else {
			target = homeCorner;
		}
	}

	
	void Blinky::boxAnimation() {
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
		} else if (start) {
			if (!go) {
				dirVector = sf::Vector2f(0.0f, 0.0f);
			} else if (boxCounter < 6) {
				dirVector = sf::Vector2f(2.0f, 0.0f);
			}
		} else {
			if (!go) {
				dirVector = sf::Vector2f(0.0f, 0.0f);
			} else if (boxCounter < 36) {
				dirVector = sf::Vector2f(0.0f, -2.0f);
			} else {
				dirVector = sf::Vector2f(2.0f, 0.0f);
			}
		}

		boxCounter++;
	}


	bool Blinky::isFree() {
		if (start) {
			return boxCounter > 6;
		} else {
			return boxCounter > 42;
		}
	}


}