#include "Main.hpp"

void initializeGlobalData();

int main()
{
	initializeGlobalData();

	sf::RenderWindow window(sf::VideoMode(globals::windowSize.x, globals::windowSize.y), "deltaXi", sf::Style::Close | sf::Style::Titlebar);
#ifdef SFML_SYSTEM_WINDOWS
	__windowsHelper.setIcon(window.getSystemHandle());
#endif

	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		window.display();
	}

	return 0;
}

void initializeGlobalData()
{
	globals::windowSize = sf::Vector2i(1400, 800);
}