#include "Main.hpp"
#include "WindowController.h"
#include "PCH.hpp"

//Update
sf::Clock updateClock;
sf::Time timeSinceLastUpdate = sf::Time::Zero;
float FPS = 0.f;

void initializeGlobalData();
void initializeSimulations();
void mainWindowLoop(sf::RenderWindow* w);

int main()
{
	initializeSimulations();
	initializeGlobalData();

	return 0;
}

void initializeSimulations()
{
	globals::currentState = globals::state::menu;

	globals::simNames.push_back("Particles");
	globals::simIsInitialized.push_back(false);

}

void initializeGlobalData()
{
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

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (window.hasFocus()) {
					switch (event.type)
					{
					case sf::Event::Closed:
						window.close();
						break;

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