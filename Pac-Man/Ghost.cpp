#include "Ghost.hpp"
#include <iostream>
#include <vector>
#include "Utility.hpp"


namespace Game {

	Ghost::Ghost(gameDataRef data)
		: data(data),
		  teleport1(-TILESIZE*2 + TILESIZE/4, TILESIZE*17, TILESIZE*8, TILESIZE),
		  teleport2(TILESIZE*22 - TILESIZE/4, TILESIZE*17, TILESIZE*8, TILESIZE),
		  animationRight(ghost, animationFramesRight, 2, GHOST_ANIMATION_TIME),
		  animationLeft(ghost, animationFramesLeft, 2, GHOST_ANIMATION_TIME), 
		  animationUp(ghost, animationFramesUp, 2, GHOST_ANIMATION_TIME),
		  animationDown(ghost, animationFramesDown, 2, GHOST_ANIMATION_TIME), 
		  fleeAnimation01(ghost, animationFramesFlee01, 2, GHOST_ANIMATION_TIME),
		  fleeAnimation02(ghost, animationFramesFlee02, 4, GHOST_ANIMATION_TIME)
	{
		closedSet.reserve(100);
		openSet.reserve(100);
		finalPath.reserve(50);


		animationFramesFlee01[0] = GHOST_FLEE_BLUE_01;
		animationFramesFlee01[1] = GHOST_FLEE_BLUE_02;

		animationFramesFlee02[0] = GHOST_FLEE_BLUE_01;
		animationFramesFlee02[1] = GHOST_FLEE_BLUE_02;
		animationFramesFlee02[2] = GHOST_FLEE_WHITE_01;
		animationFramesFlee02[3] = GHOST_FLEE_WHITE_02;

		counter = 0;
	}


	//Init static variables
	int Ghost::counter = 0;

	bool Ghost::turn = false;

	int Ghost::turnCounter = 0;

	Mode Ghost::mode = Mode::Scatter;
	
	sf::Clock Ghost::fleeModeTimer = sf::Clock();



	int Ghost::hScore(Node* a, Node* b) {
		return abs(a->x - b->x) + abs(a->y - b->y);
	}


