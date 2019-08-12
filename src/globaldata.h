// File: globaldata.h
#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include "./Utility/ColorPalette.hpp"
#include "./Utility/i18n/TranslationController.hpp"

namespace globals
{
    // General
	extern bool developerMode;
    extern ColorPalette colorPalette;

    // i18n - localization (language)
	extern std::string currentLocale;
    extern TranslationController translation;

    // Text
    extern int defaultTextSize;
    extern int minTextSize;
    extern int menuButtonTextSize;
    extern std::string defaultTextFont;

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
    extern bool windowFocused;
    extern bool transitioningInSimulation;

    // Globals textures
	extern std::map<std::string, sf::Texture> loadedTextures; // Textures

    // Globals fonts
	extern std::map<std::string, sf::Font> loadedFonts; // Fonts

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