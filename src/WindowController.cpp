#include "PCH.hpp"
#include "WindowController.h"

void WindowController::updateWindow()
{
	switch (globals::currentState)
		{
		case globals::state::menu:
			menuManager.update();
			break;

		case globals::state::particle:
			particleManager.update();
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
			menuManager.mouseClicked(buttonPressed);
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

	if (key == sf::Keyboard::Escape && globals::currentState == -1)
	{
		WindowController::close();
		return;
	}

	switch (globals::currentState)
		{
		case globals::state::menu:
			menuManager.keyPressed(key, control, alt, shift, system);
			break;

		case globals::state::particle:
			particleManager.keyPressed(key, control, alt, shift, system);
			break;

		default:
			break;
		}


	return;
}

void WindowController::close()
{
	//globals::loadedSoundBuffers.clear();

	Logger::logExtra("QUIT");
	globals::mainWindow.inst.close();
}