#include "Clyde.hpp"
#include "Utility.hpp"

namespace Game {

	Clyde::Clyde(gameDataRef data) : Ghost(data)
	{
		ghost.setTexture(data->assets.getTexture("Ghost"));
		ghost.setTextureRect(CLYDE_RIGHT_01);
		ghost.setOrigin(ghost.getGlobalBounds().width / 2, ghost.getGlobalBounds().height / 2);
		ghost.setScale(1.25f, 1.25f);
		ghost.setPosition(TILESIZE * 16, TILESIZE * 17 + TILESIZE / 2);

		ghostColor = sf::Color::Yellow;
		homeCorner = CLYDES_CORNER;

		animationFramesRight[0] = CLYDE_RIGHT_01;
		animationFramesRight[1] = CLYDE_RIGHT_02;

		animationFramesLeft[0] = CLYDE_LEFT_01;
		animationFramesLeft[1] = CLYDE_LEFT_02;

		animationFramesUp[0] = CLYDE_UP_01;
		animationFramesUp[1] = CLYDE_UP_02;

		animationFramesDown[0] = CLYDE_DOWN_01;
		animationFramesDown[1] = CLYDE_DOWN_02;
	}


	void Clyde::chase(const sf::Vector2f& pacmanPosition, const Dir& pacmandir, const sf::Vector2f& blinkyPosition, const int map[MAP_RES_Y][MAP_RES_X]) {
		
		sf::Vector2i distance((int)abs(ghost.getPosition().x / TILESIZE - pacmanPosition.x / TILESIZE), (int)abs(ghost.getPosition().y / TILESIZE - pacmanPosition.y / TILESIZE));

		if ((distance.x + distance.y) > 8) {
			target = sf::Vector2i((int)(pacmanPosition.x / TILESIZE), (int)(pacmanPosition.y / TILESIZE));
		} else {
			target = homeCorner;
		}

		//Clamp target to be on the grid
		target = utility::clamp(target, 1, MAP_RES_X - 2, 4, MAP_RES_Y - 4);

		//If target == its own position -> set new target
		if (target == (sf::Vector2i)ghost.getPosition() / TILESIZE) {
			target = sf::Vector2i(homeCorner.x + 2, homeCorner.y);
		}

	}

	void Clyde::scatter() {
		if (((sf::Vector2i)ghost.getPosition() / TILESIZE) == homeCorner) {
			target = sf::Vector2i(homeCorner.x + 2, homeCorner.y);
		} else {
			target = homeCorner;
		}
	}


	void Clyde::boxAnimation() {

		if (startTimer.getElapsedTime().asSeconds() > 1.75f) {

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
					dirVector = sf::Vector2f(2.0f, 0.0f);
				} else if (boxCounter == 66) {
					dirVector = sf::Vector2f(0.0f, 0.0f);
					target = homeCorner;
					eyes = false;
					go = false;
					boxCounter = 0;
				}
			} else if (!go) {
				dirVector = sf::Vector2f(0.0f, 0.0f);
			} else if (boxCounter < 24) {
				dirVector = sf::Vector2f(-2.0f, 0.0f);
			} else if (boxCounter < 60) {
				dirVector = sf::Vector2f(0.0f, -2.0f);
			} else if (boxCounter < 66) {
				dirVector = sf::Vector2f(-2.0f, 0.0f);
			}

			boxCounter++;
		}
	}


	bool Clyde::isFree() {
		return boxCounter > 66;
	}


}