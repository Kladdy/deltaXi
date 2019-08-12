#include "TextTools.hpp"

namespace
{
	sf::Vector2f getCenter(sf::Text* text)
	{
		sf::Vector2f center;

		center.x = round(text->getLocalBounds().width / 2.f);
		center.y = round(text->getLocalBounds().height / 2.f);

		return center;
	}

	bool heightIsInBounds(sf::Text* text, int maxHeight)
	{
		return (text->getLocalBounds().height <= maxHeight || round(maxHeight) == 0);
	}

	bool widthIsInBounds(sf::Text* text, int maxWidth)
	{
		return (text->getLocalBounds().width <= maxWidth || round(maxWidth) == 0);
	}

	bool lowerFontSizeLimitReached(int fontSize)
	{
		return (fontSize <= globals::minTextSize);
	}

	std::wstring wrapLimitedWidth(sf::Text* text, std::wstring string, int maxWidth)
	{
		std::wstring temp = L"";
		std::wstring finalString = L"";
		std::vector<std::wstring> words;

		for (size_t i = 0; i < string.length(); i++)
		{
			if (string[i] == ' ')
			{
				words.push_back(temp);
				temp = L"";
			}
			else if (string[i] == '-')
			{
                temp.append(L"-");
				words.push_back(temp);
				temp = L"";
			}
			else if (string[i] == '\\' && string[i+1] == 'n')
			{
				words.push_back(temp);
				temp = L"";
			}
			else
			{
				wchar_t c = string[i];
				std::wstring s(1, c);
				temp.append(s);

				if (i == string.length() - 1) // If we have reached the last character
				{
					words.push_back(temp);
					break;
				}
			}
		}

		finalString.append(words[0]);
		text->setString(finalString);

		for (size_t j = 1; j < words.size(); j++)
		{
			if (words[j-1].find(L"\n") != std::wstring::npos) {
				temp = finalString + words[j];
				text->setString(temp);

				if (!widthIsInBounds(text, maxWidth))
					return string; // Could not wrap, still too big.

				finalString = temp;
			}
			else
			{
				temp = finalString + L" " + words[j];
				text->setString(temp);

				if (!widthIsInBounds(text, maxWidth))
				{
					finalString.append(L"\n" + words[j]);
					text->setString(finalString);
					if (!widthIsInBounds(text, maxWidth))
						return string; // Could not wrap, still too big.
				} else {
					finalString = temp;
				}
			}
		}

		return finalString;
	}
}

std::string TextTools::wstringTOstring(std::wstring wstring)
{
    sf::String s(wstring);
    return s.toAnsiString();
}

std::wstring TextTools::stringTOwstring(std::string string)
{
    sf::String s(string);
    return s.toWideString();
}

std::wstring TextTools::getTranslation(std::string stringLabel)
{
	return globals::translation.translate(stringLabel);
}

// Creates a sf::Text with the given parameters
sf::Text TextTools::createTextLabel(std::wstring string, sf::Vector2f position, int characterSize, std::string font, bool centerOrigin, sf::Color textColor, uint32_t textStyle)
{
	sf::Text text;

	text.setString(string);
	text.setCharacterSize(characterSize);
	text.setFont(globals::loadedFonts[font]);
	text.setFillColor(textColor);
	text.setStyle(textStyle);
	if (centerOrigin)
		text.setOrigin(getCenter(&text));
	text.setPosition(VectorTools::roundVector2f(position));

	return text;
}

// Creates a translated sf::Text with the given parameters
sf::Text TextTools::createTranslatedTextLabel(std::string stringLabel, sf::Vector2f position, int characterSize, std::string font, bool centerOrigin, sf::Color textColor, uint32_t textStyle)
{
	sf::Text text;

	std::wstring string = TextTools::getTranslation(stringLabel);
	text.setString(string);
	text.setCharacterSize(characterSize);
	text.setFont(globals::loadedFonts[font]);
	text.setFillColor(textColor);
	text.setStyle(textStyle);
	if (centerOrigin)
		text.setOrigin(getCenter(&text));
	text.setPosition(VectorTools::roundVector2f(position));

	return text;
}

