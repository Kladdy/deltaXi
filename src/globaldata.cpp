// File: globaldatacpp
#include "globaldata.h"

namespace globals
{
	// General
	bool developerMode;
	ColorPalette colorPalette;

	// i18n - localization (language)
	std::string currentLocale;
	TranslationController translation;

	// Text
    int defaultTextSize;
	int minTextSize;
	int menuButtonTextSize;
	std::string defaultTextFont;

	// Window
	std::string windowTitle;
	vec2i windowSize;
	int updateRate;
	WindowStruct mainWindow;
	vec2i mousePos;
	int timeSinceUpdate;
	bool windowFocused;
	bool transitioningInSimulation;

	// Globals textures
	std::map<std::string, sf::Texture> loadedTextures; // Textures

	// Globals fonts
	std::map<std::string, sf::Font> loadedFonts; // Fonts

	// Simulations
	state currentState;

    stringvector simNames;
    std::vector<bool> simIsInitialized;
}