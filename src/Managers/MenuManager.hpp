#ifndef MENUMANAGER_HPP
#define MENUMANAGER_HPP

#include "../Objects/Clickables/Menu/MenuButton.hpp"


class MenuManager
{
private:

public:
	enum ScrollingDirection { left, right };
	enum TransitionDirection { in, out };

	void startIntro();
	void animateIntro();
	void startScrolling(ScrollingDirection direction);
	void animateScrolling();
	void startTransition(TransitionDirection direction);
	void animateTransition();
	void startTransitionInSimulation(TransitionDirection direction);
	void animateTransitionInSimulation();

	void addMainMenuButton(std::string name);
	void addSimulations();
	void resetMenu();

	void update();
	void mouseClicked(sf::Mouse::Button buttonPressed);
	void keyPressed(sf::Keyboard::Key key, bool control, bool alt, bool shift, bool system);

	MenuManager();
	~MenuManager();
};

#endif // !MENUMANAGER_HPP