// Creates a sf::Text with the given parameters. If the text is wider than is specified in boundingBox's
// x-component, the function will attempt to put in a space where it deems possible (at a - or after space)
// The same goes for the height. If either component is 0, that component will not be limited. If x is
// limited, and the height has reached the limit, the function will decrease the font size and try again.
// Any use of \n will be interpreted as a new line, and will immediately break.
sf::Text TextTools::createTextLabel(std::wstring string, sf::Vector2f boundingBox, bool allowWrapping, sf::Vector2f position, int characterSize, std::string font, bool centerOrigin, sf::Color textColor, uint32_t textStyle)
{
	if (boundingBox == sf::Vector2f())
		Logger::log("Bounding box is empty for the text box, consider using an unbounded text box");

	sf::Text text;

	text.setString(string);
	text.setCharacterSize(characterSize);
	text.setFont(globals::loadedFonts[font]);
	text.setFillColor(textColor);
	text.setStyle(textStyle);

	bool limitWidth = (boundingBox.x != 0);
	bool limitHeight = (boundingBox.y != 0);
	bool widthContained = widthIsInBounds(&text, boundingBox.x);
	bool heightContained = heightIsInBounds(&text, boundingBox.y);
	bool doDecreaseFontSize = false;

	while (!widthContained || !heightContained)
	{
		if (!widthContained)
		{
			if (allowWrapping) // Do wrapping (if allowed)
			{
				std::wstring temp = wrapLimitedWidth(&text, string, boundingBox.x);
				text.setString(temp);

				if (!widthIsInBounds(&text, boundingBox.x))
				{
					doDecreaseFontSize = true;
				}
			}
			else // Otherwise, just make the font smaller and try again next iteration
			{
				doDecreaseFontSize = true;
			}
		}

		heightContained = heightIsInBounds(&text, boundingBox.y);

		if (!heightContained) // If the height is too big, limit it by decreasing the font size
		{
			doDecreaseFontSize = true;
		}

		if (doDecreaseFontSize)
		{
			characterSize--;
			if (lowerFontSizeLimitReached(characterSize)) // Check if the character size is too small
			{
				Logger::log("Text size under threshold for text " + TextTools::wstringTOstring(string));
				break;
			}

			text.setCharacterSize(characterSize);
			doDecreaseFontSize = false;
		}

		widthContained = widthIsInBounds(&text, boundingBox.x);
		heightContained = heightIsInBounds(&text, boundingBox.y);
	}

	if (!widthContained  || !heightContained)
		Logger::log("Was not able to contain text. Bounds: X=" + std::to_string(boundingBox.x) + " Y=" + std::to_string(boundingBox.y));

	if (centerOrigin)
		text.setOrigin(getCenter(&text));
	text.setPosition(VectorTools::roundVector2f(position));

	return text;
}

// Creates a translated sf::Text with the given parameters. If the text is wider than is specified in boundingBox's
// x-component, the function will attempt to put in a space where it deems possible (at a - or after space)
// The same goes for the height. If either component is 0, that component will not be limited. If x is
// limited, and the height has reached the limit, the function will decrease the font size and try again.
// Any use of \n will be interpreted as a new line, and will immediately break.
sf::Text TextTools::createTranslatedTextLabel(std::string stringLabel, sf::Vector2f boundingBox, bool allowWrapping, sf::Vector2f position, int characterSize, std::string font, bool centerOrigin, sf::Color textColor, uint32_t textStyle)
{
	if (boundingBox == sf::Vector2f())
		Logger::log("Bounding box is empty for label " + stringLabel + ", consider using an unbounded text box");

	sf::Text text;

	std::wstring string = TextTools::getTranslation(stringLabel);
	text.setString(string);
	text.setCharacterSize(characterSize);
	text.setFont(globals::loadedFonts[font]);
	text.setFillColor(textColor);
	text.setStyle(textStyle);

	bool limitWidth = (boundingBox.x != 0);
	bool limitHeight = (boundingBox.y != 0);
	bool widthContained = widthIsInBounds(&text, boundingBox.x);
	bool heightContained = heightIsInBounds(&text, boundingBox.y);
	bool doDecreaseFontSize = false;

	while (!widthContained || !heightContained)
	{
		if (!widthContained)
		{
			if (allowWrapping) // Do wrapping (if allowed)
			{
				std::wstring temp = wrapLimitedWidth(&text, string, boundingBox.x);
				text.setString(temp);

				if (!widthIsInBounds(&text, boundingBox.x))
				{
					doDecreaseFontSize = true;
				}
			}
			else // Otherwise, just make the font smaller and try again next iteration
			{
				doDecreaseFontSize = true;
			}
		}

		heightContained = heightIsInBounds(&text, boundingBox.y);

		if (!heightContained) // If the height is too big, limit it by decreasing the font size
		{
			doDecreaseFontSize = true;
		}

		if (doDecreaseFontSize)
		{
			characterSize--;
			if (lowerFontSizeLimitReached(characterSize)) // Check if the character size is too small
			{
				Logger::log("Text size under threshold for text " + stringLabel);
				break;
			}

			text.setCharacterSize(characterSize);
			doDecreaseFontSize = false;
		}

		widthContained = widthIsInBounds(&text, boundingBox.x);
		heightContained = heightIsInBounds(&text, boundingBox.y);
	}

	if (!widthContained  || !heightContained)
		Logger::log("Was not able to contain text '" + stringLabel + "'. Bounds: X=" + std::to_string(boundingBox.x) + " Y=" + std::to_string(boundingBox.y));

	if (centerOrigin)
		text.setOrigin(getCenter(&text));
	text.setPosition(VectorTools::roundVector2f(position));

	return text;
}

