#include "Toolbox.h"
#include "PCH.hpp"

// Returns true if window is in focus and mouse is contained to the window
bool Toolbox::isHoverable()
{
	sf::Vector2 mP = globals::mousePos;
	bool mouseIsContained = ((mP.x > 0 && mP.x < globals::windowSize.x) && (mP.y > 0 && mP.y < globals::windowSize.y));
	return (globals::windowFocused && mouseIsContained);
}

// Returns the elapsed time since last frame in seconds
float Toolbox::getElapsedTime()
{
	return static_cast<float>(globals::timeSinceUpdate) / 1000000.f;
}