#pragma once

#include <SFML\Graphics.hpp>
#include "Game.hpp"
#include <fstream>

namespace Game {

	class HUD
	{
	public:
		HUD(gameDataRef data);

		void drawScore(const int& pointsRemainig, const int& bigPointsRemainig);

		void drawHighScore();

		void saveHighScore();

		void drawGameOverText();

		void addToScore(const int score);


	private:
		gameDataRef data;

		sf::Text score;

		sf::Text highScoreText;

		sf::Text highScore;

		sf::Text gameOverText;

		int scoreFromGhosts = 0;

	};

}