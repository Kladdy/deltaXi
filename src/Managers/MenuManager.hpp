#ifndef MENUMANAGER_HPP
#define MENUMANAGER_HPP

#include "../Objects/Clickables/Menu/MenuButton.hpp"


class MenuManager
{
private:

public:
	void startIntro();
	void animateIntro();
	void startScrolling(std::string direction);
	void animateScrolling();
	void startTransition();
	void animateTransition();

	void addMainMenuButton(std::string name);
	void addSimulations();

	void update();
	void mouseClicked(sf::Mouse::Button buttonPressed);
	void keyPressed(sf::Keyboard::Key key, bool control, bool alt, bool shift, bool system);

	MenuManager();
	~MenuManager();
};

#endif // !MENUMANAGER_HPP
