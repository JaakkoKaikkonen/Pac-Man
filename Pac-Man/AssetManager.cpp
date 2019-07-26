#include "AssetManager.hpp"

namespace Game {

	void AssetManager::loadTexture(std::string name, std::string fileName) {
		sf::Texture tex;

		if (tex.loadFromFile(fileName)) {
			textures[name] = tex;
		}
	}

	sf::Texture& AssetManager::getTexture(std::string name) {
		return textures.at(name);
	}

	void AssetManager::loadFont(std::string name, std::string fileName) {
		sf::Font font;

		if (font.loadFromFile(fileName)) {
			fonts[name] = font;
		}
	}

	sf::Font& AssetManager::getFont(std::string name) {
		return fonts.at(name);
	}

	void AssetManager::loadSound(std::string name, std::string fileName) {
		soundBuffers[name] = sf::SoundBuffer();
		sounds[name] = sf::Sound();

		if (soundBuffers[name].loadFromFile(fileName)) {
			sounds[name].setBuffer(soundBuffers[name]);
		}
	}

	sf::Sound& AssetManager::getSound(std::string name) {
		return sounds.at(name);
	}

}