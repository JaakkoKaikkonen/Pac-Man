#include "Point.hpp"

namespace Game {

	void Point::init(gameDataRef data, std::string texture) {
		this->data = data;
		point.setTexture(data->assets.getTexture(texture));
		point.setOrigin(point.getGlobalBounds().width / 2, point.getGlobalBounds().height / 2);
	}

	void Point::draw(const int x, const int y) {
		point.setPosition((float)x, (float)y);
		data->window.draw(point);
	}

}