	void Ghost::findPath(const int map[MAP_RES_Y][MAP_RES_X]) {

		if (!teleporting) {
			//A* algorithm / Find shortest path
			//Initialize Nodes
			for (int i = 0; i < MAP_RES_X; i++) {
				for (int j = 0; j < MAP_RES_Y; j++) {
					grid[i][j] = Node { i, j };
				}
			}

			//Clear openSet && closetSet && finalPath
			openSet.clear();
			closedSet.clear();
			finalPath.clear();

			//Set start
			Node* start = &grid[(int)(ghost.getPosition().x / TILESIZE)][(int)(ghost.getPosition().y / TILESIZE)];

			//std::cout << _target.x << ", " << _target.y << std::endl;

			//Set goal
			Node* goal = &grid[target.x][target.y];

			//Set startNodes fScore
			start->f = hScore(start, goal);

			//Push start to openSet
			openSet.push_back(start);


			while (!openSet.empty()) {

				//Find the node with lowest fScore
				int winner = 0;
				for (int i = 0; i < openSet.size(); i++) {
					if (openSet[i]->f < openSet[winner]->f) {
						winner = i;
					}
				}
				//Set current equal to node in open set with lowest fScore
				Node* current = openSet[winner];

				//Clamp current->x so it wont crash when pacman teleports
				current->x = utility::clamp(current->x, 1, MAP_RES_X - 2);

				//Check if path was found
				if (current == goal) {
					finalPath.push_back(current);

					Node* temp = current;
					while (temp->cameFrom) {
						finalPath.push_back(temp->cameFrom);
						temp = temp->cameFrom;
					}
					break;
				}

				//Remove current from openSet
				openSet[winner] = openSet[openSet.size() - 1];
				openSet.pop_back();

				//Add current to closedSet
				closedSet.push_back(current);

				//Find current nodes neighbors
				std::vector<Node*> neighbors;
				neighbors.reserve(4);
				neighbors.push_back(&grid[current->x + 1][current->y]); //Right
				neighbors.push_back(&grid[current->x - 1][current->y]); //Left
				neighbors.push_back(&grid[current->x][current->y + 1]); //Down
				neighbors.push_back(&grid[current->x][current->y - 1]); //Up

				//If current == start delete neighbor behind ghost
				if (current->x == start->x && current->y == start->y && !turn && !eyes) {
					if (Dir::Right == dir) {
						neighbors[(int)Dir::Left] = neighbors[neighbors.size() - 1];
						neighbors.pop_back();
					} else if (Dir::Left == dir) {
						neighbors[(int)Dir::Right] = neighbors[neighbors.size() - 1];
						neighbors.pop_back();
					} else if (Dir::Down == dir) {
						neighbors[(int)Dir::Up] = neighbors[neighbors.size() - 1];
						neighbors.pop_back();
					} else if (Dir::Up == dir) {
						neighbors[(int)Dir::Down] = neighbors[neighbors.size() - 1];
						neighbors.pop_back();
					}
				}

				//Check if neighbor is a wall
				for (int i = neighbors.size() - 1; i >= 0; i--) {
					if (map[neighbors[i]->y][neighbors[i]->x] == 1) {
						neighbors[i] = neighbors[neighbors.size() - 1];
						neighbors.pop_back();
					}
				}

				//Check if neighbor is in closedSet/already evaluated
				bool ignore = false;
				for (int i = 0; i < neighbors.size(); i++) {
					for (int j = 0; j < closedSet.size(); j++) {
						if (neighbors[i] == closedSet[j]) {
							ignore = true;
							break;
						}
					}
					if (ignore) {
						ignore = false;
						continue;
					}

					//Calculate tentative gScore
					int tentative_gScore = current->g + 1;

					//Check if neighbor is in openSet
					bool neighborIsInOpenSet = false;
					for (int j = 0; j < openSet.size(); j++) {
						if (neighbors[i] == openSet[j]) {
							neighborIsInOpenSet = true;
						}
					}

					//If neighbor is not in openSet add it  //If gScore is worse than the old one -> go to next 
					if (!neighborIsInOpenSet) {
						openSet.push_back(neighbors[i]);
					} else if (tentative_gScore >= neighbors[i]->g) {
						continue;
					}

					//This is best path so far record it
					neighbors[i]->cameFrom = current;
					neighbors[i]->g = tentative_gScore;
					neighbors[i]->f = tentative_gScore + hScore(neighbors[i], goal);

				}

			}


			//calculate movement
			if (finalPath.size() > 1) {
				dirVector.x = (float)finalPath[finalPath.size() - 2]->x * TILESIZE - start->x * TILESIZE;
				dirVector.y = (float)finalPath[finalPath.size() - 2]->y * TILESIZE - start->y * TILESIZE;

				dirVector /= (float)TILESIZE;
				dirVector *= GHOST_SPEED;
			} else {
				dirVector.x = 0;
				dirVector.y = 0;
			}

		}
		
	}

	void Ghost::update() {

		if (!this->isFree()) {

			this->boxAnimation();

		} 

		//Set direction
		if (dirVector.x > 0) {
			dir = Dir::Right;
		} else if (dirVector.x < 0) {
			dir = Dir::Left;
		} else if (dirVector.y > 0) {
			dir = Dir::Down;
		} else if (dirVector.y < 0) {
			dir = Dir::Up;
		}

		//Animation
		this->animate();

		if (!this->teleport()) {

			ghost.move(dirVector);

		}
		
	}

	void Ghost::animate() {
	
		if (Mode::Flee == mode && !eyes) {
			if (fleeModeTimer.getElapsedTime().asSeconds() < PACMAN_POWER_TIME - 1.5f) {
				fleeAnimation01.animate();
				fleeAnimation02.reset();
			} else {
				fleeAnimation02.animate();
			}
		} else if (dir == Dir::Right) {
			if (eyes) {
				ghost.setTextureRect(GHOST_EYES_RIGHT);
			} else {
				animationRight.animate();
			}
		} else if (dir == Dir::Left) {
			if (eyes) {
				ghost.setTextureRect(GHOST_EYES_LEFT);
			} else {
				animationLeft.animate();
			}
		} else if (dir == Dir::Down) {
			if (eyes) {
				ghost.setTextureRect(GHOST_EYES_DOWN);
			} else {
				animationDown.animate();
			}
		} else if (dir == Dir::Up) {
			if (eyes) {
				ghost.setTextureRect(GHOST_EYES_UP);
			} else {
				animationUp.animate();
			}
		}

	}

