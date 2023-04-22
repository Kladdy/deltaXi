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

	template<typename Q, typename I, typename Distance>
	void find_n_nearest(const Q& q, I first, I nth, I last, Distance dist)
	{
		using T = decltype(*first);
		auto compare = [&q, &dist] (T i, T j) { return dist(i, q) < dist(j, q); };
		std::nth_element(first, nth, last, compare);
		std::sort(first, last, compare);
	}

	template < typename T>
	std::pair<bool, int > findInVector(const std::vector<T>  & vecOfElements, const T  & element)
	{
		std::pair<bool, int > result;

		// Find given element in vector
		auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

		if (it != vecOfElements.end())
		{
			result.second = distance(vecOfElements.begin(), it);
			result.first = true;
		}
		else
		{
			result.first = false;
			result.second = -1;
		}

		return result;
	}

	float clamp(float x, float lowerlimit, float upperlimit) {
		if (x < lowerlimit)
			x = lowerlimit;
		else if (x > upperlimit)
			x = upperlimit;
		return x;
	}

	bool isCointainedInRectangle(sf::Vector2f pos, sf::Vector2f topLeft, sf::Vector2f bottomRight)
	{
		bool isBoundedInX = (pos.x > topLeft.x && pos.x < bottomRight.x);
		bool isBoundedInY = (pos.y > topLeft.y && pos.y < bottomRight.y);

		return (isBoundedInX && isBoundedInY);
	}
}

bool Slider::isHovered()
{
	auto topLeft = this->position;
	auto bottomRight = topLeft;
	bottomRight.x += roundedRectangle.getLocalBounds().width;
	bottomRight.y += roundedRectangle.getLocalBounds().height;

	auto mouseIsBounded = isCointainedInRectangle(VectorTools::vec2i_to_vec2f(globals::mousePos), topLeft, bottomRight);

	if (Toolbox::isHoverable() && isActive && !isHeld && mouseIsBounded)
	{
		this->onHover();
		return true;
	}
	else if (Toolbox::isHoverable() && isActive && isHeld && mouseIsBounded)
	{

		return true;
	}
	else if ((isActive && !mouseIsBounded) || !Toolbox::isHoverable())
	{
		this->onDeHover();
		return false;
	}

	return false;
}
/*
bool Slider::knobIsHovered()
{
	float radius = SimulationConstants::dials_SliderSelectionRadius;
	float dist = VectorTools::distancePointToPoint(knobPosition, globals::mousePos);

	if (Toolbox::isHoverable() && this->isActive && !this->knobIsHeld && dist <= radius)
	{
		this->onKnobHover();
		return true;
	}
	else if (Toolbox::isHoverable() && this->isActive && this->knobIsHeld && dist <= radius)
	{
		//setStateFromXValue();
		return true;
	}
	else if ((this->isActive && dist >= radius) || !Toolbox::isHoverable())
	{
		this->onKnobDeHover();
		return false;
	}

	return false;
}

void Slider::onKnobHover()
{
	selectionCircleShape.setOutlineColor(SimulationConstants().dials_SliderKnobHoldColor);
	this->knobIsHeld = true;
}

void Slider::onKnobDeHover()
{
	selectionCircleShape.setOutlineColor(sf::Color::White);
	this->knobIsHeld = false;
}
*/
void Slider::onHover()
{
	this->isHeld = true;
}

void Slider::onDeHover()
{
	this->isHeld = false;
}


void Slider::onClick()
{
	setStateFromXValue(globals::mousePos.x);
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

	this->knobPosition = pos;
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

int Slider::getStateToBeSelected()
{
	return stateToBeSelected;
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

void Slider::setStateFromXValue(float x)
{
	auto roundRectangleRadius =  SimulationConstants::dials_SliderRoundedRectangleRadius;
	x = clamp(x, position.x + roundRectangleRadius, position.x + sliderLength - roundRectangleRadius) - position.x; // Clamp and "normalize"

	auto smallPointXValues = smallPointPositions(stateAmount, sliderLength);

	auto distance = [] (float i, float j) { return std::abs(i-j); };

    find_n_nearest(x, smallPointXValues.begin(), smallPointXValues.begin() + smallPointXValues.size(), smallPointXValues.end(), distance);
	float nearest = smallPointXValues[0];

	// Redo the calculations in order to get the correct arrangement
	smallPointXValues = smallPointPositions(stateAmount, sliderLength);

	auto state = findInVector<float>(smallPointXValues, nearest);

	this->setState(state.second);
}

Slider::Slider(int amountStates, float length, sf::Vector2f pos, int initialState, stringvector enlistedScenes, bool isActive) : BaseClickable(enlistedScenes, isActive)
{
	auto roundedRectangleRadius = SimulationConstants::dials_SliderRoundedRectangleRadius;
	auto size = sf::Vector2f(length, roundedRectangleRadius * 2);
	selectedState = initialState;

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
	knobPosition = sf::Vector2f(pos.x + smallPointXValues[selectedState], pos.y + roundedRectangleRadius);
	selectionCircleShape.setPosition(knobPosition);

	position = pos;
	sliderLength = length;
	this->knobIsHeld = false;
	this->isActive = true;
	this->isHeld = true;
}

Slider::Slider() : BaseClickable(stringvector{"default"}, true)
{

}

Slider::~Slider()
{
}