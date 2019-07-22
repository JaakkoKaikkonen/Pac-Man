#pragma once

#include <SFML/Graphics.hpp>

namespace Collision {

	bool checkSpriteCollision(const sf::Sprite& sprite1, const float& scale1, const sf::Sprite& sprite2, const float& scale2);

}