#include "MenuManager.hpp"
#include "PCH.hpp"
#include "../Objects/Clickables/Menu/MenuButton.hpp"
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
	bool isAnimatingIntro;
	bool isAnimatingFadeIn;
	float introAnimationDuration = 2; // Intro duration (in seconds)
	float introAnimationFadeinDuration = 1; // REAL VALUES: 8 and 5
	// Flying - Initialized in constructor
	float lowXLimit;
	float highXLimit;
	// Fading
	float lowFadeLimit = 0.f;
	float highFadeLimit = 255.f;
	// Scrolling animation
	bool isAnimatingScrolling;
	float scrollingAnimationDuration = 0.3f; // Scrolling duration (in seconds)
	enum ScrollingDirection { left, right };
	ScrollingDirection scrollingDirection;
	// Scrolling - Initialized in constructor
	float lowXRightLimit;
	float highXRightLimit;
	float lowXLeftLimit;
	float highXLeftLimit;
	// Transition animation
	bool isAnimatingTransition;
	float transitionAnimationDuration = 2.f; // Transition duration (in seconds)
	float delayUntilTransition = 1.f;
	float lowRadiusLimit;
	float highRadiusLimit;

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
}

void MenuManager::update()
{
	if (isAnimatingIntro) animateIntro();
	else if (isAnimatingScrolling) animateScrolling();

	for (int i = 0; i < amountMenuButtons; i++)
	{
		std::string key = menuButtonMapKey(i);
		if (!isAnimatingIntro && !isAnimatingScrolling)
		{
			if (menuButtons[key].isHovered())
				storedSounds["menuButtonHover"].play();
		}

		menuButtons[key].draw();
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
			if (!isAnimatingIntro && !isAnimatingScrolling && menuButtons[key].isHeld)
			{
				if (menuButtons[key].index == selectedSimulation)
					Logger::log("Clicked index " + std::to_string(i));
				if (menuButtons[key].index > selectedSimulation)
					startScrolling("right");
				if (menuButtons[key].index < selectedSimulation)
					startScrolling("left");

			}

			menuButtons[key].draw();
		}
	}

	return;
}

void MenuManager::keyPressed(sf::Keyboard::Key key, bool control, bool alt, bool shift, bool system)
{
	if (key == sf::Keyboard::Space && amountMenuButtons > 0 && !isAnimatingIntro && !isAnimatingScrolling)
	{
		startIntro();
	}

	if (key == sf::Keyboard::Right && selectedSimulation != amountMenuButtons - 1 && !isAnimatingIntro && !isAnimatingScrolling)
	{
		startScrolling("right");
	}

	if (key == sf::Keyboard::Left && selectedSimulation != 0 && !isAnimatingIntro && !isAnimatingScrolling)
	{
		startScrolling("left");
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
	Logger::log("Running animation");
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

		storedSounds["menuFade"].play(); // Play manu fade in sound

		animationClock.restart();
		return;
	}
	if(isAnimatingFadeIn && elapsedTime >= introAnimationFadeinDuration) // Intro animation done
	{
		isAnimatingIntro = false;
		isAnimatingFadeIn = false;

		for (auto& [key, value]: menuButtons)
		{
			value.soundPlayed = false;
		}

		globals::mainWindow.inst.setMouseCursorVisible(true); // Set cursor visible
		Logger::log("Finished animation");
	}
}

void MenuManager::startScrolling(std::string direction)
{
	animationClock.restart();
	isAnimatingScrolling = true;
	if (direction == "right") scrollingDirection = ScrollingDirection::left;
	if (direction == "left") scrollingDirection = ScrollingDirection::right;
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

void MenuManager::startTransition()
{

}

void MenuManager::animateTransition()
{

}

void MenuManager::addMainMenuButton(std::string name)
{
	std::string key = menuButtonMapKey(amountMenuButtons); // Set current key to the amount of menu buttons (so that particles = 0 etc...)
	std::string key_sounds = menuButtonSoundMapKey(amountMenuButtons);

	stringvector scenes { "default" };
	menuButtons[key] = MenuButton(name, menuButtonRadius, menuButtonPointCount, menuButtonFillColor, menuButtonOutlineColor, menuButtonOutlineThickness, menuButtonHoldColor, amountMenuButtons);

	loadedSoundBuffers[key_sounds] = ResourceController::loadMenuButtonSoundBufferFromMemory(amountMenuButtons);
	storedSounds[key_sounds] = sf::Sound(loadedSoundBuffers[key_sounds]);

	amountMenuButtons++;
	Logger::log("Total: " + std::to_string(amountMenuButtons) + " menu buttons");
}

void MenuManager::addSimulations()
{
	addMainMenuButton("Particle");
	addMainMenuButton("Doppler");
	addMainMenuButton("Pendulum");
	addMainMenuButton("Orbits");
}

MenuManager::MenuManager()
{
	loadedSoundBuffers["menuFade"] = ResourceController::loadSoundBufferFromMemory((void*)menuFade_data, menuFade_data_length);
	storedSounds["menuFade"] = sf::Sound(loadedSoundBuffers["menuFade"]);

	loadedSoundBuffers["menuButtonHover"] = ResourceController::loadSoundBufferFromMemory((void*)mouseOver_data, mouseOver_data_length);
	storedSounds["menuButtonHover"] = sf::Sound(loadedSoundBuffers["menuButtonHover"]);

	windowSize = globals::windowSize;

	lowXLimit = -1.2f * menuButtonRadius;
	highXLimit = windowSize.x + 1.2f * menuButtonRadius;

	menuButtonRadius = menuButtonSize * windowSize.y;
	middlePoint = sf::Vector2f(windowSize.x / 2, windowSize.y / 2);

	lowXLeftLimit = -2.f * menuButtonRadius;
	highXLeftLimit = -2.f * menuButtonRadius / 3.f;
	lowXRightLimit = windowSize.x + 2.f * menuButtonRadius;
	highXRightLimit = windowSize.x + 2.f * menuButtonRadius / 3.f;

	lowRadiusLimit = menuButtonRadius;
	highRadiusLimit = windowSize.x;

	(void)setTimer; // Silence unused function warning
	(void)getTimer;

	addSimulations();

	startIntro();
}

MenuManager::~MenuManager()
{
	loadedSoundBuffers.clear();
	storedSounds.clear();
}