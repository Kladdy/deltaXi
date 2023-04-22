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
	sf::Vector2f knobPosition;
	bool knobIsHeld;
	bool isHeld;

public:
	bool isActive;
	stringvector enlistedScenes;

	bool isHovered();
	bool knobIsHovered();
	void onKnobHover();
	void onKnobDeHover();
	void onHover();
	void onDeHover();
	void onClick();

	void draw();
	void updateSelection();
	int getState();
	int getStateToBeSelected();
	void setState(int state);
	void setStateFromXValue(float x);

	Slider(int amountStates, float length, sf::Vector2f pos, int initialState = 0, stringvector enlistedScenes = stringvector{"default"}, bool isActive = true);
	Slider();
	~Slider();
};

#endif // !SLIDER_HPP