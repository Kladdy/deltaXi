#ifndef MENUBUTTON_HPP
#define MENUBUTTON_HPP

#include "../RoundClickable.h"
#include "PCH.hpp"

class MenuButton : public RoundClickable
{
	private:
		sf::Vector2f pos;
		std::string name;
		sf::CircleShape circleShape;
	public:
		int index;
		float buttonRadius;
		float outlineThicknessFraction;
		bool soundPlayed;
		bool isHeld;
		sf::Color normalColor;
		sf::Color holdColor;

		bool isHovered();
		bool isClicked();
		void onHover();
		void onDeHover();
		void onClick();
		void draw();

		sf::Vector2f getPosition();
		void setPosition(sf::Vector2f pos);
		void setOutlineColor(sf::Color color);
		void setFillColor(sf::Color color);
		void setRadius(float radius);

		MenuButton(std::string name, float radius, int pointCount, sf::Color fillColor, sf::Color outlineColor, float outlineThickness, sf::Color holdColor, int menuButtonIndex, stringvector enlistedScenes = stringvector{"default"}, bool isActive = true);
		MenuButton();
		~MenuButton();
};

#endif // !MENUBUTTON_HPP