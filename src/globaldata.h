// File: globaldata.h
#ifndef GLOBALDATA_H
#define GLOBALDATA_H

namespace globals
{
    // Window
    struct WindowStruct {
	    sf::RenderWindow inst;
	    std::function<void()> loop;
    };
    extern std::string windowTitle;
    extern vec2i windowSize;
    extern int updateRate;
    extern WindowStruct mainWindow;
    extern vec2i mousePos;
    extern int timeSinceUpdate;

    // Simulations
    enum state
	{
        settings = -2,
		menu = -1,
		particle = 0,
	};
    extern state currentState;

    extern stringvector simNames;
    extern std::vector<bool> simIsInitialized;
}

#endif