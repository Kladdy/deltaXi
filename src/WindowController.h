#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

#include "Managers/ParticleManager.h"

class WindowController
{
	private:
		ParticleManager particleManager;


	public:
		void updateWindow();
		void mouseClicked(sf::Mouse::Button buttonPressed);
		void keyPressed(sf::Keyboard::Key key, bool control, bool alt, bool shift, bool system);
};

#endif