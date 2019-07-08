#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "StateMachine.hpp"
#include "AssetManager.hpp"

namespace Game {

	struct GameData {
		StateMachine machine;
		sf::RenderWindow window;
		AssetManager assets;
	};

	typedef std::shared_ptr<GameData> gameDataRef;

	class Game {
	public:
		Game(int width, int height, std::string title);

	private:
		void run();

	private:
		sf::Clock clock;

		const float dt = 1.0f / 60.0f;

		gameDataRef data = std::make_shared<GameData>();

	};

}
