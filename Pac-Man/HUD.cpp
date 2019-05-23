#include "HUD.hpp"
#include "DEFINITIONS.hpp"
#include <string>
#include <iostream>

namespace Game {

	HUD::HUD(gameDataRef data)
		: _data(data), _score("00", _data->assets.getFont("Font"), 24), _highScoreText("HIGH SCORE", _data->assets.getFont("Font"), 24), 
		  _highScore(" ", _data->assets.getFont("Font"), 24), _gameOverText("GAME OVER!", _data->assets.getFont("Font"), 24)
	{
		_score.setOrigin(_score.getGlobalBounds().width, 0.0f);
		_score.setPosition(168.0f, 28.0f);

		//Read highscore
		std::ifstream highScoreFile;
		highScoreFile.open(HIGH_SCORE_FILEPATH);

		if (highScoreFile.is_open()) {
			int highScore;
			highScoreFile >> highScore;
			_highScore.setString(std::to_string(highScore));
		} else {
			std::cout << "Highscore file not found" << std::endl;
		}


		_highScoreText.setPosition(SCREEN_WIDTH / 2 - _highScoreText.getGlobalBounds().width / 2, 2.0f);

		_highScore.setOrigin(_highScore.getGlobalBounds().width, 0.0f);
		_highScore.setPosition(SCREEN_WIDTH / 2, 36.0f);

		_gameOverText.setOrigin(_gameOverText.getGlobalBounds().width / 2, 0.0f);
		_gameOverText.setPosition(SCREEN_WIDTH / 2, 20*TILESIZE);

	}

	void HUD::drawScore(const int& pointsRemainig, const int& bigPointsRemainig) {
		_score.setString(std::to_string(POINT_COUNT - pointsRemainig));
		_score.setString(std::to_string(std::stoi((std::string)_score.getString()) + _scoreFromGhosts / 10 + (BIGPOINT_COUNT - bigPointsRemainig) * 5) + "0");

		_score.setOrigin(_score.getGlobalBounds().width, 0.0f);

		_data->window.draw(_score);
	}


	void HUD::drawHighScore() {

		_data->window.draw(_highScoreText);

		if (std::stoi((std::string)_score.getString()) > std::stoi((std::string)_highScore.getString())) {
			_highScore.setString(_score.getString());
		}

		_highScore.setOrigin(_highScore.getGlobalBounds().width / 2, 0.0f);
		_data->window.draw(_highScore);

	}

	void HUD::saveHighScore() {

		//Save highscore
		std::ofstream highScoreFile(HIGH_SCORE_FILEPATH);

		if (highScoreFile.is_open()) {
			highScoreFile << std::stoi((std::string)_highScore.getString());
		} else {
			std::cout << "Highscore file not found" << std::endl;
		}

	}

	void HUD::drawGameOverText() {
		_data->window.draw(_gameOverText);
	}
	 
	void HUD::addToScore(const int score) {
		_scoreFromGhosts += score;
	}

	

}
