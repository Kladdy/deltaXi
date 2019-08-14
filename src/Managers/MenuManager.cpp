#include "MenuManager.hpp"
#include "PCH.hpp"
#include "../Objects/Clickables/Menu/MenuButton.hpp"
#include "../Animations/MenuButtonIcons/ParticleIconAnimation.hpp"
//#include "../../content/sound/menuFade.hpp"
#include "../../content/sound/menuSounds.hpp"

namespace
{
	// Window
	sf::Vector2i windowSize;
	sf::Vector2f middlePoint; // Middle point of window
	int selectedSimulation = 0; // The currently selected animation

	// Menu button
	int amountMenuButtons = 0;
	int menuButtonPointCount = 100;
	float menuButtonSize = 1.f / 6.f; // Radius of menu buttons expressed as fraction of window height
	float menuButtonRadius; // Initialized in constructor
	float menuButtonOutlineThickness = 0.03f; // Thickness of menu buttons expressed as fraction of radius
	sf::Color menuButtonFillColor = sf::Color::Black;
	sf::Color menuButtonOutlineColor = sf::Color::White;
	sf::Color menuButtonHoldColor = sf::Color(0, 72, 123);
	std::map<std::string, MenuButton> menuButtons; // MenuButtons object

	// Menu button animations
	ParticleIconAnimation particleIconAnimation;

	// Map prefixes
	std::string prefix = "menu_"; // Specifies prefix to use for global resources, such as textures, sounds etc.
	std::string menuButtonPrefix = "button_"; // Specifies the prefix for menubuttons when mapping resources
	std::string menuButtonSoundPrefix = "menuTraverse"; // Specifies prefix for menu button traversing sound

	// Resources
	std::map<std::string, sf::SoundBuffer> loadedSoundBuffers; // Soundbuffers
	std::map<std::string, sf::Sound> storedSounds; // Sounds
	std::map<std::string, sf::Sprite> storedSprites; // Sprites
    std::map<std::string, sf::CircleShape> storedCircleShapes; // CircleShapes
	std::map<std::string, sf::RectangleShape> storedRectangleShapes; // RectangleShapes
    std::map<std::string, sf::Text> storedTexts; // Texts

	// Animations
	sf::Clock animationClock;
	// Intro: Button fading using Smooth(er)step by Ken Perlin (https://en.wikipedia.org/wiki/Smoothstep)
	bool isAnimatingIntro = false;
	bool isAnimatingFadeIn = false;
	float introAnimationDuration = 6; // Intro duration (in seconds)
	float introAnimationFadeinDuration = 5;
	// Flying - Initialized in constructor
	float lowXLimit;
	float highXLimit;
	// Fading
	float lowFadeLimit = 0.f;
	float highFadeLimit = 255.f;
	// Scrolling animation
	bool isAnimatingScrolling = false;
	float scrollingAnimationDuration = 0.3f; // Scrolling duration (in seconds)
	MenuManager::ScrollingDirection scrollingDirection;
	// Scrolling - Initialized in constructor
	float lowXRightLimit;
	float highXRightLimit;
	float lowXLeftLimit;
	float highXLeftLimit;
	// Transition animation
	bool isAnimatingTransition = false;
	float transitionAnimationDuration = 1.5f; // Transition duration (in seconds)
	float delayUntilTransition = 0.5f;
	MenuManager::TransitionDirection transitionDirection;
	float lowRadiusLimit;
	float highRadiusLimit;
	// Transitioning back
	MenuButton transitionMenuButton;

	float clamp(float x, float lowerlimit, float upperlimit) {
		if (x < lowerlimit)
			x = lowerlimit;
		else if (x > upperlimit)
			x = upperlimit;
		return x;
	}

	float smootherStep(float edge0, float edge1, float x) { // Gives a smooth value between 0 and 1
		// Scale, and clamp x to 0..1 range
		x = clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
		// Evaluate polynomial
		return x * x * x * (x * (x * 6 - 15) + 10);
	}

	// Get menu button key of menu button with index 'index'
	std::string menuButtonMapKey(int index)
	{
		return menuButtonPrefix + std::to_string(index);
	}

	std::string menuButtonSoundMapKey(int index)
	{
		return menuButtonSoundPrefix + std::to_string(index);
	}

	// Timer clock
	sf::Clock timerClock;

	void setTimer()
	{
		timerClock.restart();
	}

