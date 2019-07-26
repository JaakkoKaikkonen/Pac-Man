#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "AssetManager.hpp"
#include "State.hpp"

namespace Game {

	struct GameData {
		sf::RenderWindow window;
		AssetManager assets;

		State* state = nullptr;
		State* newState = nullptr;
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