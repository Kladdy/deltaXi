#ifndef RESOURCECONTROLLER_HPP
#define RESOURCECONTROLLER_HPP

#include "PCH.hpp"

class ResourceController
{
private:

public:
	static sf::Font loadFontFromMemory(const void* data, int sizeInBytes);
	static sf::Texture loadTextureFromMemory(const void* data, int sizeInBytes, bool setSmooth = true);
	static sf::SoundBuffer loadSoundBufferFromMemory(const void* data, int sizeInBytes);
	static sf::SoundBuffer loadMenuButtonSoundBufferFromMemory(int menuButtonIndex);

	static void addFont();
	static void addTexture();
	static sf::SoundBuffer addSoundbuffer(std::string filePath);

	static sf::Text addTextLabel();
	static sf::Sprite addSprite();
	static sf::CircleShape addCircleShape();
	static sf::RectangleShape addRectangleShape();

	ResourceController();
};

#endif // !RESOURCECONTROLLER_HPP
