#include "Smootherstep.hpp"

namespace
{
	float clamp(float x, float lowerlimit, float upperlimit) {
		if (x < lowerlimit)
			x = lowerlimit;
		else if (x > upperlimit)
			x = upperlimit;
		return x;
	}

	float smootherStep(float edge0, float edge1, float x) { // Gives a smooth value between 0 and 1
		// Scale, and clamp x to 0..1 range
		x = clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
		// Evaluate polynomial
		return x * x * x * (x * (x * 6 - 15) + 10);
	}
}

void Smootherstep::retartAnimation(bool forward )
{
	this->forward = forward;
	this->animationClock.restart();
}

std::tuple<float, bool> Smootherstep::getAnimationState()
{
	float elapsedTime = this->animationClock.getElapsedTime().asSeconds();

	float stateZeroToOne = smootherStep(0.f, this->animationDuration, elapsedTime);
	float stateScaled = (this->upperValue - this->lowerValue) * stateZeroToOne + this->lowerValue;

	return std::make_tuple(stateScaled, (elapsedTime >= this->animationDuration)); // Set tuple object 2 to true if animation is done
}

Smootherstep::Smootherstep(float animationDuration, float lowerValue, float upperValue)
{
	this->animationDuration = animationDuration;
	this->lowerValue = lowerValue;
	this->upperValue = upperValue;
}

Smootherstep::Smootherstep()
{

}

Smootherstep::~Smootherstep()
{
}