std::string TextTools::fromKeyToString(const sf::Keyboard::Key& key){
        std::string ret;
        switch(key){

    case sf::Keyboard::A :

        ret="A";
        break;
    case sf::Keyboard::B :

        ret="B";
        break;
    case sf::Keyboard::C :

        ret="C";
        break;
    case sf::Keyboard::D :

        ret="D";
        break;
    case sf::Keyboard::E :

        ret="E";
        break;
    case sf::Keyboard::F :

        ret="F";
        break;
    case sf::Keyboard::G :

        ret="G";
        break;
    case sf::Keyboard::H :

        ret="H";
        break;
    case sf::Keyboard::I :

        ret="I";
        break;
    case sf::Keyboard::J :

        ret="J";
        break;
    case sf::Keyboard::K :

        ret="K";
        break;
    case sf::Keyboard::L :

        ret="L";
        break;
    case sf::Keyboard::M :

        ret="M";
        break;
    case sf::Keyboard::N :

        ret="N";
        break;
    case sf::Keyboard::O :

        ret="O";
        break;
    case sf::Keyboard::P :

        ret="P";
        break;
    case sf::Keyboard::Q :

        ret="Q";
        break;
    case sf::Keyboard::R :

        ret="R";
        break;
    case sf::Keyboard::S :

        ret="S";
        break;
    case sf::Keyboard::T :

        ret="T";
        break;
    case sf::Keyboard::U :

        ret="U";
        break;
    case sf::Keyboard::V :

        ret="V";
        break;
    case sf::Keyboard::W :

        ret="W";
        break;
    case sf::Keyboard::X :

        ret="X";
        break;
    case sf::Keyboard::Y :

        ret="Y";
        break;
    case sf::Keyboard::Z :

        ret="Z";
        break;
    case sf::Keyboard::Num0 :

        ret="Num0";
        break;
    case sf::Keyboard::Num1 :

        ret="Num1";
        break;
    case sf::Keyboard::Num2 :

        ret="Num2";
        break;
    case sf::Keyboard::Num3 :

        ret="Num3";
        break;
    case sf::Keyboard::Num4 :

        ret="Num4";
        break;
    case sf::Keyboard::Num5 :

        ret="Num5";
        break;
    case sf::Keyboard::Num6 :

        ret="Num6";
        break;
    case sf::Keyboard::Num7 :

        ret="Num7";
        break;
    case sf::Keyboard::Num8 :

        ret="Num8";
        break;
    case sf::Keyboard::Num9 :

        ret="Num9";
        break;
    case sf::Keyboard::Escape :

        ret="Escape";
        break;
    case sf::Keyboard::LControl :

        ret="LControl";
        break;
    case sf::Keyboard::LShift :

        ret="LShift";
        break;
    case sf::Keyboard::LAlt :

        ret="LAlt";
        break;
    case sf::Keyboard::LSystem :

        ret="LSystem";
        break;
    case sf::Keyboard::RControl :

        ret="RControl";
        break;
    case sf::Keyboard::RShift :

        ret="RShift";
        break;
    case sf::Keyboard::RAlt :

        ret="RAlt";
        break;
    case sf::Keyboard::RSystem :

        ret="RSystem";
        break;
    case sf::Keyboard::Menu :

        ret="Menu";
        break;
    case sf::Keyboard::LBracket :

        ret="LBracket";
        break;
    case sf::Keyboard::RBracket :

        ret="RBracket";
        break;
    case sf::Keyboard::SemiColon :

        ret="SemiColon";
        break;
    case sf::Keyboard::Comma :

        ret="Comma";
        break;
    case sf::Keyboard::Period :

        ret="Period";
        break;
    case sf::Keyboard::Quote :

        ret="Quote";
        break;
    case sf::Keyboard::Slash :

        ret="Slash";
        break;
    case sf::Keyboard::BackSlash :

        ret="BackSlash";
        break;
    case sf::Keyboard::Tilde :

        ret="Tilde";
        break;
    case sf::Keyboard::Equal :

        ret="Equal";
        break;
    case sf::Keyboard::Dash :

        ret="Dash";
        break;
    case sf::Keyboard::Space :

        ret=" ";
        break;
    case sf::Keyboard::Return :

        ret="Return";
        break;
    case sf::Keyboard::BackSpace :

        ret="BackSpace";
        break;
    case sf::Keyboard::Tab :

        ret="Tab";
        break;
    case sf::Keyboard::PageUp :

        ret="PageUp";
        break;
    case sf::Keyboard::PageDown :

        ret="PageDown";
        break;
    case sf::Keyboard::End :

        ret="End";
        break;
    case sf::Keyboard::Home :

        ret="Home";
        break;
    case sf::Keyboard::Insert :

        ret="Insert";
        break;
    case sf::Keyboard::Delete :

        ret="Delete";
        break;
    case sf::Keyboard::Add :

        ret="Add";
        break;
    case sf::Keyboard::Subtract :

        ret="Subtract";
        break;
    case sf::Keyboard::Multiply :

        ret="Multiply";
        break;
    case sf::Keyboard::Divide :

        ret="Divide";
        break;
    case sf::Keyboard::Left :

        ret="Left";
        break;
    case sf::Keyboard::Right :

        ret="Right";
        break;
    case sf::Keyboard::Up :

        ret="Up";
        break;
    case sf::Keyboard::Down :

        ret="Down";
        break;
    case sf::Keyboard::Numpad0 :

        ret="Numpad0";
        break;
    case sf::Keyboard::Numpad1 :

        ret="Numpad1";
        break;
    case sf::Keyboard::Numpad2 :

        ret="Numpad2";
        break;
    case sf::Keyboard::Numpad3 :

        ret="Numpad3";
        break;
    case sf::Keyboard::Numpad4 :

        ret="Numpad4";
        break;
    case sf::Keyboard::Numpad5 :

        ret="Numpad5";
        break;
    case sf::Keyboard::Numpad6 :

        ret="Numpad6";
        break;
    case sf::Keyboard::Numpad7 :

        ret="Numpad7";
        break;
    case sf::Keyboard::Numpad8 :

        ret="Numpad8";
        break;
    case sf::Keyboard::Numpad9 :

        ret="Numpad9";
        break;
    case sf::Keyboard::F1 :

        ret="F1";
        break;
    case sf::Keyboard::F2 :

        ret="F2";
        break;
    case sf::Keyboard::F3 :

        ret="F3";
        break;
    case sf::Keyboard::F4 :

        ret="F4";
        break;
    case sf::Keyboard::F5 :

        ret="F5";
        break;
    case sf::Keyboard::F6 :

        ret="F6";
        break;
    case sf::Keyboard::F7 :

        ret="F7";
        break;
    case sf::Keyboard::F8 :

        ret="F8";
        break;
    case sf::Keyboard::F9 :

        ret="F9";
        break;
    case sf::Keyboard::F10 :

        ret="F10";
        break;
    case sf::Keyboard::F11 :

        ret="F11";
        break;
    case sf::Keyboard::F12 :

        ret="F12";
        break;
    case sf::Keyboard::F13 :

        ret="F13";
        break;
    case sf::Keyboard::F14 :

        ret="F14";
        break;
    case sf::Keyboard::F15 :

        ret="F15";
        break;
    case sf::Keyboard::Pause :

        ret="Pause";
        break;
    case sf::Keyboard::KeyCount :

        ret="KeyCount";
        break;


    default:
        ret="Unknown";
        break;
        }
    return ret;






}

TextTools::TextTools()
{
}

TextTools::~TextTools()
{
}
