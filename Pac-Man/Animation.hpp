#pragma once

#include <SFML/Graphics.hpp>

namespace Game {

	class Animation
	{
	public:
		Animation(sf::Sprite& sprite, sf::IntRect* animationFrames, unsigned int size, float animationTime);

		void animate();

		void reset() { animationiterator = 0; }

	private:
		sf::Clock clock;

		sf::Sprite& sprite;

		sf::IntRect* animationFrames;

		unsigned int size;

		float animationTime;

		unsigned int animationiterator = 0;
	};

}