#include "Point.hpp"

namespace engine {

	void Point::init(gameDataRef data, std::string texture) {
		_data = data;
		_point.setTexture(_data->assets.getTexture(texture));
		_point.setOrigin(_point.getGlobalBounds().width / 2, _point.getGlobalBounds().height / 2);
	}

	Point::~Point()
	{
	}

	void Point::draw(const int x, const int y) {
		_point.setPosition((float)x, (float)y);
		_data->window.draw(_point);
	}


}