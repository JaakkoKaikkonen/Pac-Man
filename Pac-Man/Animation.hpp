#pragma once

#include <SFML/Graphics.hpp>

namespace Game {

	class Animation
	{
	public:
		Animation(sf::Sprite& sprite, sf::IntRect* animationFrames, unsigned int size, float animationTime);

		void animate();

		void reset() { _animationiterator = 0; }

	private:
		sf::Clock _clock;

		sf::Sprite& _sprite;

		sf::IntRect* _animationFrames;

		unsigned int _size;

		float _animationTime;

		unsigned int _animationiterator = 0;
	};

}