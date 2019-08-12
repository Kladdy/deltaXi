#include "MenuButton.hpp"

bool MenuButton::isHovered()
{
	float dist = VectorTools::distancePointToPoint(this->pos, globals::mousePos);
	if (Toolbox::isHoverable() && isActive && !isHeld && dist <= this->buttonRadius)
	{
		this->onHover();
		return true;
	}
	else if ((isActive && dist >= this->buttonRadius) || !Toolbox::isHoverable())
	{
		this->onDeHover();
		return false;
	}

	return false;
}

bool MenuButton::isClicked()
{
	Logger::log("isClicked!");
	return false;
}

void MenuButton::onHover()
{
	this->isHeld = true;
	this->circleShape.setOutlineColor(this->holdColor);
}

void MenuButton::onDeHover()
{
	this->isHeld = false;
	this->circleShape.setOutlineColor(this->normalColor);
}

void MenuButton::onClick()
{
	Logger::log("click!");
}

void MenuButton::draw()
{
	if(this->isActive)
	{
		if (this->isHeld) // Only draw label if the button is held
			globals::mainWindow.inst.draw(this->label);

		globals::mainWindow.inst.draw(this->circleShape);
	}
}

sf::Vector2f MenuButton::getPosition()
{
	return this->pos;
}

void MenuButton::setPosition(sf::Vector2f pos)
{
	this->pos = pos;
	this->circleShape.setPosition(pos);

	pos.y += this->buttonRadius * 1.2f; // Change text position to specific fraction under
	this->label.setPosition(pos);

}

void MenuButton::setOutlineColor(sf::Color color)
{
	this->circleShape.setOutlineColor(color);
}

void MenuButton::setFillColor(sf::Color color)
{
	this->circleShape.setFillColor(color);
}

void MenuButton::setRadius(float radius)
{
	this->buttonRadius = radius;
	this->circleShape.setRadius(radius);
	this->circleShape.setOrigin(sf::Vector2f(radius, radius));
	this->circleShape.setOutlineThickness(this->outlineThicknessFraction * radius);
}

MenuButton::MenuButton(std::wstring name, float radius, int pointCount, sf::Color fillColor, sf::Color outlineColor, float outlineThickness, sf::Color holdColor, int menuButtonIndex, stringvector enlistedScenes, bool isActive) : RoundClickable(enlistedScenes, isActive)
{
	this->isActive = isActive;
	this->enlistedScenes = enlistedScenes;

	this->name = name;
	this->pos = sf::Vector2f(-globals::windowSize.x / 2, globals::windowSize.y / 2);

	sf::CircleShape circleShape(radius, pointCount);
	circleShape.setOrigin(sf::Vector2f(radius, radius));
	circleShape.setFillColor(fillColor);
	circleShape.setOutlineColor(outlineColor);
	circleShape.setOutlineThickness(outlineThickness * radius);
	circleShape.setPosition(this->pos);
	this->circleShape = circleShape;

	this->buttonRadius = (outlineThickness + 1) * radius;
	this->outlineThicknessFraction = outlineThickness;
	this->soundPlayed = globals::developerMode; // If in developer mode, dont play translation sound (too noisy)
	this->isHeld = false;

	this->normalColor = outlineColor;
	this->holdColor = holdColor;

	this->index = menuButtonIndex;

	this->label = TextTools::createTextLabel(name, this->pos, globals::menuButtonTextSize, globals::defaultTextFont, true);
}

MenuButton::MenuButton() : RoundClickable(stringvector{"default"}, true)
{

}

MenuButton::~MenuButton()
{
}