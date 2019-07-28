#include "ColorPalette.hpp"

namespace
{
	std::map<std::string, std::vector<sf::Color>> colorPalettes;
}

sf::Color ColorPalette::getRandomColorInPalette(std::string palette)
{
	std::vector<sf::Color> colorPalette = colorPalettes[palette];
	int randomColorIndex = rand() % colorPalette.size();
	return colorPalette[randomColorIndex];
}

std::tuple<sf::Color, int> ColorPalette::getRandomColorInPaletteExcludePrior(std::string palette, int priorIndex)
{
	std::vector<sf::Color> colorPalette = colorPalettes[palette];
	int randomColorIndex = rand() % colorPalette.size();

	if (colorPalette.size() > 1) // Exclude prior color if color palette contains more than 1 color
	{
		while (randomColorIndex == priorIndex)
		{
			randomColorIndex = rand() % colorPalette.size();
		}
	}

	return std::make_tuple(colorPalette[randomColorIndex], randomColorIndex);
}

void ColorPalette::addColorPalette(std::string label, std::vector<sf::Color> colorVector)
{
	colorPalettes[label] = colorVector;
}

ColorPalette::ColorPalette()
{
}

ColorPalette::~ColorPalette()
{
}
