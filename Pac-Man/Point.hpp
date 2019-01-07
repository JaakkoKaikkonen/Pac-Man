#pragma once

#include <SFML\Graphics.hpp>
#include "Game.hpp"
#include <string>

namespace engine {

	class Point
	{
	public:
		Point() = default;
		~Point();

		void init(gameDataRef data, std::string texture);

		void draw(const int x, const int y);

	private:
		gameDataRef _data;

		sf::Sprite _point;

	};


}