#include "Pacman.hpp"
#include "Collision.hpp"
#include <iostream>

namespace Game {

	Pacman::Pacman(gameDataRef data)
		: data(data),
		  pacman(data->assets.getTexture("Pac-man"), PACMAN_03),
		  pacmanAnimation(pacman, pacmanAniamtionFrames, 4, PACMAN_ANIMATION_TIME),
		  teleport1(-TILESIZE*4, TILESIZE*16 + TILESIZE/2, TILESIZE*4 + 5, TILESIZE*2),
		  teleport2(SCREEN_WIDTH - 5, TILESIZE*16 + TILESIZE/2, TILESIZE*4, TILESIZE*2)
	{
		pacman.setOrigin(pacman.getGlobalBounds().width / 2, pacman.getGlobalBounds().height / 2);
		pacman.setScale(1.25f, 1.25f);
		pacman.setPosition(PACMAN_START_POS);
	}


	void Pacman::update(int map[MAP_RES_Y][MAP_RES_X]) {

		if (!stopped) {

			//check if pacman should animate
			if (previousPos == pacman.getPosition()) {
				pacman.setTextureRect(PACMAN_01);
			} else {
				pacmanAnimation.animate();
			}

			//Teleports
			if (teleport1.contains((int)(pacman.getPosition().x - pacman.getGlobalBounds().width / 2), (int)pacman.getPosition().y)) {
				if (teleportTimer.getElapsedTime().asSeconds() < 0.4f) {
					pacman.move(PACMAN_SPEED, 0.0f);
					return;
				}
				pacman.move(-PACMAN_SPEED, 0.0f);
				if (pacman.getPosition().x + pacman.getGlobalBounds().width < 0) {
					pacman.setPosition(teleport2.left + pacman.getGlobalBounds().width, teleport2.top + pacman.getGlobalBounds().height / 2 + 4);
					teleportTimer.restart();
				}
				return;
			}

			if (teleport2.contains((int)(pacman.getPosition().x + pacman.getGlobalBounds().width / 2), (int)pacman.getPosition().y)) {
				if (teleportTimer.getElapsedTime().asSeconds() < 0.4f) {
					pacman.move(-PACMAN_SPEED, 0.0f);
					return;
				}
				pacman.move(PACMAN_SPEED, 0.0f);
				if (pacman.getPosition().x - pacman.getGlobalBounds().width > SCREEN_WIDTH) {
					pacman.setPosition(teleport1.left + pacman.getGlobalBounds().width + 20, teleport1.top + pacman.getGlobalBounds().height / 2 + 4);
					teleportTimer.restart();
				}
				return;
			}


			//Save previous position
			previousPos = pacman.getPosition();


			int x, y;

			//Movement
			for (int tries = 0; tries < 2; tries++) {

				switch (dir1) {

				case Dir::Right:
					pacman.move(PACMAN_SPEED, 0.0f);
					pacman.setRotation(0.0f);
					break;
				case Dir::Left:
					pacman.move(-PACMAN_SPEED, 0.0f);
					pacman.setRotation(180.0f);
					break;
				case Dir::Down:
					pacman.move(0.0f, PACMAN_SPEED);
					pacman.setRotation(90.0f);
					break;
				case Dir::Up:
					pacman.move(0.0f, -PACMAN_SPEED);
					pacman.setRotation(-90.0f);
				}


				//Check Collision

				x = (int)(pacman.getPosition().x + (TILESIZE / 2 - 1)) / TILESIZE;
				y = (int)(pacman.getPosition().y + (TILESIZE / 2 - 1)) / TILESIZE;

				if (map[y][x] == 1) {
					pacman.setPosition(previousPos);
					std::swap(dir1, dir2);
					continue;
				}

				x = (int)(pacman.getPosition().x + (TILESIZE / 2 - 1)) / TILESIZE;
				y = (int)(pacman.getPosition().y - (TILESIZE / 2 - 1)) / TILESIZE;

				if (map[y][x] == 1) {
					pacman.setPosition(previousPos);
					std::swap(dir1, dir2);
					continue;
				} 

				x = (int)(pacman.getPosition().x - (TILESIZE / 2 - 1)) / TILESIZE;
				y = (int)(pacman.getPosition().y + (TILESIZE / 2 - 1)) / TILESIZE;

				if (map[y][x] == 1) {
					pacman.setPosition(previousPos);
					std::swap(dir1, dir2);
					continue;
				} 

				x = (int)(pacman.getPosition().x - (TILESIZE / 2 - 1)) / TILESIZE;
				y = (int)(pacman.getPosition().y - (TILESIZE / 2 - 1)) / TILESIZE;

				if (map[y][x] == 1) {
					pacman.setPosition(previousPos);
					std::swap(dir1, dir2);
					continue;
				} 


				if (tries == 1) {
					std::swap(dir1, dir2);
				} else {
					dir2 = dir1;
				}
				
				break;
			}

			//Check if pacman should eat point
			x = (int)pacman.getPosition().x / TILESIZE;
			y = (int)pacman.getPosition().y / TILESIZE;

			if (map[y][x] == 2) {
				map[y][x] = 0;
			} else if (map[y][x] == 3) {
				map[y][x] = 0;
				powerOn = true;
				powerTimer.restart();
			}

			if (powerTimer.getElapsedTime().asSeconds() > PACMAN_POWER_TIME) {
				powerOn = false;
			}

		}

	}

	void Pacman::move(Dir dir) {
		stopped = false;
		dir1 = dir;
	}

	void Pacman::draw() {
		data->window.draw(pacman);
	}

	bool Pacman::isDead(std::array<Ghost*, 4> ghosts) {
		for (int i = 0; i < ghosts.size(); i++) {
			if (Collision::checkSpriteCollision(pacman, 0.3f, ghosts[i]->getSprite(), 1.0f) && !ghosts[i]->getEyes()) {
				return true;
			}
		}
		return false;
	}

	/*void Pacman::reset() {
		pacman.setPosition(PACMAN_START_POS);
	}*/

}