	void getTimer(std::string eventJustFinished)
	{
		if (!isAnimatingIntro) return;
		std::string t = std::to_string(timerClock.restart().asMicroseconds());
		Logger::log(eventJustFinished + " finished in " + t + " us");
	}

	bool isAnimating()
	{
		return (isAnimatingIntro || isAnimatingScrolling || isAnimatingTransition);
	}
}

void MenuManager::update()
{
	if (isAnimatingIntro) animateIntro();
	else if (isAnimatingScrolling) animateScrolling();
	else if (isAnimatingTransition) animateTransition();

	for (int i = 0; i < amountMenuButtons; i++)
	{
		std::string key = menuButtonMapKey(i);
		if (!isAnimatingIntro && !isAnimatingScrolling && !isAnimatingTransition)
		{
			if (menuButtons[key].isHovered())
				storedSounds["menuButtonHover"].play();
		}

		menuButtons[key].draw();

		switch (i) // Draw menu button animation
		{
			case 0:
				particleIconAnimation.updateAnimation(menuButtons[key].getPosition());
				break;

			default:
				break;
		}
	}

	/* for (auto& [key, value] : storedCircleShapes)
	{
		globals::mainWindow.inst.draw(value);
	}*/
}

void MenuManager::mouseClicked(sf::Mouse::Button buttonPressed)
{
	if (buttonPressed == sf::Mouse::Left)
	{
		for (int i = 0; i < amountMenuButtons; i++)
		{
			std::string key = menuButtonMapKey(i);
			if (!isAnimatingIntro && !isAnimatingScrolling && !isAnimatingTransition && menuButtons[key].isHeld)
			{
				if (menuButtons[key].index == selectedSimulation)
					startTransition(TransitionDirection::in);
				if (menuButtons[key].index > selectedSimulation)
					startScrolling(ScrollingDirection::left);
				if (menuButtons[key].index < selectedSimulation)
					startScrolling(ScrollingDirection::right);

			}

			menuButtons[key].draw();
		}
	}

	return;
}

void MenuManager::keyPressed(sf::Keyboard::Key key, bool control, bool alt, bool shift, bool system)
{
	if (key == sf::Keyboard::Space && amountMenuButtons > 0 && !isAnimating())
	{
		startIntro();
	}

	if (key == sf::Keyboard::Right && selectedSimulation != amountMenuButtons - 1 && !isAnimating())
	{
		startScrolling(ScrollingDirection::left);
	}

	if (key == sf::Keyboard::Left && selectedSimulation != 0 && !isAnimating())
	{
		startScrolling(ScrollingDirection::right);
	}

	if (key == sf::Keyboard::A && control == true && alt == true && shift == true && system == true)
		Logger::log("Keyboard error (you are a maniac!)");

	return;
}

void MenuManager::startIntro()
{
	for (auto& [key, value] : menuButtons)
	{
		value.setPosition(sf::Vector2f(lowXLimit, globals::windowSize.y / 2));
	}

	animationClock.restart();
	isAnimatingIntro = true;
	globals::mainWindow.inst.setMouseCursorVisible(false);
}

