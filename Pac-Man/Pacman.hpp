#pragma once

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "DEFINITIONS.hpp"
#include "Animation.hpp"
#include "Ghost.hpp"
#include <array>

namespace Game {

	class Pacman
	{
	public:
		Pacman(gameDataRef data);

		void update(int map[MAP_RES_Y][MAP_RES_X]);

		void move(Dir dir);

		void draw();

		bool isDead(std::array<Ghost*, 4> ghosts);

		/*void reset();*/

		bool power() { return powerOn; }

		sf::Vector2f getPosition() { return pacman.getPosition(); }

		sf::Sprite& getSprite() { return pacman; }

		Dir& getDir() { return dir2; }

	private:
		gameDataRef data;

		sf::Sprite pacman;

		sf::Vector2f previousPos;

		sf::IntRect pacmanAniamtionFrames[4] = { PACMAN_01 , PACMAN_02 , PACMAN_01, PACMAN_03 };

		Animation pacmanAnimation;

		sf::IntRect teleport1, teleport2;

		sf::Clock teleportTimer;

		Dir dir1;

		Dir dir2;

		bool powerOn = false;

		sf::Clock powerTimer;

		bool stopped = true;

	};

}