#ifndef SMOOTHERSTEP_HPP
#define SMOOTHERSTEP_HPP

#include "PCH.hpp"

class Smootherstep
{
private:
	sf::Clock animationClock;
	float animationDuration;
	float lowerValue;
	float upperValue;
	bool forward; // Indicates direction of animation (true if forwards, false if backwards)

public:
	void retartAnimation(bool forward = true);
	std::tuple<float, bool> getAnimationState();
	float getElapsedTime();

	Smootherstep(float animationDuration, float lowerValue = 0.f, float upperValue = 1.f);
	Smootherstep();
	~Smootherstep();
};

#endif // !SMOOTHERSTEP_HPP