void MenuManager::animateIntro()
{
	float elapsedTime = animationClock.getElapsedTime().asSeconds();

	if (!isAnimatingFadeIn) // If we are not animating fade in, we are animating the traversing (first part)
	{
		for(int i = 0; i < amountMenuButtons; i++)
		{
			std::string key = menuButtonMapKey(i);
			float waitPeriod = i * introAnimationDuration / amountMenuButtons / 2;

			if (elapsedTime >= waitPeriod) // Ensures animation will happen at regular intervals
			{
				float traversedFraction = smootherStep(waitPeriod, introAnimationDuration, elapsedTime);
				sf::Vector2f pos = menuButtons[key].getPosition();
				pos.x = (highXLimit - lowXLimit) * traversedFraction + lowXLimit;
				menuButtons[key].setPosition(pos);

				if (!menuButtons[key].soundPlayed && pos.x >= - menuButtonSize * globals::windowSize.y)
				{
					storedSounds[menuButtonSoundMapKey(i)].play();
					menuButtons[key].soundPlayed = true;
				}
			}
		}
	}
	else
	{
		// Fade in the first menu button
		std::string key0 = menuButtonMapKey(0);

		float fadedFraction = smootherStep(0.f, introAnimationFadeinDuration, elapsedTime);
		sf::Color color = menuButtonOutlineColor;
		color.a = (highFadeLimit - lowFadeLimit) * fadedFraction + lowFadeLimit;
		menuButtons[key0].setOutlineColor(color);
		particleIconAnimation.setFade(color.a);

		// Move in the second menu botton from the right
		std::string key1 = menuButtonMapKey(1);

		float traversedFraction = fadedFraction;
		sf::Vector2f pos = menuButtons[key1].getPosition();
		pos.x = (highXRightLimit - lowXRightLimit) * traversedFraction + lowXRightLimit;
		menuButtons[key1].setPosition(pos);
	}

	if(elapsedTime >= introAnimationDuration) // Traversing done - move on to fading in the first button
	{
		isAnimatingFadeIn = true;
		menuButtons[menuButtonMapKey(0)].setPosition(middlePoint);
		menuButtons[menuButtonMapKey(0)].setOutlineColor(sf::Color::Transparent);
		particleIconAnimation.setFade(0);

		storedSounds["menuFade"].play(); // Play manu fade in sound

		animationClock.restart();
		return;
	}
	if(isAnimatingFadeIn && elapsedTime >= introAnimationFadeinDuration) // Intro animation done
	{
		isAnimatingIntro = false;
		isAnimatingFadeIn = false;

		particleIconAnimation.setFade(255);

		for (auto& [key, value]: menuButtons)
		{
			value.soundPlayed = false;
		}

		globals::mainWindow.inst.setMouseCursorVisible(true); // Set cursor visible
	}
}

void MenuManager::startScrolling(ScrollingDirection direction)
{
	animationClock.restart();
	isAnimatingScrolling = true;
	scrollingDirection = direction;
}

void MenuManager::animateScrolling()
{
	int buttonToMoveAway = -1;
	int buttonToMoveOut = -1;
	int buttonToMoveIn = -1;
	int buttonToMoveToward = -1;

	float elapsedTime = animationClock.getElapsedTime().asSeconds();
	float traversedFraction = smootherStep(0.f, scrollingAnimationDuration, elapsedTime);

	int sign = 0;
	if (scrollingDirection == ScrollingDirection::left) sign = +1;
	if (scrollingDirection == ScrollingDirection::right) sign = -1;

	if (selectedSimulation != 0) buttonToMoveAway = selectedSimulation - sign * 1;
	buttonToMoveOut = selectedSimulation;
	buttonToMoveIn = selectedSimulation + sign * 1;
	if (amountMenuButtons >= 3) buttonToMoveToward = selectedSimulation + sign * 2;

	std::string keyAway = menuButtonMapKey(buttonToMoveAway);
	std::string keyOut = menuButtonMapKey(buttonToMoveOut);
	std::string keyIn = menuButtonMapKey(buttonToMoveIn);
	std::string keyToward = menuButtonMapKey(buttonToMoveToward);

	float pointAway = 0.f;
	float pointOut = 0.f;
	float pointIn = 0.f;
	float pointToward = 0.f;
	float pointGoingToward = 0.f;

	if (sign == +1)
	{
		pointAway = lowXLeftLimit;
		pointOut = highXLeftLimit;
		pointIn = middlePoint.x;
		pointToward = highXRightLimit;
		pointGoingToward = lowXRightLimit;
	}
	else if (sign == -1)
	{
		pointAway = lowXRightLimit;
		pointOut = highXRightLimit;
		pointIn = middlePoint.x;
		pointToward = highXLeftLimit;
		pointGoingToward = lowXLeftLimit;
	}

	if (buttonToMoveAway != -1) // Button to move away
	{
		sf::Vector2f posAway = menuButtons[keyAway].getPosition();
		posAway.x = (pointAway - pointOut) * traversedFraction + pointOut;
		menuButtons[keyAway].setPosition(posAway);
	}

	sf::Vector2f posOut = menuButtons[keyOut].getPosition(); // Button to move out
	posOut.x = (pointOut - pointIn) * traversedFraction + pointIn;
	menuButtons[keyOut].setPosition(posOut);

	sf::Vector2f posIn = menuButtons[keyIn].getPosition(); // Button to move in
	posIn.x = (pointIn - pointToward) * traversedFraction + pointToward;
	menuButtons[keyIn].setPosition(posIn);

	if (buttonToMoveToward != -1) // Button to move toward
	{
		sf::Vector2f posToward = menuButtons[keyToward].getPosition();
		posToward.x = (pointToward - pointGoingToward) * traversedFraction + pointGoingToward;
		menuButtons[keyToward].setPosition(posToward);
	}

	if (elapsedTime >= scrollingAnimationDuration)
	{
		selectedSimulation += sign;
		isAnimatingScrolling = false;
	}
}

