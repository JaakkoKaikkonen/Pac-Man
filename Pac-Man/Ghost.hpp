#pragma once

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "DEFINITIONS.hpp"
#include "Animation.hpp"


namespace Game {

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

		sf::Vector2f getPosition() { return ghost.getPosition(); }

		sf::Sprite& getSprite() { return ghost; }

		bool getEyes() { return eyes; }

		void setTarget(const sf::Vector2f& pacmanPosition, const Dir& pacmandir, const sf::Vector2f& blinkyPosition, const int map[MAP_RES_Y][MAP_RES_X]);

		virtual bool isFree() = 0;

		static bool findNewPath();

		static void setMode(Mode& _mode);

	protected:
		void fixTarget(const int map[MAP_RES_Y][MAP_RES_X]);

	private:
		virtual void chase(const sf::Vector2f& pacmanPosition, const Dir& pacmandir, const sf::Vector2f& blinkyPosition, const int map[MAP_RES_Y][MAP_RES_X]) = 0;

		virtual void scatter() = 0;

		virtual void boxAnimation() = 0;

		void animate();

		bool teleport();

		int hScore(Node* a, Node* b);

	protected:
		gameDataRef data;

		sf::Sprite ghost;

		sf::Color ghostColor = sf::Color::Red;

		sf::Vector2i target;

		Dir dir = Dir::Right;

		sf::Vector2f dirVector;

		sf::IntRect animationFramesRight[2];
		sf::IntRect animationFramesLeft[2];
		sf::IntRect animationFramesUp[2];
		sf::IntRect animationFramesDown[2];

		bool eyes = false;
		
		bool teleporting = false;

		static bool turn;

		static int turnCounter;

		static sf::Clock fleeModeTimer;

		static Mode mode;

		static int counter;

		sf::Vector2i homeCorner;

		int boxCounter = 0;

		bool go = false;

		bool start = true;

		Dir spot;

	private:
		Node grid[MAP_RES_X][MAP_RES_Y];

		std::vector<Node*> closedSet;

		std::vector<Node*> openSet;

		std::vector<Node*> finalPath;

		static sf::IntRect teleport1, teleport2;

		sf::Clock teleportTimer;

		sf::IntRect animationFramesFlee01[2];
		sf::IntRect animationFramesFlee02[4];

		Animation animationRight;
		Animation animationLeft;
		Animation animationUp;
		Animation animationDown;
		Animation fleeAnimation01;
		Animation fleeAnimation02;

	};

}

