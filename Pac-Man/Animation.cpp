#include "Animation.hpp"

namespace Game {

	Animation::Animation(sf::Sprite& sprite, sf::IntRect* animationFrames, unsigned int size, float animationTime)
		: sprite(sprite),
		  animationFrames(animationFrames),
		  size(size),
		  animationTime(animationTime)
	{
	}


	void Animation::animate() {
		if (clock.getElapsedTime().asSeconds() > animationTime / size) {
			if (animationiterator < size - 1) {
				animationiterator++;
			} else {
				animationiterator = 0;
			}
			sprite.setTextureRect(animationFrames[animationiterator]);
			clock.restart();
		}
	}

}