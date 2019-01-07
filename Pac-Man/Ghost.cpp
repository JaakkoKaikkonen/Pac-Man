#include "Ghost.hpp"
#include <iostream>
#include <vector>
#include "Utility.hpp"


namespace engine {

	Ghost::Ghost(gameDataRef data)
		: _data(data), _teleport1(-TILESIZE*2 + TILESIZE/4, TILESIZE*17, TILESIZE*8, TILESIZE), _teleport2(TILESIZE*22 - TILESIZE/4, TILESIZE*17, TILESIZE*8, TILESIZE),
		_animationRight(_ghost, _animationFramesRight, 2, GHOST_ANIMATION_TIME), _animationLeft(_ghost, _animationFramesLeft, 2, GHOST_ANIMATION_TIME), 
		_animationUp(_ghost, _animationFramesUp, 2, GHOST_ANIMATION_TIME), _animationDown(_ghost, _animationFramesDown, 2, GHOST_ANIMATION_TIME), 
		_fleeAnimation01(_ghost, _animationFramesFlee01, 2, GHOST_ANIMATION_TIME), _fleeAnimation02(_ghost, _animationFramesFlee02, 4, GHOST_ANIMATION_TIME)
	{
		_closedSet.reserve(100);
		_openSet.reserve(100);
		_finalPath.reserve(50);


		_animationFramesFlee01[0] = GHOST_FLEE_BLUE_01;
		_animationFramesFlee01[1] = GHOST_FLEE_BLUE_02;

		_animationFramesFlee02[0] = GHOST_FLEE_BLUE_01;
		_animationFramesFlee02[1] = GHOST_FLEE_BLUE_02;
		_animationFramesFlee02[2] = GHOST_FLEE_WHITE_01;
		_animationFramesFlee02[3] = GHOST_FLEE_WHITE_02;

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

		if (!_teleport) {
			//A* algorithm / Find shortest path
			//Initialize Nodes
			for (int i = 0; i < MAP_RES_X; i++) {
				for (int j = 0; j < MAP_RES_Y; j++) {
					_grid[i][j] = Node { i, j };
				}
			}

			//Clear openSet && closetSet && finalPath
			_openSet.clear();
			_closedSet.clear();
			_finalPath.clear();

			//Set start
			Node* start = &_grid[(int)(_ghost.getPosition().x / TILESIZE)][(int)(_ghost.getPosition().y / TILESIZE)];

			//std::cout << _target.x << ", " << _target.y << std::endl;

			//Set goal
			Node* goal = &_grid[_target.x][_target.y];

			//Set startNodes fScore
			start->f = hScore(start, goal);

			//Push start to openSet
			_openSet.push_back(start);


			while (!_openSet.empty()) {

				//Find the node with lowest fScore
				int winner = 0;
				for (int i = 0; i < _openSet.size(); i++) {
					if (_openSet[i]->f < _openSet[winner]->f) {
						winner = i;
					}
				}
				//Set current equal to node in open set with lowest fScore
				Node* current = _openSet[winner];

				//Clamp current->x so it wont crash when pacman teleports
				current->x = utility::clamp(current->x, 1, MAP_RES_X - 2);

				//Check if path was found
				if (current == goal) {
					_finalPath.push_back(current);

					Node* temp = current;
					while (temp->cameFrom) {
						_finalPath.push_back(temp->cameFrom);
						temp = temp->cameFrom;
					}
					break;
				}

				//Remove current from openSet
				_openSet[winner] = _openSet[_openSet.size() - 1];
				_openSet.pop_back();

				//Add current to closedSet
				_closedSet.push_back(current);

				//Find current nodes neighbors
				std::vector<Node*> neighbors;
				neighbors.reserve(4);
				neighbors.push_back(&_grid[current->x + 1][current->y]);
				neighbors.push_back(&_grid[current->x - 1][current->y]);
				neighbors.push_back(&_grid[current->x][current->y + 1]);
				neighbors.push_back(&_grid[current->x][current->y - 1]);

				//If current == start delete neighbor behind ghost
				if (current->x == start->x && current->y == start->y && !turn && !_eyes) {
					//Find spot behind ghost
					sf::Vector2i spotBehindGhost;
					if (Dir::Right == _dir) {
						spotBehindGhost.x = start->x - 1;
						spotBehindGhost.y = start->y;
					} else if (Dir::Left == _dir) {
						spotBehindGhost.x = start->x + 1;
						spotBehindGhost.y = start->y;
					} else if (Dir::Down == _dir) {
						spotBehindGhost.x = start->x;
						spotBehindGhost.y = start->y - 1;
					} else if (Dir::Up == _dir) {
						spotBehindGhost.x = start->x;
						spotBehindGhost.y = start->y + 1;
					}
					for (int i = 0; i < neighbors.size(); i++) {
						if (neighbors[i]->x == spotBehindGhost.x && neighbors[i]->y == spotBehindGhost.y) {
							neighbors[i] = neighbors[neighbors.size() - 1];
							neighbors.pop_back();
							break;
						}
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
					for (int j = 0; j < _closedSet.size(); j++) {
						if (neighbors[i] == _closedSet[j]) {
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
					for (int j = 0; j < _openSet.size(); j++) {
						if (neighbors[i] == _openSet[j]) {
							neighborIsInOpenSet = true;
						}
					}

					//If neighbor is not in openSet add it  //If gScore is worse than the old one -> go to next 
					if (!neighborIsInOpenSet) {
						_openSet.push_back(neighbors[i]);
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
			if (_finalPath.size() > 1) {
				_dirVector.x = (float)_finalPath[_finalPath.size() - 2]->x * TILESIZE - start->x * TILESIZE;
				_dirVector.y = (float)_finalPath[_finalPath.size() - 2]->y * TILESIZE - start->y * TILESIZE;

				_dirVector /= (float)TILESIZE;
				_dirVector *= GHOST_SPEED;
			} else {
				_dirVector.x = 0;
				_dirVector.y = 0;
			}

			//////////////////////////////DEBUG////////////////////////////////
			if (_dirVector.x == 0 && _dirVector.y == 0 && mode != Mode::Flee && !_eyes) {
				_dirVector.x = 0;
			}

		}

		

		
	}

	void Ghost::update() {

		if (!this->isFree()) {

			this->boxAnimation();

		} 

		//Set direction
		if (_dirVector.x > 0) {
			_dir = Dir::Right;
		} else if (_dirVector.x < 0) {
			_dir = Dir::Left;
		} else if (_dirVector.y > 0) {
			_dir = Dir::Down;
		} else if (_dirVector.y < 0) {
			_dir = Dir::Up;
		}

		//Animation
		this->animate();

		if (!this->teleport()) {

			_ghost.move(_dirVector);

		}
		
	}

	void Ghost::animate() {
	
		if (Mode::Flee == mode && !_eyes) {
			if (fleeModeTimer.getElapsedTime().asSeconds() < PACMAN_POWER_TIME - 1.5f) {
				_fleeAnimation01.animate();
				_fleeAnimation02.reset();
			} else {
				_fleeAnimation02.animate();
			}
		} else if (_dir == Dir::Right) {
			if (_eyes) {
				_ghost.setTextureRect(GHOST_EYES_RIGHT);
			} else {
				_animationRight.animate();
			}
		} else if (_dir == Dir::Left) {
			if (_eyes) {
				_ghost.setTextureRect(GHOST_EYES_LEFT);
			} else {
				_animationLeft.animate();
			}
		} else if (_dir == Dir::Down) {
			if (_eyes) {
				_ghost.setTextureRect(GHOST_EYES_DOWN);
			} else {
				_animationDown.animate();
			}
		} else if (_dir == Dir::Up) {
			if (_eyes) {
				_ghost.setTextureRect(GHOST_EYES_UP);
			} else {
				_animationUp.animate();
			}
		}

	}

	bool Ghost::teleport() {

		_teleport = false;

		if (_teleport1.contains((sf::Vector2i)_ghost.getPosition())) {
			_teleport = true;
			if (_teleportTimer.getElapsedTime().asSeconds() < 3.0f) {
				_ghost.move(GHOST_SPEED, 0.0f);
				return true;
			}
			_ghost.move(-GHOST_SPEED, 0.0f);
			if (_ghost.getPosition().x + _ghost.getGlobalBounds().width < 0) {
				_ghost.setPosition(float(_teleport2.left + TILESIZE * 7 + TILESIZE / 2), float(_teleport2.top + TILESIZE / 2));
				_teleportTimer.restart();
			}
			return true;
		}

		if (_teleport2.contains((sf::Vector2i)_ghost.getPosition())) {
			_teleport = true;
			if (_teleportTimer.getElapsedTime().asSeconds() < 3.0f) {
				_ghost.move(-GHOST_SPEED, 0.0f);
				return true;
			}
			_ghost.move(GHOST_SPEED, 0.0f);
			if (_ghost.getPosition().x - _ghost.getGlobalBounds().width > SCREEN_WIDTH) {
				_ghost.setPosition(float(_teleport1.left + TILESIZE / 2), float(_teleport1.top + TILESIZE / 2));
				_teleportTimer.restart();
			}
			return true;
		}

		return false;

	}


	void Ghost::reset() {
		_target = sf::Vector2i((TILESIZE * 14) / TILESIZE, (TILESIZE * 14 + TILESIZE / 2) / TILESIZE);
		_eyes = true;
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

		sf::Vector2i tempTarget = _target;
		int index = 0;
		int amount = 1;

		do {
			tempTarget = _target;

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

		_target = tempTarget;
	}


	void Ghost::draw() {
		
		//////////////////////////////////DEBUG////////////////////////////////////////////
		sf::CircleShape point(5);
		point.setOrigin(point.getGlobalBounds().width / 2, point.getGlobalBounds().height / 2);
		point.setFillColor(sf::Color::Red);

		sf::Vertex line[100];

		for (int i = 0; i < _finalPath.size(); i++) {
			line[i] = sf::Vertex(sf::Vector2f(float(_finalPath[i]->x * TILESIZE + TILESIZE / 2), float(_finalPath[i]->y * TILESIZE + TILESIZE / 2)), sf::Color::Red);
		}
		if (_finalPath.size()) {
			point.setPosition(float(_finalPath[0]->x * TILESIZE + TILESIZE / 2), float(_finalPath[0]->y * TILESIZE + TILESIZE / 2));
		}

		_data->window.draw(line, _finalPath.size(), sf::LineStrip);
		_data->window.draw(point);
		//////////////////////////////////DEBUG////////////////////////////////////////////

		_data->window.draw(_ghost);
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