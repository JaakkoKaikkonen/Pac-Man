#pragma once

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "Ghost.hpp"
#include "DEFINITIONS.hpp"

namespace engine {

	class Clyde : public Ghost
	{
	public:
		Clyde(gameDataRef data);

		void setTarget(const sf::Vector2f& pacmanPosition, const Dir& pacmandir, const sf::Vector2f& blinkyPosition, const int map[MAP_RES_Y][MAP_RES_X]) override;

		void boxAnimation() override;

		bool isFree() override;

	private:
		int _boxCounter = 0;
		bool _go = false;

		sf::Clock _startTimer;

		Dir _spot;

	};

}



