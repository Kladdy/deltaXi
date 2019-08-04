#include "Main.hpp"
#include "WindowController.h"
#include "PCH.hpp"
#include "../content/icon/icon256.c"
#include "../content/fonts/RomanSerif_ttf.hpp"

namespace
{
	// Timer clock
	sf::Clock timerClock;

	void setTimer()
	{
		timerClock.restart();
	}

	void getTimer(std::string eventJustFinished)
	{
		std::string t = std::to_string(timerClock.restart().asMicroseconds());
		Logger::log(eventJustFinished + " finished in " + t + " us");
	}
}

//Update
sf::Clock updateClock;
sf::Time timeSinceLastUpdate = sf::Time::Zero;
float FPS = 0.f;

void initializeGlobalData();
void initializeSimulations();
void mainWindowLoop(sf::RenderWindow* w);

int main()
{
	(void)setTimer; // Silence unused function warning
	(void)getTimer;

	initializeSimulations();
	initializeGlobalData();

	return 0;
}

void colorPaletteSetup()
{
	globals::colorPalette = ColorPalette();

	globals::colorPalette.addColorPalette("RGB", std::vector<sf::Color>{sf::Color::Red, sf::Color::Green, sf::Color::Blue});
	globals::colorPalette.addColorPalette("particles1", std::vector<sf::Color>{sf::Color(70, 188, 222), sf::Color(82, 210, 115), sf::Color(233, 79, 100), sf::Color(229, 114, 84), sf::Color(229, 196, 84)});

}

void loadFonts()
{
	globals::loadedFonts["RomanSerif"] = ResourceController::loadFontFromMemory((void*)RomanSerif_ttf, RomanSerif_ttf_length);
}

void initializeSimulations()
{
	srand (time(NULL));
	globals::currentState = globals::state::menu;

	globals::simNames.push_back("Particles");
	globals::simIsInitialized.push_back(false);

	globals::transitioningInSimulation = false;

}

void initializeGlobalData()
{
	// Setup color palettes
	colorPaletteSetup();

	// Load fonts
	loadFonts();

	// General
	globals::developerMode = false;

	// Window
	globals::windowTitle = "deltaXi";
	globals::windowSize = sf::Vector2i(1400, 800);
	globals::updateRate = 60;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	globals::mainWindow.inst.create(sf::VideoMode(globals::windowSize.x, globals::windowSize.y), globals::windowTitle, sf::Style::Close | sf::Style::Titlebar, settings);
	#ifdef SFML_SYSTEM_WINDOWS
		__windowsHelper.setIcon(globals::mainWindow.inst.getSystemHandle());
	#endif

	globals::mainWindow.inst.setIcon( Icon256.width,  Icon256.height,  Icon256.pixel_data );

	globals::mainWindow.loop = std::bind(mainWindowLoop, &globals::mainWindow.inst);
	globals::mainWindow.loop();

	return;
}

void mainWindowLoop(sf::RenderWindow* w)
{

	sf::RenderWindow& window = *w;
	WindowController windowController;
	int elapsedTime;

	while (window.isOpen())
	{
		timeSinceLastUpdate += updateClock.restart();
		elapsedTime = timeSinceLastUpdate.asMicroseconds();

		if (elapsedTime >= (1000000 / globals::updateRate)) {
			globals::timeSinceUpdate = elapsedTime;
			globals::mousePos = sf::Mouse::getPosition(window);
			globals::windowFocused = window.hasFocus();

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) WindowController::close();

				if (globals::windowFocused) {
					switch (event.type)
					{
					case sf::Event::MouseButtonPressed:
						windowController.mouseClicked(event.mouseButton.button);
						break;

					case sf::Event::KeyPressed:
						windowController.keyPressed(event.key.code, event.key.control, event.key.alt, event.key.shift, event.key.system);
						break;

					default:
						break;
					}
				}
			}

			window.clear(sf::Color::Black);

			windowController.updateWindow();

			window.display();

			timeSinceLastUpdate = sf::Time::Zero;
		}

		// Sleep for 0.4 millisecond to prevent program from updating to often
		sf::sleep(sf::microseconds(400));
	}

	return;
}