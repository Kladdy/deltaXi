#include "ResourceController.hpp"
#include "../content/sound/menuSounds.hpp"

sf::Font ResourceController::loadFontFromMemory(const void* data, int sizeInBytes) {
	sf::Font font;
	if (!font.loadFromMemory(data, sizeInBytes)) {
		Logger::log("Failed to load font");
	}

	return font;
}

sf::Texture ResourceController::loadTextureFromMemory(const void* data, int sizeInBytes, bool setSmooth) {
	sf::Texture texture;
	if (!texture.loadFromMemory(data, sizeInBytes)) {
		Logger::log("Failed to load texture");
	}

	texture.setSmooth(setSmooth);
	return texture;
}

sf::SoundBuffer ResourceController::loadSoundBufferFromMemory(const void* data, int sizeInBytes)
{
	sf::SoundBuffer buffer;
	if (!(buffer.loadFromMemory(data, sizeInBytes))) {
		Logger::log("Failed to load soundbuffer");
	}

	return buffer;
}

sf::SoundBuffer ResourceController::loadMenuButtonSoundBufferFromMemory(int menuButtonIndex)
{
	const void* data;
	static unsigned int sizeInBytes;

	if (menuButtonIndex == 0) { data = (void*)menuTraverse0;
								sizeInBytes = menuTraverse0_length; }
	else if (menuButtonIndex == 1) { data = (void*)menuTraverse1;
								sizeInBytes = menuTraverse1_length; }
	else if (menuButtonIndex == 2) { data = (void*)menuTraverse2;
								sizeInBytes = menuTraverse2_length; }
	else if (menuButtonIndex == 3) { data = (void*)menuTraverse3;
								sizeInBytes = menuTraverse3_length; }
	else if (menuButtonIndex == 4) { data = (void*)menuTraverse4;
								sizeInBytes = menuTraverse4_length; }
	else if (menuButtonIndex == 5) { data = (void*)menuTraverse5;
								sizeInBytes = menuTraverse5_length; }
	else if (menuButtonIndex == 6) { data = (void*)menuTraverse6;
								sizeInBytes = menuTraverse6_length; }
	else if (menuButtonIndex == 7) { data = (void*)menuTraverse7;
								sizeInBytes = menuTraverse7_length; }
	else if (menuButtonIndex == 8) { data = (void*)menuTraverse8;
								sizeInBytes = menuTraverse8_length; }
	else if (menuButtonIndex == 9) { data = (void*)menuTraverse9;
								sizeInBytes = menuTraverse9_length; }
	else if (menuButtonIndex == 10) { data = (void*)menuTraverse10;
								sizeInBytes = menuTraverse10_length; }
	else return sf::SoundBuffer();

	sf::SoundBuffer buffer;
	if (!(buffer.loadFromMemory(data, sizeInBytes))) {
		Logger::log("Failed to load soundbuffer");
	}

	return buffer;
}

void ResourceController::addFont()
{
	Logger::log("Not implemented");
}

void ResourceController::addTexture()
{
	Logger::log("Not implemented");
}

sf::SoundBuffer ResourceController::addSoundbuffer(std::string filePath)
{
	sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile(filePath))
        return sf::SoundBuffer();

	return soundBuffer;
}

sf::Text ResourceController::addTextLabel()
{
	Logger::log("Not implemented");

	return sf::Text();
}

sf::Sprite ResourceController::addSprite()
{
	Logger::log("Not implemented");

	return sf::Sprite();
}

sf::CircleShape ResourceController::addCircleShape()
{
	Logger::log("Not implemented");

	return sf::CircleShape();
}

sf::RectangleShape ResourceController::addRectangleShape()
{
	Logger::log("Not implemented");

	return sf::RectangleShape();
}

ResourceController::ResourceController()
{

}