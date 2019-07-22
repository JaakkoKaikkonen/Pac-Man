#include <SFML\Graphics.hpp>
#include <algorithm>


namespace utility {

	float normalize(const float& value, float min, float max) {
		return (value - min) / (max - min);
	}

	float lerp(const float& value, float min, float max) {
		return (max - min) * value + min;
	}

	float map(const float& value, float min1, float max1, float min2, float max2) {
		return lerp(normalize(value, min1, max1), min2, max2);
	}

	float clamp(const float& value, float min, float max) {
		return std::min(std::max(value, min), max);
	}

	int clamp(const int& value, int min, int max) {
		return std::min(std::max(value, min), max);
	}

	sf::Vector2f clamp(const sf::Vector2f& vector, float minX, float maxX, float minY, float maxY) {
		return sf::Vector2f(clamp(vector.x, minX, maxX), clamp(vector.y, minY, maxY));
	}

	sf::Vector2i clamp(const sf::Vector2i& vector, int minX, int maxX, int minY, int maxY) {
		return sf::Vector2i(clamp(vector.x, minX, maxX), clamp(vector.y, minY, maxY));
	}

}