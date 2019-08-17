#include "Slider.hpp"
#include "PCH.hpp"

namespace
{
	std::vector<float> smallPointPositions(int amountStates, float sliderLength)
	{
		std::vector<float> positions;
		float base = SimulationConstants::dials_SliderRoundedRectangleRadius;
		for (int i = 0; i < amountStates; i++)
		{
			float pos = base + i / (float)(amountStates - 1.f) * (float)(sliderLength - 2.f * base);
			positions.push_back(pos);
		}

		return positions;
	}
}

void Slider::draw()
{
	if (isAnimatingSelection)
		updateSelection();

	globals::mainWindow.inst.draw(roundedRectangle);

	for (int i = 0; i < stateAmount; i++)
	{
		globals::mainWindow.inst.draw(circleShapes[i]);
	}

	globals::mainWindow.inst.draw(selectionCircleShape);
}

void Slider::updateSelection()
{
	auto [ Xpos, animationIsComplete ] = smootherStepAnimation.getAnimationState();

	auto pos = selectionCircleShape.getPosition();
	pos.x = Xpos;

	selectionCircleShape.setPosition(pos);

	if (animationIsComplete)
	{
		isAnimatingSelection = false;
		selectedState = stateToBeSelected;
	}
}

int Slider::getState()
{
	return selectedState;
}

void Slider::setState(int state)
{
	if (isAnimatingSelection) // Dont allow changing of state when animating
		return;

	if (state < 0 || state >= stateAmount) // Return if outside of allowed states
		return;

	isAnimatingSelection = true;
	stateToBeSelected = state;

	auto smallPointXValues = smallPointPositions(stateAmount, sliderLength);
	smootherStepAnimation = Smootherstep(SimulationConstants::dials_SliderSelectionAnimationDuration / 1000.f, position.x + smallPointXValues[selectedState], position.x + smallPointXValues[stateToBeSelected]);
}

Slider::Slider(int amountStates, float length, sf::Vector2f pos, stringvector enlistedScenes, bool isActive) : BaseClickable(enlistedScenes, isActive)
{
	auto roundedRectangleRadius = SimulationConstants::dials_SliderRoundedRectangleRadius;
	auto size = sf::Vector2f(length, roundedRectangleRadius * 2);

	roundedRectangle = RoundedRectangleShape(size, roundedRectangleRadius, SimulationConstants::dials_SliderRoundedRectanglePointCount);

	roundedRectangle.setFillColor(sf::Color::Black);
	roundedRectangle.setOutlineColor(SimulationConstants().dials_SliderRoundedRectangleOutlineColor);
	roundedRectangle.setOutlineThickness(SimulationConstants::dials_SliderRoundedRectangleOutlineThickness);
	roundedRectangle.setPosition(pos);

	auto smallPointXValues = smallPointPositions(amountStates, size.x);

	circleShapes.clear();
	for (int i = 0; i < amountStates; i++)
	{
		auto radius = SimulationConstants::dials_SliderSmallRadius;
		auto circleShape = sf::CircleShape(radius, SimulationConstants::dials_SliderSmallPointCount);
		circleShape.setFillColor(SimulationConstants().dials_SliderSmallFillColor);
		circleShape.setOrigin(sf::Vector2f(radius, radius));
		circleShape.setPosition(sf::Vector2f(pos.x + smallPointXValues[i], pos.y + roundedRectangleRadius));
		circleShapes.push_back(circleShape);

		stateAmount++;
	}

	auto selectionCircleRadius = SimulationConstants::dials_SliderSelectionRadius;
	selectionCircleShape = sf::CircleShape(selectionCircleRadius, SimulationConstants::dials_SliderSelectionPointCount);
	selectionCircleShape.setFillColor(SimulationConstants().dials_SliderSelectionFillColor);
	selectionCircleShape.setOutlineColor(SimulationConstants().dials_SliderSelectionOutlineColor);
	selectionCircleShape.setOutlineThickness(SimulationConstants::dials_SliderSelectionOutlineThickness);
	selectionCircleShape.setOrigin(sf::Vector2f(selectionCircleRadius, selectionCircleRadius));
	selectionCircleShape.setPosition(sf::Vector2f(pos.x + smallPointXValues[selectedState], pos.y + roundedRectangleRadius));

	position = pos;
	sliderLength = length;

}

Slider::Slider() : BaseClickable(stringvector{"default"}, true)
{

}

Slider::~Slider()
{
}