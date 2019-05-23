#include "Animation.hpp"

namespace Game {

	Animation::Animation(sf::Sprite& sprite, sf::IntRect* animationFrames, unsigned int size, float animationTime)
		: _sprite(sprite), _animationFrames(animationFrames), _size(size), _animationTime(animationTime)
	{
	}


	void Animation::animate() {
		if (_clock.getElapsedTime().asSeconds() > _animationTime / _size) {
			if (_animationiterator < _size - 1) {
				_animationiterator++;
			} else {
				_animationiterator = 0;
			}
			_sprite.setTextureRect(_animationFrames[_animationiterator]);
			_clock.restart();
		}
	}


}