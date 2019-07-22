#pragma once

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "Ghost.hpp"
#include "DEFINITIONS.hpp"

namespace Game {

	class Inky : public Ghost
	{
	public:
		Inky(gameDataRef data);

		void chase(const sf::Vector2f& pacmanPosition, const Dir& pacmandir, const sf::Vector2f& blinkyPosition, const int map[MAP_RES_Y][MAP_RES_X]) override;

		void scatter() override;


		void boxAnimation() override;

		bool isFree() override;

	private:
	
		sf::Clock startTimer;

	};

}