	bool Ghost::teleport() {

		teleporting = false;

		if (teleport1.contains((sf::Vector2i)ghost.getPosition())) {
			teleporting = true;
			if (teleportTimer.getElapsedTime().asSeconds() < 3.0f) {
				ghost.move(GHOST_SPEED, 0.0f);
				return true;
			}
			ghost.move(-GHOST_SPEED, 0.0f);
			if (ghost.getPosition().x + ghost.getGlobalBounds().width < 0) {
				ghost.setPosition(float(teleport2.left + TILESIZE * 7 + TILESIZE / 2), float(teleport2.top + TILESIZE / 2));
				teleportTimer.restart();
			}
			return true;
		}

		if (teleport2.contains((sf::Vector2i)ghost.getPosition())) {
			teleporting = true;
			if (teleportTimer.getElapsedTime().asSeconds() < 3.0f) {
				ghost.move(-GHOST_SPEED, 0.0f);
				return true;
			}
			ghost.move(GHOST_SPEED, 0.0f);
			if (ghost.getPosition().x - ghost.getGlobalBounds().width > SCREEN_WIDTH) {
				ghost.setPosition(float(teleport1.left + TILESIZE / 2), float(teleport1.top + TILESIZE / 2));
				teleportTimer.restart();
			}
			return true;
		}

		return false;

	}


	void Ghost::reset() {
		target = sf::Vector2i((TILESIZE * 14) / TILESIZE, (TILESIZE * 14 + TILESIZE / 2) / TILESIZE);
		eyes = true;
	}

	void Ghost::disableTurn() {
		//If all 4 ghosts have turned set turn to false
		if (turn) {
			turnCounter++;
			if (turnCounter == 4) {
				turnCounter = 0;
				turn = false;
			}
		}
	}


	void Ghost::fixTarget(const int map[MAP_RES_Y][MAP_RES_X]) {

		sf::Vector2i tempTarget = target;
		int index = 0;
		int amount = 1;

		do {
			tempTarget = target;

			switch (index) {
			case 0:
				tempTarget.y += amount;
				break;
			case 1:
				tempTarget.y -= amount;
				break;
			case 2:
				tempTarget.x += amount;
				break;
			case 3:
				tempTarget.x -= amount;
			}

			index++;
			if (index == 4) {
				index = 0;
				amount++;
			}

		} while (map[tempTarget.y][tempTarget.x] == 1);

		target = tempTarget;
	}


	void Ghost::draw() {
		
		//////////////////////////////////DEBUG////////////////////////////////////////////
		//Draw closest path to Pacman
		sf::CircleShape point(5);
		point.setOrigin(point.getGlobalBounds().width / 2, point.getGlobalBounds().height / 2);
		point.setFillColor(sf::Color::Red);

		sf::Vertex line[100];

		for (int i = 0; i < finalPath.size(); i++) {
			line[i] = sf::Vertex(sf::Vector2f(float(finalPath[i]->x * TILESIZE + TILESIZE / 2), float(finalPath[i]->y * TILESIZE + TILESIZE / 2)), sf::Color::Red);
		}
		if (finalPath.size()) {
			point.setPosition(float(finalPath[0]->x * TILESIZE + TILESIZE / 2), float(finalPath[0]->y * TILESIZE + TILESIZE / 2));
		}

		data->window.draw(line, finalPath.size(), sf::LineStrip);
		data->window.draw(point);
		//////////////////////////////////DEBUG////////////////////////////////////////////

		data->window.draw(ghost);
	}

	
	bool Ghost::findNewPath() {

		counter++;

		//check if new path should be calculated
		if (counter < TILESIZE / GHOST_SPEED) {
			return false;
		} else {
			counter = 0;
			return true;
		}
	}


	void Ghost::setMode(Mode& _mode) {

		if (_mode == Mode::Flee && mode != Mode::Flee) {
			turn = true;
			fleeModeTimer.restart();
		}

		mode = _mode;
	}
		

}