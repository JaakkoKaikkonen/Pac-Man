#pragma once

#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace Game {

	class AssetManager {

	public:
		void loadTexture(std::string name, std::string fileName);
		sf::Texture& getTexture(std::string name);

		void loadFont(std::string name, std::string fileName);
		sf::Font& getFont(std::string name);

		void loadSound(std::string name, std::string fileName);
		sf::Sound& getSound(std::string name);

	private:
		std::map<std::string, sf::Texture> textures;
		std::map<std::string, sf::Font> fonts;
		std::map<std::string, sf::SoundBuffer> soundBuffers;
		std::map<std::string, sf::Sound> sounds;

	};

}