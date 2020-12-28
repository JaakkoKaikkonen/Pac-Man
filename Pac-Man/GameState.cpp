#include <string>
#include "GameState.hpp"
#include "MenuState.hpp"
#include "DEFINITIONS.hpp"
#include <iostream>
#include "Utility.hpp"
#include "Collision.hpp"


namespace Game {

	GameState::GameState(gameDataRef data)
		: data(data),
		  map(data->assets.getTexture("Map")),
		  pacman(data),
		  hud(data),
		  scoreText("100", data->assets.getFont("Font"), 18)
	{

		//Setup map
		map.setPosition(0.0f, TILESIZE*3);

		//Init points
		for (int i = 0; i < points.size(); i++) {
			points[i].init(data, "Point");
		}

		for (int i = 0; i < bigPoints.size(); i++) {
			bigPoints[i].init(data, "Big_point");
		}

		//Set up score sprite
		scoreText.setOrigin(scoreText.getGlobalBounds().width / 2, scoreText.getGlobalBounds().height / 2);
		scoreText.setFillColor(sf::Color::Cyan);

		//Setup ghosts
		ghosts[0] = new Clyde(data);
		ghosts[1] = new Inky(data);
		ghosts[2] = new Pinky(data);
		ghosts[3] = new Blinky(data);
	}

	GameState::~GameState() {
		for (int i = 0; i < ghosts.size(); i++) {
			delete ghosts[i];
		}
	}


	void GameState::handleInput() {
		sf::Event event;
		while (data->window.pollEvent(event)) {
			if (sf::Event::Closed == event.type) {
				data->window.close();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			data->window.close();
		}

		//////////////////////////////////DEBUG////////////////////////////////////////////
		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
			pacman.reset();
		}*/
		//////////////////////////////////DEBUG////////////////////////////////////////////

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			pacman.move(Dir::Right);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			pacman.move(Dir::Left);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			pacman.move(Dir::Down);
		} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
			pacman.move(Dir::Up);
		}

	}

	void GameState::update() {

		
		if (stopTimer.getElapsedTime().asSeconds() > 0.5f) {

			stop = false;

			if (pacman.isDead(ghosts) || win) {

				if (!highScoreSaved) {
					hud.saveHighScore();
					highScoreSaved = true;
					mapTimer.restart();
					endTimer.restart();
				}

				if (win) {
					pacman.getSprite().setTextureRect(PACMAN_03);
					if (mapTimer.getElapsedTime().asSeconds() < 0.25f) {
						map.setTexture(data->assets.getTexture("Map"));
					} else if (mapTimer.getElapsedTime().asSeconds() < 0.50f) {
						map.setTexture(data->assets.getTexture("Map_white"));
					} else {
						mapTimer.restart();
					}
				} else {
					gameOver = true;
				}

				if (endTimer.getElapsedTime().asSeconds() > 2.0f) {
					data->newState = new MenuState(data);
				}

			} else {

				if (modeTimer.getElapsedTime().asSeconds() > modeTimes[modeIndex]) {
					modeIndex++;
					modeTimer.restart();
				}

				if (pacman.power()) {
					mode = Mode::Flee;
					for (int i = 0; i < ghosts.size(); i++) {
						if (Collision::checkSpriteCollision(pacman.getSprite(), 0.7f, ghosts[i]->getSprite(), 1.0f) && !ghosts[i]->getEyes()) {
							ghosts[i]->reset();
							stop = true;
							scoreText.setString(std::to_string(std::stoi((std::string)scoreText.getString()) * 2));
							scoreText.setOrigin(scoreText.getGlobalBounds().width / 2, scoreText.getGlobalBounds().height / 2);
							hud.addToScore(std::stoi((std::string)scoreText.getString()));
							ghostIndex = i;
							stopTimer.restart();
						}
					}
				} else {
					scoreText.setString("100");
					mode = modes[modeIndex];
				}

				pacman.update(grid);

				Ghost::setMode(mode);

				if (Ghost::findNewPath()) {
					for (int i = 0; i < ghosts.size(); i++) {
						if (ghosts[i]->isFree()) {
							ghosts[i]->setTarget(pacman.getPosition(), pacman.getDir(), ghosts[3]->getPosition(), grid);
							ghosts[i]->findPath(grid);
						}
						ghosts[i]->disableTurn();
					}
				}

				for (int i = 0; i < ghosts.size(); i++) {
					ghosts[i]->update();
				}

			}

		}

	}

	void GameState::draw() {

		data->window.clear();

		data->window.draw(map);

		//////////////////////////////////DEBUG////////////////////////////////////////////
		//Draw grid lines
		/*sf::Vertex line[2];
		float y = 0;
		for (int i = 0; i < MAP_RES_Y; i++) {
			line[0] = sf::Vertex(sf::Vector2f(0.0, y));
			line[1] = sf::Vertex(sf::Vector2f(SCREEN_WIDTH, y));
			y += TILESIZE;
			data->window.draw(line, 2, sf::Lines);
		}
		float x = 0;
		for (int i = 0; i < MAP_RES_X; i++) {
			line[0] = sf::Vertex(sf::Vector2f(x, 0.0));
			line[1] = sf::Vertex(sf::Vector2f(x, SCREEN_HEIGHT));
			x += TILESIZE;
			data->window.draw(line, 2, sf::Lines);
		}*/
		//////////////////////////////////DEBUG////////////////////////////////////////////

		//Draw points
		int pointCount = 0;
		int bigPointCount = 0;

		for (int i = 0; i < MAP_RES_Y; i++) {
			for (int j = 0; j < MAP_RES_X; j++) {
				if (grid[i][j] == 2) {
					points[pointCount].draw(j * TILESIZE + TILESIZE / 2, i * TILESIZE + TILESIZE / 2);
					pointCount++;
				} else if (grid[i][j] == 3) {
					if (bigPointFalshTimer.getElapsedTime().asSeconds() < 0.25f) {
						bigPoints[bigPointCount].draw(j * TILESIZE + TILESIZE / 2, i * TILESIZE + TILESIZE / 2);
					}
					bigPointCount++;
				}
			}
		}

		//Big point flash timer
		if (bigPointFalshTimer.getElapsedTime().asSeconds() > 0.5f) {
			bigPointFalshTimer.restart();
		}

		if ((pointCount + bigPointCount) == 0) {
			win = true;
		}

		if (!stop) {
			pacman.draw();
		}

		if (!win) {
			for (int i = 0; i < ghosts.size(); i++) {
				if (stop && ghostIndex == i) {
					scoreText.setPosition(ghosts[i]->getPosition());
					data->window.draw(scoreText);
				} else {
					ghosts[i]->draw();
				}
			}
		}

		if (gameOver) {
			hud.drawGameOverText();
		}

		hud.drawScore(pointCount, bigPointCount);

		hud.drawHighScore();


		data->window.display();

	}


}