void MenuManager::startTransition(TransitionDirection direction)
{
	isAnimatingTransition = true;
	animationClock.restart();
	transitionDirection = direction;

}

void MenuManager::animateTransition()
{
	float elapsedTime = animationClock.getElapsedTime().asSeconds();

	int sign = 0;
	if (transitionDirection == TransitionDirection::in) sign = +1;
	if (transitionDirection == TransitionDirection::out) sign = -1;

	if ((sign == +1 && elapsedTime < delayUntilTransition) || (sign == -1 && elapsedTime >= transitionAnimationDuration - delayUntilTransition)) // Move adjacent buttons to the side
	{
		float transitionFraction = 0.f;
		if (sign == +1) transitionFraction = smootherStep(0.f, delayUntilTransition, elapsedTime);
		else if (sign == -1) transitionFraction = smootherStep(transitionAnimationDuration - delayUntilTransition, transitionAnimationDuration, elapsedTime);

		int buttonToMoveLeft = -1;
		int buttonToMoveRight = -1;

		if (selectedSimulation != 0) buttonToMoveLeft = selectedSimulation - 1;
		if (selectedSimulation != amountMenuButtons) buttonToMoveRight = selectedSimulation + 1;

		std::string keyLeft = menuButtonMapKey(buttonToMoveLeft);
		std::string keyRight = menuButtonMapKey(buttonToMoveRight);

		float initialXLeft = 0.f;
		float initialXRight = 0.f;
		float finalXLeft = 0.f;
		float finalXRight = 0.f;

		if (sign == +1)
		{
			initialXLeft = highXLeftLimit;
			initialXRight = highXRightLimit;
			finalXLeft = lowXLeftLimit;
			finalXRight = lowXRightLimit;
		}
		else if (sign == -1)
		{
			initialXLeft = lowXLeftLimit;
			initialXRight = lowXRightLimit;
			finalXLeft = highXLeftLimit;
			finalXRight = highXRightLimit;
		}

		if (buttonToMoveLeft != -1) // Button to move left
		{
			sf::Vector2f posLeft = menuButtons[keyLeft].getPosition();
			posLeft.x = (finalXLeft - initialXLeft) * transitionFraction + initialXLeft;
			menuButtons[keyLeft].setPosition(posLeft);
		}

		if (buttonToMoveRight != -1) // Button to move right
		{
			sf::Vector2f posRight = menuButtons[keyRight].getPosition();
			posRight.x = (finalXRight - initialXRight) * transitionFraction + initialXRight;
			menuButtons[keyRight].setPosition(posRight);
		}
	}
	else
	{
		float zoomFraction = 0.f;
		if (sign == +1) zoomFraction = smootherStep(delayUntilTransition, transitionAnimationDuration, elapsedTime);
		else if (sign == -1) zoomFraction = smootherStep(0.f, transitionAnimationDuration - delayUntilTransition, elapsedTime);

		std::string key = menuButtonMapKey(selectedSimulation);

		float radius = 0;
		if (sign == +1) radius = (highRadiusLimit - lowRadiusLimit) * zoomFraction + lowRadiusLimit;
		else if (sign == -1) radius = (lowRadiusLimit - highRadiusLimit) * zoomFraction + highRadiusLimit;
		menuButtons[key].setRadius(radius);

		switch (selectedSimulation){
			case 0:
				particleIconAnimation.setFade(sign * 255 * (0.5f + sign / 2.f - zoomFraction));
				break;

			default:
				break;
		}
	}


	if (elapsedTime >= transitionAnimationDuration)
	{
		isAnimatingTransition = false;
		if (sign == +1)
		{
			globals::currentState = globals::state(selectedSimulation);
			globals::transitioningInSimulation = true;
			startTransitionInSimulation(TransitionDirection::out);
		}
	}
}

void MenuManager::startTransitionInSimulation(TransitionDirection direction)
{
	transitionMenuButton.setRadius(0);
	transitionMenuButton.setPosition(middlePoint);
	isAnimatingTransition = true;
	animationClock.restart();
	transitionDirection = direction;
}

