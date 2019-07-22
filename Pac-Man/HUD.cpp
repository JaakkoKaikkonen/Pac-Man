#include "HUD.hpp"
#include "DEFINITIONS.hpp"
#include <string>
#include <iostream>

namespace Game {

	HUD::HUD(gameDataRef data)
		: data(data),
		  score("00", data->assets.getFont("Font"), 24),
		  highScoreText("HIGH SCORE", data->assets.getFont("Font"), 24), 
		  highScore(" ", data->assets.getFont("Font"), 24),
		  gameOverText("GAME OVER!", data->assets.getFont("Font"), 24)
	{
		score.setOrigin(score.getGlobalBounds().width, 0.0f);
		score.setPosition(168.0f, 28.0f);

		//Read highscore
		std::ifstream highScoreFile;
		highScoreFile.open(HIGH_SCORE_FILEPATH);

		if (highScoreFile.is_open()) {
			int highScoreInt;
			highScoreFile >> highScoreInt;
			highScore.setString(std::to_string(highScoreInt));
		} else {
			std::cout << "Highscore file not found" << std::endl;
		}


		highScoreText.setPosition(SCREEN_WIDTH / 2 - highScoreText.getGlobalBounds().width / 2, 2.0f);

		highScore.setOrigin(highScore.getGlobalBounds().width, 0.0f);
		highScore.setPosition(SCREEN_WIDTH / 2, 36.0f);

		gameOverText.setOrigin(gameOverText.getGlobalBounds().width / 2, 0.0f);
		gameOverText.setPosition(SCREEN_WIDTH / 2, 20*TILESIZE);

	}

	void HUD::drawScore(const int& pointsRemainig, const int& bigPointsRemainig) {
		score.setString(std::to_string(POINT_COUNT - pointsRemainig));
		score.setString(std::to_string(std::stoi((std::string)score.getString()) + scoreFromGhosts / 10 + (BIGPOINT_COUNT - bigPointsRemainig) * 5) + "0");

		score.setOrigin(score.getGlobalBounds().width, 0.0f);

		data->window.draw(score);
	}


	void HUD::drawHighScore() {

		data->window.draw(highScoreText);

		if (std::stoi((std::string)score.getString()) > std::stoi((std::string)highScore.getString())) {
			highScore.setString(score.getString());
		}

		highScore.setOrigin(highScore.getGlobalBounds().width / 2, 0.0f);
		data->window.draw(highScore);

	}

	void HUD::saveHighScore() {

		//Save highscore
		std::ofstream highScoreFile(HIGH_SCORE_FILEPATH);

		if (highScoreFile.is_open()) {
			highScoreFile << std::stoi((std::string)highScore.getString());
		} else {
			std::cout << "Error with highscore file" << std::endl;
		}

	}

	void HUD::drawGameOverText() {
		data->window.draw(gameOverText);
	}
	 
	void HUD::addToScore(const int score) {
		scoreFromGhosts += score;
	}

	

}