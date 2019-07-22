#include <SFML/Graphics.hpp>

namespace Collision {

	bool checkSpriteCollision(const sf::Sprite& sprite1, const float& scale1, const sf::Sprite& sprite2, const float& scale2) {

		sf::FloatRect rect1 = sprite1.getGlobalBounds();
		sf::FloatRect rect2 = sprite2.getGlobalBounds();

		rect1.left += (rect1.width - rect1.width * scale1) / 2;
		rect1.top += (rect1.height - rect1.height * scale1) / 2;
		rect2.left += (rect2.width - rect2.width * scale2) / 2;
		rect2.top += (rect2.height - rect2.height * scale2) / 2;

		rect1.width *= scale1;
		rect1.height *= scale1;
		rect2.width *= scale2;
		rect2.height *= scale2;

		return rect1.intersects(rect2);
	}

}