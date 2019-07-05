#include "Toolbox.h"
#include "PCH.hpp"

// Returns the elapsed time since last frame in seconds
float Toolbox::getElapsedTime()
{
	return static_cast<float>(globals::timeSinceUpdate) / 1000000.f;
}