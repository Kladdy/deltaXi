#include "PCH.hpp"
#include "WindowController.h"

void WindowController::updateWindow()
{
	switch (globals::currentState)
		{
		case globals::state::menu:

			break;

		case globals::state::particle:
			particleManager.updateWindow();
			break;

		default:
			break;
		}

	return;
}

void WindowController::mouseClicked(sf::Mouse::Button buttonPressed)
{
	if (buttonPressed == sf::Mouse::Left)
	{
		switch (globals::currentState)
		{
		case globals::state::menu:
			globals::currentState = globals::state::particle;
			Logger::logExtra("State set to particles");
			break;

		case globals::state::particle:
			particleManager.mouseClicked(buttonPressed);
			break;

		default:
			break;
		}
	}

	return;
}

void WindowController::keyPressed(sf::Keyboard::Key key, bool control, bool alt, bool shift, bool system)
{
	if (control == true && alt == true && shift == true && system == true)
		Logger::log("Keyboard error (you are a maniac!)");

	if (key == sf::Keyboard::Escape && globals::currentState > -1)
	{
		{
			globals::currentState = globals::state::menu;
			Logger::logExtra("State set to menu");
			return;
		}
	}

	switch (globals::currentState)
		{
		case globals::state::menu:

			break;

		case globals::state::particle:
			particleManager.keyPressed(key, control, alt, shift, system);
			break;

		default:
			break;
		}


	return;
}