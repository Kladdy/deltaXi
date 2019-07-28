#ifndef COLORPALETTE_HPP
#define COLORPALETTE_HPP

#include "PCH.hpp"

class ColorPalette
{
private:

public:
	sf::Color getRandomColorInPalette(std::string palette);
	std::tuple<sf::Color, int> getRandomColorInPaletteExcludePrior(std::string palette, int priorIndex);
	void addColorPalette(std::string label, std::vector<sf::Color> colorVector);

	ColorPalette();
	~ColorPalette();
};

#endif // !COLORPALETTE_HPP