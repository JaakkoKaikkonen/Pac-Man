#pragma once

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "DEFINITIONS.hpp"
#include "Animation.hpp"
#include "Ghost.hpp"
#include <array>

namespace engine {

	class Pacman
	{
	public:
		Pacman(gameDataRef data);

		void update(int map[MAP_RES_Y][MAP_RES_X]);

		void move(Dir dir);

		void draw();

		bool isDead(std::array<Ghost*, 4> ghosts);

		void reset();

		bool power() { return _power; }

		sf::Vector2f getPosition() { return _pacman.getPosition(); }

		sf::Sprite& getSprite() { return _pacman; }

		Dir& getDir() { return _dir2; }

	private:
		gameDataRef _data;

		sf::Sprite _pacman;

		sf::Vector2f _previousPos;

		sf::IntRect _pacmanAniamtionFrames[4] = { PACMAN_01 , PACMAN_02 , PACMAN_01, PACMAN_03 };

		Animation _pacmanAnimation;

		sf::IntRect _teleport1, _teleport2;

		sf::Clock _teleportTimer;

		Dir _dir1;

		Dir _dir2;

		bool _power = false;

		sf::Clock _powerTimer;

		bool _stopped = true;

	};

}

