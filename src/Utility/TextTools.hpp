#ifndef TEXTTOOLS_HPP
#define TEXTTOOLS_HPP

#include "PCH.hpp"

class TextTools
{
private:

public:
	static std::string wstringTOstring(std::wstring wstring);
	static std::wstring stringTOwstring(std::string string);

	static std::wstring getTranslation(std::string stringLabel);
	static sf::Text createTextLabel(std::wstring string, sf::Vector2f position = sf::Vector2f(), int characterSize = globals::defaultTextSize, std::string font = globals::defaultTextFont, bool centerOrigin = false, sf::Color textColor = sf::Color::White, uint32_t textStyle = sf::Text::Regular);
	static sf::Text createTranslatedTextLabel(std::string stringLabel, sf::Vector2f position = sf::Vector2f(), int characterSize = globals::defaultTextSize, std::string font = globals::defaultTextFont, bool centerOrigin = false, sf::Color textColor = sf::Color::White, uint32_t textStyle = sf::Text::Regular);
	static sf::Text createTextLabel(std::wstring string, sf::Vector2f boundingBox, bool allowWrapping, sf::Vector2f position = sf::Vector2f(), int characterSize = globals::defaultTextSize, std::string font = globals::defaultTextFont, bool centerOrigin = false, sf::Color textColor = sf::Color::White, uint32_t textStyle = sf::Text::Regular);
	static sf::Text createTranslatedTextLabel(std::string stringLabel, sf::Vector2f boundingBox, bool allowWrapping, sf::Vector2f position = sf::Vector2f(), int characterSize = globals::defaultTextSize, std::string font = globals::defaultTextFont, bool centerOrigin = false, sf::Color textColor = sf::Color::White, uint32_t textStyle = sf::Text::Regular);

	static std::string fromKeyToString(const sf::Keyboard::Key& key);

	TextTools();
	~TextTools();
};

#endif // !TEXTTOOLS_HPP
