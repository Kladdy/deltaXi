#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

#include "Managers/ParticleManager.h"
#include "Managers/MenuManager.hpp"

class WindowController
{
	private:
		MenuManager menuManager;
		ParticleManager particleManager;

	public:
		void updateWindow();
		void mouseClicked(sf::Mouse::Button buttonPressed);
		void keyPressed(sf::Keyboard::Key key, bool control, bool alt, bool shift, bool system);
		static void close();
};

#endif