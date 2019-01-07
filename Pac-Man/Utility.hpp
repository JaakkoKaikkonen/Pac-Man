#pragma once

#include <SFML\Graphics.hpp>

namespace utility {

	float normalize(const float& value, float min, float max);

	float lerp(const float& value, float min, float max);

	float map(const float& value, float min1, float max1, float min2, float max2);

	float clamp(const float& value, float min, float max);

	int clamp(const int& value, int min, int max);

	sf::Vector2f clamp(const sf::Vector2f& vector, float minX, float maxX, float minY, float maxY);

	sf::Vector2i clamp(const sf::Vector2i& vector, int minX, int maxX, int minY, int maxY);

}

