#include "AssetManager.hpp"

namespace Game {

	void AssetManager::loadTexture(std::string name, std::string fileName) {
		sf::Texture tex;

		if (tex.loadFromFile(fileName)) {
			this->textures[name] = tex;
		}
	}

	sf::Texture& AssetManager::getTexture(std::string name) {
		return this->textures.at(name);
	}

	void AssetManager::loadFont(std::string name, std::string fileName) {
		sf::Font font;

		if (font.loadFromFile(fileName)) {
			this->fonts[name] = font;
		}
	}

	sf::Font& AssetManager::getFont(std::string name) {
		return this->fonts.at(name);
	}

	void AssetManager::loadSound(std::string name, std::string fileName) {
		soundBuffers[name] = sf::SoundBuffer();
		sounds[name] = sf::Sound();

		if (soundBuffers[name].loadFromFile(fileName)) {
			sounds[name].setBuffer(soundBuffers[name]);
		}
	}

	sf::Sound& AssetManager::getSound(std::string name) {
		return this->sounds.at(name);
	}

}