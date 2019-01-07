#pragma once

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "DEFINITIONS.hpp"
#include "Animation.hpp"


namespace engine {

	class Ghost
	{
	public:

		struct Node {
			int x;
			int y;
			int g;
			int h;
			int f;
			Node* cameFrom = nullptr;
		};


		Ghost(gameDataRef data);

		void findPath(const int map[MAP_RES_Y][MAP_RES_X]);

		void draw();

		void update();

		void reset();

		void disableTurn();

		sf::Vector2f getPosition() { return _ghost.getPosition(); }

		sf::Sprite& getSprite() { return _ghost; }

		bool getEyes() { return _eyes;  }

		virtual void setTarget(const sf::Vector2f& pacmanPosition, const Dir& pacmandir, const sf::Vector2f& blinkyPosition, const int map[MAP_RES_Y][MAP_RES_X]) = 0;

		virtual void boxAnimation() = 0;

		virtual bool isFree() = 0;

		static bool findNewPath();

		static void setMode(Mode& _mode);

	protected:
		void fixTarget(const int map[MAP_RES_Y][MAP_RES_X]);

	private:
		void animate();

		bool teleport();

		int hScore(Node* a, Node* b);

	protected:
		gameDataRef _data;

		sf::Sprite _ghost;

		sf::Vector2i _target;

		sf::Vector2i _previousTarget;

		Dir _dir = Dir::Right;

		sf::Vector2f _dirVector;

		sf::IntRect _animationFramesRight[2];
		sf::IntRect _animationFramesLeft[2];
		sf::IntRect _animationFramesUp[2];
		sf::IntRect _animationFramesDown[2];

		bool _eyes = false;
		
		bool _teleport = false;

		static bool turn;

		static int turnCounter;

		static sf::Clock fleeModeTimer;

		static Mode mode;

		static int counter;

	private:
		Node _grid[MAP_RES_X][MAP_RES_Y];

		std::vector<Node*> _closedSet;

		std::vector<Node*> _openSet;

		std::vector<Node*> _finalPath;

		sf::IntRect _teleport1, _teleport2;

		sf::Clock _teleportTimer;

		sf::IntRect _animationFramesFlee01[2];
		sf::IntRect _animationFramesFlee02[4];

		Animation _animationRight;
		Animation _animationLeft;
		Animation _animationUp;
		Animation _animationDown;
		Animation _fleeAnimation01;
		Animation _fleeAnimation02;

	};

}

