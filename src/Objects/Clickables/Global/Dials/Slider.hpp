#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "../../BaseClickable.h"
#include "../../../../Animations/Smootherstep.hpp"
#include "../../../Drawables/Global/Shapes/RoundedRectangleShape.hpp"

class Slider : public BaseClickable
{
private:
	Smootherstep smootherStepAnimation;
	bool isAnimatingSelection = false;

	RoundedRectangleShape roundedRectangle;
	sf::CircleShape selectionCircleShape;
	std::vector<sf::CircleShape> circleShapes;

	int stateAmount = 0;
	int selectedState = 0;
	int stateToBeSelected = 0;
	float sliderLength;
	sf::Vector2f position;

public:
	bool isActive;
	stringvector enlistedScenes;

	void onHover();
	void onClick();

	void draw();
	void updateSelection();
	int getState();
	void setState(int state);

	Slider(int amountStates, float length, sf::Vector2f pos, stringvector enlistedScenes = stringvector{"default"}, bool isActive = true);
	Slider();
	~Slider();
};

#endif // !SLIDER_HPP