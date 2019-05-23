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
		gameDataRef _data;

		sf::Text _score;

		sf::Text _highScoreText;

		sf::Text _highScore;

		sf::Text _gameOverText;

		int _scoreFromGhosts = 0;

	};

}