void MenuManager::animateTransitionInSimulation()
{
	float elapsedTime = animationClock.getElapsedTime().asSeconds();
	float zoomDuration = transitionAnimationDuration - delayUntilTransition;

	int sign = 0;
	if (transitionDirection == TransitionDirection::in) sign = +1;
	if (transitionDirection == TransitionDirection::out) sign = -1;

	float zoomFraction = smootherStep(0.f, zoomDuration, elapsedTime);

	float radius = 0;
	if (sign == +1) radius = (highRadiusLimit - 0.f) * zoomFraction + 0.f;
	else if (sign == -1) radius = (0.f - highRadiusLimit) * zoomFraction + highRadiusLimit;
	transitionMenuButton.setRadius(radius);

	transitionMenuButton.draw();

	if(elapsedTime >= zoomDuration)
	{
		globals::transitioningInSimulation = false;
		if (sign == +1)
		{
			startTransition(TransitionDirection::out);
			globals::currentState = globals::state::menu;
		}
	}
}

void MenuManager::addMainMenuButton(std::wstring name)
{
	std::string key = menuButtonMapKey(amountMenuButtons); // Set current key to the amount of menu buttons (so that particles = 0 etc...)
	std::string key_sounds = menuButtonSoundMapKey(amountMenuButtons);

	menuButtons[key] = MenuButton(name, menuButtonRadius, menuButtonPointCount, menuButtonFillColor, menuButtonOutlineColor, menuButtonOutlineThickness, menuButtonHoldColor, amountMenuButtons);

	loadedSoundBuffers[key_sounds] = ResourceController::loadMenuButtonSoundBufferFromMemory(amountMenuButtons);
	storedSounds[key_sounds] = sf::Sound(loadedSoundBuffers[key_sounds]);

	amountMenuButtons++;
}

void MenuManager::addSimulations()
{
	for (size_t i = 0; i < globals::simNames.size(); i++)
	{
		std::wstring simulationName = TextTools::getTranslation("sim_" + globals::simNames[i]);
		addMainMenuButton(simulationName);
	}
}

void MenuManager::resetMenu()
{
	for (auto& [key, value]: menuButtons)
	{
		value.setRadius((menuButtonOutlineThickness + 1) * menuButtonRadius);
	}
}

MenuManager::MenuManager()
{
	loadedSoundBuffers["menuFade"] = ResourceController::loadSoundBufferFromMemory((void*)menuFade_data, menuFade_data_length);
	storedSounds["menuFade"] = sf::Sound(loadedSoundBuffers["menuFade"]);

	loadedSoundBuffers["menuButtonHover"] = ResourceController::loadSoundBufferFromMemory((void*)mouseOver_data, mouseOver_data_length);
	storedSounds["menuButtonHover"] = sf::Sound(loadedSoundBuffers["menuButtonHover"]);

	windowSize = globals::windowSize;

	menuButtonRadius = menuButtonSize * windowSize.y;
	middlePoint = sf::Vector2f(windowSize.x / 2, windowSize.y / 2);

	lowXLimit = -1.2f * menuButtonRadius;
	highXLimit = windowSize.x + 1.2f * menuButtonRadius;

	lowXLeftLimit = -2.f * menuButtonRadius;
	highXLeftLimit = -2.f * menuButtonRadius / 3.f;
	lowXRightLimit = windowSize.x + 2.f * menuButtonRadius;
	highXRightLimit = windowSize.x + 2.f * menuButtonRadius / 3.f;

	lowRadiusLimit = menuButtonRadius;
	highRadiusLimit = 2.f * windowSize.x / 3.f;

	(void)setTimer; // Silence unused function warning
	(void)getTimer;

	// Add transitioning menu button
	transitionMenuButton = MenuButton(L"transition", menuButtonRadius, menuButtonPointCount, menuButtonFillColor, menuButtonHoldColor, menuButtonOutlineThickness, menuButtonHoldColor, -1);

	if (globals::developerMode) // Developer animation durations to shorter launch
	{
		introAnimationDuration = 0.5;
		introAnimationFadeinDuration = 0;
		transitionAnimationDuration = 0.5;
		delayUntilTransition = 0;
	}

	// Setup menu animations
	particleIconAnimation = ParticleIconAnimation(4); // Setup particle simulation icon with 3 particles

	addSimulations();

	startIntro();
}

MenuManager::~MenuManager()
{
	loadedSoundBuffers.clear();
	storedSounds.clear();
}