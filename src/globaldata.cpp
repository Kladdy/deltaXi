// File: globaldatacpp
#include "globaldata.h"

namespace globals
{
	// Window
	std::string windowTitle;
	vec2i windowSize;
	int updateRate;
	WindowStruct mainWindow;
	vec2i mousePos;
	int timeSinceUpdate;

	// Simulations
	state currentState;

    stringvector simNames;
    std::vector<bool> simIsInitialized;
}